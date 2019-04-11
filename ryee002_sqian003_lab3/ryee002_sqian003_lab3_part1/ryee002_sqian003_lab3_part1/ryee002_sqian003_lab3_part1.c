/*	Partner(s) Name & E-mail:Robert Yee (Ryee002), Stephanie Qian (Sqian003)
 *	Lab Section: 022
 *	Assignment: Lab # 3  Exercise # 1 
 *	Exercise Description: [optional - include for your own benefit]
 *	Count # of 1 ports on A and B and output onto C	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/*
 * ryee002_sqian003_lab3_part1.c
 *
 * Created: 1/15/2019 11:44:31 AM
 * Author : Robert Yee
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
    DDRA = 0x00; PORTA = 0xFF; // 8 pin input for port A 
	DDRB = 0x00; PORTB = 0xFF; // 8 pin input for port B
	DDRC = 0xFF; PORTC = 0x00; // 8 pin output for port C

	unsigned char tempA = 0x00;
	unsigned char tempB = 0x00;
	unsigned char count = 0x00;
	while(1){
		tempA = 0x00;
		tempB = 0x00;
		tempA = PINA;
		count = 0x00;
		while (tempA != 0x00) {
			if ((tempA & 0x01) == 1) {
				++count;
			} 
			tempA = tempA >> 1;
		}
		tempB = PINB;
		while (tempB != 0x00) {
    		if ((tempB & 0x01) == 1) {
        		++count;
    		}
    		tempB = tempB >> 1;
		}
		PORTC = count;
	}
}

