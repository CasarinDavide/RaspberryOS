
#include "mini_uart.h"
#define TDX 14
#define RXD 15

/**
 * 
 * 	The mini UART is a secondary low throughput4
		UART intended to be used as a console. It
		needs to be enabled before it can be used. It is also recommended that the correct GPIO
		function mode is selected before enabling the mini Uart.
		The mini Uart has the following features:
		• 7 or 8 bit operation.
		• 1 start and 1 stop bit.
		• No parities.
		• Break generation.
		• 8 symbols deep FIFOs for receive and transmit.
		• SW controlled RTS, SW readable CTS.
		• Auto flow control with programmable FIFO level.
		• 16550 like registers.
		• Baudrate derived from system clock.
		This is a mini UART and it does NOT have the following capabilities:
		• Break detection
		• Framing errors detection.
		• Parity bit
		• Receive Time-out interrupt
		• DCD, DSR, DTR or RI signals
 * 
 */




/*
	UART Initialization (uart_init):

	This function configures the GPIO pins and auxiliary registers to set up the mini UART for communication.
	GPIO Configuration: The writeFSel() function is used to set the GPIO pins 14 (TDX - Transmit Data) 
	and 15 (RXD - Receive Data) to Alternate Function 5 mode, which configures them for UART transmission and reception.

	Disabling Pull-up/down resistors: The execFsel() function calls offPUD() to disable pull-up/down resistors on the transmit (TDX) and receive (RXD) GPIO pins.

	Enabling the mini UART: The aux_reg_map->enables register is set to 1 to enable the UART.

	Configuring the mini UART:
	
	Interrupts are disabled (aux_reg_map->mu_ier = 0).
	
	The control register is cleared (aux_reg_map->mu_control = 0).
	
	The line control register is set to enable 8-bit operation (aux_reg_map->mu_lcr = 3).
	
	The modem control register is cleared (aux_reg_map->mu_mcr = 0).
	
	The baud rate is set to 270 (aux_reg_map->mu_baud_rate = 270), which corresponds to the desired communication speed.
*/
void uart_init(volatile gpio* gpio_reg,volatile auxiliary_register_map* aux_reg_map)
{
	// 
	writeFSel(gpio_reg,TDX,Alt_Fun_5_state);
	writeFSel(gpio_reg,RXD,Alt_Fun_5_state);

	volatile void (*f) = &offPUD;
	execFsel(f,gpio_reg,TDX);
	execFsel(f,gpio_reg,RXD);

	aux_reg_map->enables = 1;
	aux_reg_map->mu_ier = 0;
	aux_reg_map->mu_control = 0;
	aux_reg_map->mu_lcr = 3;
	aux_reg_map->mu_mcr = 0;
	aux_reg_map->mu_baud_rate = 270;
}



/*
	Sending/Reciving a Character (urt_send_char):
	This function waits for the UART transmitter to be ready and then sends a single character (char) through the UART.
	Transmit Status: It waits until the transmit FIFO is empty (using the mu_lsr register) by checking if bit 5 is set (0x20).
	Sending Data: Once ready, the character is written to the mu_io register, which handles data transmission.

*/
void urt_send_char(volatile auxiliary_register_map* aux_reg_map, char c)
{
	while(!(aux_reg_map->mu_lsr & 0x20));
	aux_reg_map->mu_io = c;		
}


char urt_recive_char(volatile auxiliary_register_map* aux_reg_map)
{
	while(!(aux_reg_map->mu_lsr & 1));
	// read 1 byte from the buffer
	return (char)aux_reg_map->mu_io & 0xFF;
}


void uart_send_string(volatile auxiliary_register_map* aux_reg_map,char *s)
{
	char* tmp = s;
	while(*tmp != '\0')
	{
		urt_send_char(aux_reg_map,*tmp);
		tmp++;
	}
}


