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

/*** Macros ******************************************************************/
#define SYSTICK_ONE_MILLISECOND ((uint32_t)FUNCONF_SYSTEM_CORE_CLOCK / 1000)
#define SYSTICK_ONE_MICROSECOND ((uint32_t)FUNCONF_SYSTEM_CORE_CLOCK / 1000000)
#define millis() (g_systick_millis)
#define micros() (SysTick->CNT / SYSTICK_ONE_MICROSECOND)

// Timing Variables
#define DISPLAY_REFRESH_MILLIS 5

/*** Globals *****************************************************************/
// Incremented in the SysTick IRQ once per millisecond
volatile uint32_t g_systick_millis;

// Display Data Array [0] = Left    [1] = Right
uint16_t g_seg_data[2] = {ascii_font['K' - ' '], ascii_font[9]};

// UART RX Ring Buffer
#define UART_RING_BUFFER_SIZE 64
static uint8_t g_uart_ring_buffer[UART_RING_BUFFER_SIZE];

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
/// @brief Initialise the SysTick interrupt to incriment every 1 millisecond
/// @param None
/// @return None
static void systick_init(void);

/// @brief SysTick IRQ
/// @param none
/// @return none
__attribute__((interrupt))
void SysTick_Handler(void); 

/// @brief Sets all segments in the display
/// @param uint16_t segment bits
/// @return None
__attribute__((always_inline))
static inline void disp_write(const uint16_t seg_data);

/// @brief Initialises the Display pins and sets it into a blank state
/// @param None
/// @return None
static void disp_init(void);

/// @brief Update which segment is visible, and set the Segment pins
/// @param None
/// @return None
static void disp_refresh(void);

/// @brief Set the global display data from an input string of 2 chars
/// @param char*, array of chars (2 chars)
/// @return None
__attribute__((always_inline))
static inline void disp_set_string(const char *string);

/// @brief Parse the received ASCII String, set data to be displayed, and data
/// to be transmitted to the next display.







/*** Main ********************************************************************/
int main(void)
{
	SystemInit();
	systick_init();

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



	disp_set_string("Hi");


	// Display Refresh and UART Parsing loop
	while(1)
	{
		// Refresh the display once per TODO:
		static uint32_t disp_last_refresh = 0;
		if(millis() - disp_last_refresh > DISPLAY_REFRESH_MILLIS)
		{
			disp_refresh();
			disp_last_refresh = millis();
		}
		
		// TODO: trigger or somehow set a flag to parse the UART Buffer if \n or \r were passed 



	}

	return 0;
}







/*** Functions ***************************************************************/
static void systick_init(void)
{
	// Reset any pre-existing configuration
	SysTick->CTLR = 0x0000;
	
	// Set the compare register to trigger once per millisecond
	SysTick->CMP = SYSTICK_ONE_MILLISECOND - 1;

	// Reset the Count Register, and the global millis counter to 0
	SysTick->CNT = 0x00000000;
	g_systick_millis = 0x00000000;
	
	// Set the SysTick Configuration
	// NOTE: By not setting SYSTICK_CTLR_STRE, we maintain compatibility with
	// busywait delay funtions used by ch32v003_fun.
	SysTick->CTLR |= SYSTICK_CTLR_STE   |  // Enable Counter
	                 SYSTICK_CTLR_STIE  |  // Enable Interrupts
	                 SYSTICK_CTLR_STCLK ;  // Set Clock Source to HCLK/1
	
	// Enable the SysTick IRQ
	NVIC_EnableIRQ(SysTicK_IRQn);
}


__attribute__((interrupt))
void SysTick_Handler(void)
{
	// Increment the Compare Register for the next trigger
	SysTick->CMP += SYSTICK_ONE_MILLISECOND;

	// Clear the trigger state for the next IRQ
	SysTick->SR = 0x00000000;

	// Increment the milliseconds count
	g_systick_millis++;
}


__attribute__((always_inline))
static inline void disp_write(const uint16_t seg_data)
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
	for(uint8_t seg_pin = 0; seg_pin < 15; seg_pin++)
	{
		gpio_set_mode(segment_pin[seg_pin], OUTPUT_10MHZ_PP);
		gpio_digital_write(segment_pin[seg_pin], GPIO_LOW);
	}

	// Set the Display Control Pin to OUTPUT, LOW
	gpio_set_mode(digit_pin, OUTPUT_10MHZ_PP);
	gpio_digital_write(digit_pin, GPIO_LOW);

	// Set the global display data to 0
	g_seg_data[0] = 0x0000;
	g_seg_data[1] = 0x0000;
}


static void disp_refresh(void)
{
	// Display Selection Pin state. 0 = Left    1 = Right 
	static uint8_t digit_state = 0;

	// Write the current digit state
	gpio_digital_write(digit_pin, digit_state);
	
	// Blank all the segments to prevent ghosting
	disp_write(0x0000);
	// Write the global segment data to the segment chosen
	disp_write(g_seg_data[digit_state]);

	// Invert the digit_state for the next loop
	digit_state = !digit_state;
}


static void disp_set_string(const char *string)
{
	// Guard against null input
	if(string == NULL) return;

	// Guard against chars being too low or high
	// To convert ASCII char to its index in the font, minus 'space' (0x20)
	// from the character
	if(string[0] > 0x1F && string[0] < 0x7F)
		g_seg_data[0] = ascii_font[ (string[0] - 0x20) ];

	if(string[1] > 0x1F && string[1] < 0x7F)
		g_seg_data[1] = ascii_font[ (string[1] - 0x20) ];
}
