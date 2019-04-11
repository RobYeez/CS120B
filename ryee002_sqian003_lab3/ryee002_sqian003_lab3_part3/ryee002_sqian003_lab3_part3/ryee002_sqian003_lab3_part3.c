/*	Partner(s) Name & E-mail: Robert Yee (Ryee002), Stephanie Qian (Sqian003)
 *	Lab Section: 022
 *	Assignment: Lab # 3 Exercise # 3 
 *	Exercise Description: [optional - include for your own benefit]
 *	Light for gas fuel sensor + belt sensor/key ignition
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/*
 * ryee002_sqian003_lab3_part2.c
 *
 * Created: 1/15/2019 1:17:39 PM
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
	
	unsigned char tempAback = 0x00;
	unsigned char tempA = 0x00;
	unsigned char tempC = 0x00;
	
	while(1) {
		tempAback = 0x00;
		tempAback = PINA & 0x0F;
		PORTC = 0x00;
	/*	tempA = 0x00;
		tempC = 0x00; */
		if(tempAback == 0x01 || tempAback == 0x02) 
		{
			tempC = 0x60; //Includes port6 
		}
		else if (tempAback == 0x03 || tempAback == 0x04) 
		{
			tempC = 0x70; //Includes port6
		}
		else if (tempAback == 0x05 || tempAback == 0x06)
		{
			tempC = 0x38;
		}
		else if (tempAback == 0x07 || tempAback == 0x08 || tempAback == 0x09)
		{
			tempC = 0x3C;
		}
		else if (tempAback == 0x0A || tempAback == 0x0B || tempAback == 0x0C) //10,11,12
		{
			tempC = 0x3E;
		}
		else if (tempAback == 0x0D || tempAback == 0x0E || tempAback == 0x0F) //13,14,15
		{
    		tempC = 0x3F;
		}
		tempA = PINA;
		if ((tempA & 0x70) == 0x30){
			tempC = (tempC | 0x80);
		}
		PORTC = tempC;
	}
}	    

