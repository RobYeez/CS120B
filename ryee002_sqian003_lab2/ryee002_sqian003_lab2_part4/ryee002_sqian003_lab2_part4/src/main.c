/*	Partner(s) Name & E-mail:Robert Yee (Ryee002), Stephanie Qian (Sqian003)
 *	Lab Section: 022
 *	Assignment: Lab #2  Exercise #4 
 *	Exercise Description: [optional - include for your own benefit]
 *	Challenge exercise: Amusement part ride exercise
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/**
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
#include <asf.h>

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF; //8 input A
    DDRB = 0x00; PORTB = 0xFF; //8 input B
	DDRC = 0x00; PORTC = 0xFF; //8 input C
    DDRD = 0xFF; PORTD = 0x00; //8 output D
	unsigned char weightA = 0x00; //weight of personA
	unsigned char weightB = 0x00; //weight of personB
	unsigned char weightC = 0x00; //weight of personC
	unsigned char total_weight = 0x00; //total weight

	while(1){
		PORTD = 0x00; //helps for testing
		weightA = PINA; //test example 105 x69 1101001
		weightB = PINB; //test example 45 x2D 101101
		weightC = PINC; //test example 3 x03 0011
		total_weight = weightA + weightB + weightC;
		PORTD = total_weight >> 2;
		PORTD = PORTD << 2;
		//should have PD0 and PD1 as 0;
		if(total_weight > 140) {
			PORTD = PORTD + 0x01;
		}	
		if(weightA - weightC > 80) {
			PORTD = PORTD + 0x02;
		}	
	}
}