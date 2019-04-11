/*	Partner(s) Name & E-mail:Robert Yee (ryee002), Stephanie Qian (Sqian003)
 *	Lab Section: 022
 *	Assignment: Lab # 8 Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *	Light sesor with max or min
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/*
 * ryee002_sqian003_lab8_part1.c
 *
 * Created: 2/6/2019 11:40:05 PM
 * Author : Robert Yee
 */ 

#include <avr/io.h>
unsigned short my_short = 0x0000;
unsigned char MAX = 0x7F;
//unsigned char one_eigth = MAX / 8;

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
		if (my_short/8 >= 7 * (MAX/8) && (my_short/8 < MAX)) {
			PORTB = 0x7F;
		}
		else if (my_short/8 >= 6 * (MAX/8) && (my_short/8 < 7 * (MAX/8))){
			PORTB = 0x3F;
		}
		else if (my_short/8 >= 5 * (MAX/8) && (my_short/8 < 6 * (MAX/8))) {
			PORTB = 0x1F;
		}
		else if (my_short/8 >= 4 * (MAX/8) && (my_short/8 < 5 * (MAX/8))){
			PORTB = 0x0F;
		}
		else if (my_short/8 >= 3 * (MAX/8) && (my_short/8 < 4 * (MAX/8))) {
			PORTB = 0x07;
		}
		else if (my_short/8 >= 2 * (MAX/8) && (my_short/8 < 3 * (MAX/8))){
			PORTB = 0x03;
		}
		else if (my_short/8 >= 1 * (MAX/8) && (my_short/8 < 2 * (MAX/8))) {
			PORTB = 0x01;
		}
		else if (my_short/8 >= 0 * (MAX/8) && (my_short/8 < 1 * (MAX/8))) {
			PORTB = 0x00;
		}	
	}
}

