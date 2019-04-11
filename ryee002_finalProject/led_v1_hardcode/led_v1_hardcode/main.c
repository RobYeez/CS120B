/*
 * ledv4_with_includes.c
 *
 * Created: 3/10/2019 3:14:23 PM
 * Author : Robert Yee
 */ 

#include <avr/io.h>

unsigned char pattern = 0x69; //PORTC 1= on 0 = off
unsigned char row = 0x00; // PORTD 1= off 0 = on

int main(void)
{
    /* Replace with your application code */
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	PORTC = pattern;
	PORTD = row;
}

