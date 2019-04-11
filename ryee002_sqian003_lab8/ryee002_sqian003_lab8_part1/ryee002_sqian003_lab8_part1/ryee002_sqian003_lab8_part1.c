/*	Partner(s) Name & E-mail:Robert Yee (ryee002), Stephanie Qian (Sqian003)
 *	Lab Section: 022
 *	Assignment: Lab # 8 Exercise #1 
 *	Exercise Description: [optional - include for your own benefit]
 *	Potentimeter led
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/*
 * ryee002_sqian003_lab8_part1.c
 *
 * Created: 2/6/2019 10:11:05 PM
 * Author : Robert Yee
 */ 

#include <avr/io.h>
unsigned short my_short = 0x0000;


void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}


int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	ADC_init();
    while (1) 
    {
		my_short = ADC;
		PORTB = (char)my_short; // my_char = 0xCD
		PORTD = (char)(my_short >> 8); // my_char = 0xAB
    }
}

