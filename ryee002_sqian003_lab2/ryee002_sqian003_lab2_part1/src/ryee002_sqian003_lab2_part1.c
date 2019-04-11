/*	Partner(s) Name & E-mail:Robert Yee (Ryee002), Stephanie Qian (Sqian003)
 *	Lab Section: 022
 *	Assignment: Lab #2  Exercise #1 
 *	Exercise Description: [optional - include for your own benefit]
 *	Parking sensor with light problem
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
/*
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <avr/io.h>

int main (void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char tmpA = 0x00;
	while(1) {	
		tmpA = PINA & 0x01;
		if (tmpA == (PINA | 0x01)) //when A0 = 1 then true
		{
			PORTB = 0x01;
		}
		else
		{
    		PORTB = 0x00;
		}
	}	

	return 0;
}
