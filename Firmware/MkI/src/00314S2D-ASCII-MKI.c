/******************************************************************************
* 00314S2D-ASCII MkI
* A 2 Digit, 14 Segment Display Module using the CH32V003 MCU.
* For more information see the GitHub: 
* https://github.com/ADBeta/00314S2D-ASCII_Display_Module
*
* Functional Description:
* Each board Receives data via RX, and transmits it via TX at 115200 Baud
* When receiving data, each unit refils its internal display buffer, when it 
* is full, any incoming data is echoed directly to TX down the chain - due to
* this you can have a theoretically infinte number of displays 
*
* When power is applied, a self check takes place to look for the presense of
* a WCHLink programmer - if SWIO is asserted HIGH, a programmer is present,
* and the deivce will boot into a blank waiting mode to be programmed.
* - if the SWIO pin is left open, it boots into regular display mode
*
* Ver 0.1    14 Oct 2024 
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
#define DISPLAY_REFRESH_MILLIS 2

/*** Globals *****************************************************************/
// Incremented in the SysTick IRQ once per millisecond
volatile uint32_t g_systick_millis;

// Display Data Array [0] = Left    [1] = Right
uint16_t g_seg_data[2];

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

static void detect_programmer(void);

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

/*** Main ********************************************************************/
int main(void)
{
	SystemInit();
	systick_init();

	// Checks if the SWIO Porgramming pin is connected. Waits forever if so 
	detect_programmer();
	
	// Initialise the display. Disables SWIO
	disp_init();
	
	// Setup and Initialise UART (Default Pinout)
	uart_config_t uart_conf = {
		.baudrate    = UART_BAUD_115200,
		.wordlength  = UART_WORDLENGTH_8,
		.parity      = UART_PARITY_NONE,
		.stopbits    = UART_STOPBITS_ONE,
		.flowctrl    = UART_FLOWCTRL_NONE,
	};

	uart_init(&uart_conf);	

	// Display Refresh and UART Parsing loop
	while(1)
	{
		// Refresh the display when the target millis are reached
		static uint32_t disp_last_refresh = 0;
		if(millis() - disp_last_refresh > DISPLAY_REFRESH_MILLIS)
		{
			disp_refresh();
			disp_last_refresh = millis();
		}
		
		// An interrupt will handle all display setting. Only refresh in loop
	}
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

static void detect_programmer(void)
{
	// Set PD1 (SWIO) to INPUT_PULLDOWN - the programmer pulls HIGH
	gpio_set_mode(GPIO_PD1, INPUT_PULLDOWN);
	// Wait for line to settle
	Delay_Ms(100);
	// Detect the SWIO pins' state
	if(gpio_digital_read(GPIO_PD1) == GPIO_HIGH)
	{
		// Stop pulling SWIO so programming can happen
		gpio_set_mode(GPIO_PD1, INPUT_FLOATING);
		// Wait
		while(1) { }
	}
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
	// Enable Alternate GPIO Funtionality and disable SWIO
	RCC->APB2PCENR |= RCC_APB2Periph_AFIO;
	AFIO->PCFR1    |= 0x04000000;

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

	// Blank all the segments to prevent ghosting
	disp_write(0x0000);
	// Write the current digit state
	gpio_digital_write(digit_pin, digit_state);	
	// Write the global segment data to the segment chosen
	disp_write(g_seg_data[digit_state]);

	// Invert the digit_state for the next loop
	digit_state = !digit_state;
}


/*** IRQHandler for UART ****************************************************/
/// @brief UART Receiver Interrupt handler - Puts the data received into the
/// UART Ring Buffer
/// @param None
/// @return None
void USART1_IRQHandler(void) __attribute__((interrupt));
void USART1_IRQHandler(void)
{
	if(USART1->STATR & USART_STATR_RXNE) 
	{
		// Set 2 chars on this display, after that echo to the next display.
		static uint8_t char_idx = 0;
		bool echo_flag = 0;


		// Read from the DATAR Register to reset the flag
		uint8_t recv = (uint8_t)USART1->DATAR;

		/*** Handle incomming chars ******************************************/
		// Check if the received byte is a control char
		// Newline and CR are treated the same
		if(recv == ASCII_CONTROL_LINE_FEED || 
		   recv == ASCII_CONTROL_CARRIAGE_RETURN)
		{
			// Clear displays and reset char counter
			g_seg_data[0] = 0x0000;
			g_seg_data[1] = 0x0000;
			char_idx = 0;

			// Mark this byte to be echoed forward
			echo_flag = true;

		// NOTE: Add more control char beviour at this point
		
		// Any regular ASCII Char
		} else if(recv >= 0x20 && recv <= 0x7E)
		{
			if(char_idx < 2)
			{
				// Set the corresponding char with the received data
				g_seg_data[char_idx] = ascii_font[recv - 0x20];
				char_idx++;
			} else {
				// Set flag to re-transmit the received data
				echo_flag = true;
			}
		}


		// Transmit the received byte if the flag is set
		if(echo_flag)
		{
			while(!(USART1->STATR & USART_FLAG_TC));
			USART1->DATAR = recv;
		}
	}
}
