/*	Partner(s) Name & E-mail: Robert Yee (Ryee002), Stephanie Qian (Sqian003)
 *	Lab Section: 022
 *	Assignment: Lab # 5 Exercise # 1 
 *	Exercise Description: [optional - include for your own benefit]
 *	Light for gas fuel sensor + belt sensor/key ignition + button breadboard
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/*
 * ryee002_sqian003_lab3_part2.c
 *
 * Created: 1/27/2019 4:39:39 PM
 * Author : Robert Yee
 */ 

#include <avr/io.h>


int main(void)
{
	unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
    	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
	}
	unsigned char GetBit(unsigned char x, unsigned char k) {
    	return ((x & (0x01 << k)) != 0);
	}

    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	unsigned char tempA = 0x00;
	unsigned char tempC = 0x00;
	
	while(1) {
		tempA = ~PINA;
		
		if((tempA & 0x01) == 0x01 || (tempA & 0x02)== 0x02) 
		{
			tempC = 0x20; //Includes port6 
		}
		else if ((tempA & 0x03) == 0x03 || (tempA & 0x04) == 0x04) 
		{
			tempC = 0x30; //Includes port6
		}
		else if ((tempA & 0x05) == 0x05 || (tempA & 0x06) == 0x06)
		{
			tempC = 0x28;
		}
		else if ((tempA & 0x07) == 0x07 || (tempA & 0x08) == 0x08 || (tempA & 0x09) == 0x09)
		{
			tempC = 0x3C;
		}
		else if ((tempA & 0x0A) == 0x0A || (tempA & 0x0B) == 0x0B || (tempA & 0x0C) == 0x0C) 
		{
			tempC = 0x3E;
		}
		else if ((tempA & 0x0D) == 0x0D || (tempA & 0x0E) == 0x0E || (tempA & 0x0F) == 0x0F)
		{
    		tempC = 0x3F;
		}
		PORTC = tempC;
	}	    
}

