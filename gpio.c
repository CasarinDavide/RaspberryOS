#include "gpio.h"

/**
 *	GPIO Function Select Registers (GPFSELn) 
 * Usually for each select register this is his structure
 * except for number 5
 * 29-27 FSEL9 FSEL9 - Function Select 9
	000 = GPIO Pin 9 is an input
	001 = GPIO Pin 9 is an output
	100 = GPIO Pin 9 takes alternate function 0
	101 = GPIO Pin 9 takes alternate function 1
	110 = GPIO Pin 9 takes alternate function 2
	111 = GPIO Pin 9 takes alternate function 3
	011 = GPIO Pin 9 takes alternate function 4
	010 = GPIO Pin 9 takes alternate function 5
	R/W 0
	26-24 FSEL8 FSEL8 - Function Select 8 R/W 0
	23-21 FSEL7 FSEL7 - Function Select 7 R/W 0
	20-18 FSEL6 FSEL6 - Function Select 6 R/W 0
	17-15 FSEL5 FSEL5 - Function Select 5 R/W 0
	14-12 FSEL4 FSEL4 - Function Select 4 R/W 0
	11-9 FSEL3 FSEL3 - Function Select 3 R/W 0
	8-6 FSEL2 FSEL2 - Function Select 2 R/W 0
	5-3 FSEL1 FSEL1 - Function Select 1 R/W 0
	2-0 FSEL0 FSEL0 - Function Select 0 R/W 0 
 */

/**
 * GPIO Alternate function select register 4 
 * 	31-12 --- Reserved R 0
		11-9 FSEL53 FSEL53 - Function Select 53
		000 = GPIO Pin 53 is an input
		001 = GPIO Pin 53 is an output
		100 = GPIO Pin 53 takes alternate function 0
		101 = GPIO Pin 53 takes alternate function 1
		110 = GPIO Pin 53 takes alternate function 2
		111 = GPIO Pin 53 takes alternate function 3
		011 = GPIO Pin 53 takes alternate function 4
		010 = GPIO Pin 53 takes alternate function 5
		R/W 0
		8-6 FSEL52 FSEL52 - Function Select 52 R/W 0
		5-3 FSEL51 FSEL51 - Function Select 51 R/W 0
		2-0 FSEL50 FSEL50 - Function Select 50 R/W 0
 * 
 * 
 */



void writeFSel(volatile gpio* gpio_reg,uint32 register_number,GPIO_REG_STATE pin_state)
{
	uint8 pos = register_number / 10;
	volatile gpfsel* gpio_sel = &gpio_reg->gpsel_reg[pos];
	uint32 to_shift = (register_number % 10) * 3;
	uint32 bit_mask = (0x7 << to_shift);
	gpio_sel->data &= ~bit_mask;
	gpio_sel->data |= (pin_state << to_shift); 
}

/***
   31-2 --- Unused R 0
   1-0 PUD PUD - GPIO Pin Pull-up/down
   00 = Off – disable pull-up/down
   01 = Enable Pull Down control
   10 = Enable Pull Up control
   11 = Reserved
   * Use in conjunction with GPPUDCLK0/1/2 R/W 0
*/

/* Function to disable pull-up/down (Off) */
void offPUD(volatile gpio* gpio_reg)
{
    gpio_reg->gppud = 0x0;  // Set GPPUD to 00 to disable pull-up/down
}

/* Function to enable pull-down resistor */
void enablePullDownPUD(volatile gpio* gpio_reg)
{
    gpio_reg->gppud = 0x1;  // Set GPPUD to 01 to enable pull-down
}

/* Function to enable pull-up resistor */
void enablePullUpPUD(volatile gpio* gpio_reg)
{
    gpio_reg->gppud = 0x2;  // Set GPPUD to 10 to enable pull-up
}


/**
 * Executes a function (offPUD, enablePullDownPUD, enablePullUpPUD) 
 * and applies the clock to the specified pin.
 * 
 * @param f: Function pointer to offPUD, enablePullDownPUD, or enablePullUpPUD
 * @param gpio_reg: GPIO register structure
 * @param number: GPIO function select register (0 to 6)
 * @param pinNumber: Pin number within the FSEL register (0 to 9)
 */
void execFsel(void (*f)(volatile gpio*), volatile gpio* gpio_reg,uint32 pin_number)
{
    // Execute the passed function to set pull-up/down state
    f(gpio_reg);

    // Delay to allow the configuration to settle (assuming delay is in milliseconds)
    delay(150);

    // Calculate the physical pin number (FSEL register and pin index within that register)
    
    
    // Apply clock to the correct GPPUDCLK register based on the physical pin number
    if (pin_number < 32)
    {
        gpio_reg->gppudclk0 = 1 << pin_number;  // Set clock in GPPUDCLK0 for pins 0-31
    }
    else
    {
        gpio_reg->gppudclk1 = 1 << (pin_number - 32);  // Set clock in GPPUDCLK1 for pins 32-53
    }

    // Another delay to allow the clock configuration to settle
    delay(150); 

    // Clear the clock in the GPPUDCLK register to finalize the configuration
    if (pin_number < 32)
    {
        gpio_reg->gppudclk0 = 0;  // Clear clock for pins 0-31
    }
    else
    {
        gpio_reg->gppudclk1 = 0;  // Clear clock for pins 32-53
    }

    // Disable the pull-up/down configuration to avoid further changes
    offPUD(gpio_reg);
}





