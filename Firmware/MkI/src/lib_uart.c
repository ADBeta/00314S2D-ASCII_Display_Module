/******************************************************************************
* This is a modified version of lib_uart, to remove the ring buffer,
* and incorperate the ISR into main.c
*
* lib_uart - A simple but full-featured library for UART on the CH32V003
*
* See GitHub for more information: 
* https://github.com/ADBeta/CH32V003_lib_uart
* 
* Released under the MIT Licence
* Copyright ADBeta (c) 2024
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to
* deal in the Software without restriction, including without limitation the 
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
* sell copies of the Software, and to permit persons to whom the Software is 
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in 
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE 
* USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/
#include "lib_uart.h"

#include "ch32v003fun.h"
#include <stddef.h>
#include <stdbool.h>

/*** Static Variables ********************************************************/
// Bool flag to prevent users from getting suck in a read/write loop if the
// UART was not able to init
static bool _uart_init_ok = false;

/*** Initialiser *************************************************************/
uart_err_t uart_init(const uart_config_t *conf)
{	
	// Enable UART1 Clock
	RCC->APB2PCENR |= RCC_APB2Periph_USART1;
	// Enable the UART GPIO Port, and the Alternate Function IO Flag
	RCC->APB2PCENR |= UART_TXRX_PORT_RCC | RCC_APB2Periph_AFIO;

	// Reset, then set the AFIO Register depending on UART_PINOUT selected
	AFIO->PCFR1 &= UART_AFIO_RESET_MASK;
	AFIO->PCFR1 |= UART_AFIO_MASK;

	// TODO: RTS CTS pins if set
	// Set up the GPIO Pins for UART
	// TX 10MHz PP AF
	// RX INPUT_FLOATING
	UART_TXRX_PORT->CFGLR &= ~(0x0F << (4 * UART_PIN_TX));
	UART_TXRX_PORT->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF) << (4 * UART_PIN_TX);	
	UART_TXRX_PORT->CFGLR &= ~(0x0F << (4 * UART_PIN_RX));
	UART_TXRX_PORT->CFGLR |= GPIO_CNF_IN_FLOATING << (4 * UART_PIN_RX);

	// Set CTLR1 Register (Enable RX & TX, set Worhummusd Length and Parity)
	USART1->CTLR1 = USART_Mode_Tx | USART_Mode_Rx | conf->wordlength | conf->parity;
	// Set CTLR2 Register (Stopbits)
	USART1->CTLR2 = conf->stopbits;
	// Set CTLR3 Register (Flow control)
	USART1->CTLR3 = (uint16_t)0x0000 | conf->flowctrl;
	
	// Set the Baudrate, assuming 48KHz
	USART1->BRR = conf->baudrate;

	// Enable the UART RXNE Interrupt
	USART1->CTLR1 |= USART_CTLR1_RXNEIE;
	NVIC_EnableIRQ(USART1_IRQn);
	
	// Enable the UART
	USART1->CTLR1 |= CTLR1_UE_Set;

	// Set ok flag and return OK
	_uart_init_ok = true;
	return UART_OK;
}

/*** Write *******************************************************************/
uart_err_t uart_write(const void *buffer, size_t size)
{
	if(!_uart_init_ok) return UART_NOT_INITIALIZED;
	if(buffer == NULL || size == 0) return UART_INVALID_ARGS;
	
	// Cast the input to a uint8_t
	const uint8_t *bytes = (const uint8_t *)buffer;
	while(size--)
	{
		// Wait for the current transmission to finish
		while(!(USART1->STATR & USART_FLAG_TC));
		USART1->DATAR = *bytes++;
	}
	
	return UART_OK;
}


uart_err_t uart_print(const char *string)
{
	if(!_uart_init_ok) return UART_NOT_INITIALIZED;
	if(string == NULL) return UART_INVALID_ARGS;
	
	while(*string != '\0')
	{
		// Wait for the current transmission to finish
		while(!(USART1->STATR & USART_FLAG_TC));
		USART1->DATAR = *string++;
	}
	
	return UART_OK;
}


uart_err_t uart_println(const char *string)
{
	// Catches NULL or invalid string and not init
	uart_err_t ret_err = uart_print(string);
	
	// If input was valid & sent, send \r\n
	if(ret_err == UART_OK)
	{
		// Print the terminating characters
		while(!(USART1->STATR & USART_FLAG_TC));
		USART1->DATAR = '\r';
		while(!(USART1->STATR & USART_FLAG_TC));
		USART1->DATAR = '\n';
	}

	return ret_err;
}
