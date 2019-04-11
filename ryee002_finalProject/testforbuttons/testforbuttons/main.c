/*
 * testforbuttons.c
 *
 * Created: 3/10/2019 11:27:00 PM
 * Author : Robert Yee
 */ 

#include <avr/io.h>

unsigned char button = 0x00;

int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    while (1) 
    {
		button = PINA & 0x04;
		if (button == 0x04) {
			PORTB = 0x00;
		}
		else {
			PORTB = 0x01;
		}
    }
}

