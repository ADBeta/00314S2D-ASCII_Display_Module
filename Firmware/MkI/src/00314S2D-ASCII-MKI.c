/******************************************************************************
* 00314S2D-ASCII MkI
* A 2 Digit, 14 Segment Display Module using the CH32V003 MCU.
* For more information see the GitHub:
*
*
* Functional Description:
* 
*
* 
* ADBeta (c) 2024
******************************************************************************/
#include "ch32v003fun.h"
#include "lib_gpioctrl.h"
#include "lib_uart.h"

#include "font.h"

#include <stdio.h>

/*** Globals *****************************************************************/
// Display Data Array [0] = Left    [1] = Right
uint16_t g_seg_data[2] = {ascii_font['K' - ' '], ascii_font[9]};

// UART RX Ring Buffer
#define UART_RING_BUFFER_SIZE 64
uint8_t g_uart_ring_buffer[UART_RING_BUFFER_SIZE];

// Digit Pin
static const GPIO_PIN digit_pin = GPIO_PD7;

// Segment Pins
static const GPIO_PIN segment_pin[15] = {
	GPIO_PC5,    // A
	GPIO_PC6,    // B
	GPIO_PC7,    // C
	GPIO_PD2,    // D
	GPIO_PC4,    // E
	GPIO_PA1,    // F
	GPIO_PD4,    // G1
	GPIO_PD3,    // G2
	GPIO_PA2,    // H
	GPIO_PD0,    // I
	GPIO_PC0,    // J
	GPIO_PC1,    // K
	GPIO_PC2,    // L
	GPIO_PC3,    // M
	GPIO_PD1,    // DP
};

/*** Forward Declaration *****************************************************/
/// @brief Sets all segments in the display
/// @param uint16_t segment bits
/// @return None
__attribute__((always_inline))
inline static void disp_set_segments(const uint16_t seg_data);

/// @brief Initialises the Display pins and sets it into a blank state
/// @param None
/// @return None
static void disp_init(void);

/// @brief Update which segment is visible, and set the Segment pins
/// @param None
/// @return None
static void disp_update(void);

/*** Main ********************************************************************/
int main(void)
{
	SystemInit();

	// NOTE: Development Phase delay to allow re-programming
	Delay_Ms(500);

	// Initialise the display
	disp_init();
	
	// Setup and Initialise UART (Default Pinout)
	uart_config_t uart_conf = {
		.baudrate    = UART_BAUD_115200,
		.wordlength  = UART_WORDLENGTH_8,
		.parity      = UART_PARITY_NONE,
		.stopbits    = UART_STOPBITS_ONE,
		.flowctrl    = UART_FLOWCTRL_NONE,
	};

	uart_init(g_uart_ring_buffer, UART_RING_BUFFER_SIZE, &uart_conf);





	while(1)
	{
		disp_update();
		Delay_Ms(10);
		/*
		Delay_Ms(1);
		gpio_digital_write(digit_pin, GPIO_HIGH);
		Delay_Ms(1);
		gpio_digital_write(digit_pin, GPIO_LOW);
		*/
	}

	return 0;
}


/*** Functions ***************************************************************/
__attribute__((always_inline))
inline static void disp_set_segments(const uint16_t seg_data)
{
	for(uint16_t seg_idx = 0; seg_idx < 15; seg_idx++)
	{
		uint8_t seg_bit = (seg_data >> seg_idx) & 0x01;
		gpio_digital_write(segment_pin[seg_idx], seg_bit);
	}
}


static void disp_init(void)
{
	// TODO: Re-enable this later
	// Enable Alternate GPIO Funtionality and disable SWIO
	//RCC->APB2PCENR |= RCC_APB2Periph_AFIO;
	//AFIO->PCFR1    |= 0x04000000;

	// TODO: Using FLASH, set the option bit to dsiable the NRST Pin

	// Set all Segment GPIO to OUTPUT, LOW
	// TODO: change name
	for(uint8_t p = 0; p < 15; p++)
	{
		gpio_set_mode(segment_pin[p], OUTPUT_10MHZ_PP);
		gpio_digital_write(segment_pin[p], GPIO_LOW);
	}

	// Set the Display Control Pin to OUTPUT, LOW
	gpio_set_mode(digit_pin, OUTPUT_10MHZ_PP);
	gpio_digital_write(digit_pin, GPIO_LOW);
}


static void disp_update(void)
{
	// Display Selection Pin state. 0 = Left    1 = Right 
	static uint8_t digit_state = 0;

	// Write the current digit state
	gpio_digital_write(digit_pin, digit_state);
	
	// Blank all the segments to prevent ghosting
	disp_set_segments(0x0000);
	// Write the global segment data to the segment chosen
	disp_set_segments(g_seg_data[digit_state]);

	// Invert the digit_state for the next loop
	digit_state = !digit_state;
}
