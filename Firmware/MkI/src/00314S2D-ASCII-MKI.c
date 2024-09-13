/******************************************************************************
******************************************************************************/
#include "ch32v003fun.h"
#include "lib_gpioctrl.h"

#include <stdio.h>

/*** Globals *****************************************************************/
// Digit Pin
const GPIO_PIN digit_pin = GPIO_PD7;

// Segment Pins
const GPIO_PIN segment_pin[15] = {
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


/*** Main ********************************************************************/
int main()
{
	SystemInit();

	// NOTE: Development Phase delay to allow re-programming
	Delay_Ms(5000);

	// Enable Alternate GPIO Funtionality and disable SWIO
	RCC->APB2PCENR |= RCC_APB2Periph_AFIO;
	AFIO->PCFR1    |= 0x04000000;

	// TODO: Using FLASH, set the option bit to dsiable the NRST Pin

	// Set up UART on the default Pins
	// TODO:

	// Set all Segment GPIO to OUTPUT, LOW
	for(uint8_t p = 0; p < 15; p++)
	{
		gpio_set_mode(segment_pin[p], OUTPUT_10MHZ_PP);
		gpio_digital_write(segment_pin[p], GPIO_HIGH);

	}

	// Set the Display Control Pin to OUTPUT, LOW
	gpio_set_mode(digit_pin, OUTPUT_10MHZ_PP);
	gpio_digital_write(digit_pin, GPIO_LOW);
	




	while(1)
	{
		Delay_Ms(100);
		gpio_digital_write(digit_pin, GPIO_HIGH);
		Delay_Ms(100);
		gpio_digital_write(digit_pin, GPIO_LOW);
	}
}
