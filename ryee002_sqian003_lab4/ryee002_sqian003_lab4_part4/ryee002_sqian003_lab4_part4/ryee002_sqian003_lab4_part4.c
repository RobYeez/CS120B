/*	Partner(s) Name & E-mail: Robert Yee (Ryee002), Stephanie Qian (Sqian003)
 *	Lab Section: 022
 *	Assignment: Lab # 4 Exercise # 4 
 *	Exercise Description: [optional - include for your own benefit]
 *	Door combination lock + same combo lock
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/*
 * ryee002_sqian003_lab4_3.c
 *
 * Created: 1/18/2019 10:33:21 AM
 * Author : Robert Yee
 */ 

#include <avr/io.h>
enum States {start, pound1, pound2, y3, unlock, locked} state;
unsigned char lockCode = 0x00;


void tick() {
	switch (state) //transitions 
	{
		case start:
		state = pound1;
		PORTB = 0x00;
		break;
		
		case pound1:
		if ((PINA & 0x04) == 0x04) { //pound = 1
			state = pound2;
			break;
		}
		else if ((PINA & 0x04) != 0x04 && lockCode == 0x01) { //incorrect lock code
			state = unlock;
			break;
		}		
		else {
			state = pound1;
			break;
		}
		
		case pound2:
		if((PINA & 0x04) == 0x00 && lockCode == 0x01) { //pound = 0 + trying to lock with same code
			state = y3;
			break;
		}
		else if ((PINA & 0x04) != 0x00 && lockCode == 0x01) { //incorrect lock code
			state = unlock;
			break;
		}		
		else if ((PINA & 0x04) == 0x00 && lockCode == 0x00) { //pound = 0
			state = y3;
			break;
		}
		else {
			state = pound1;
			break;
		}
		
		case y3:
		if ((PINA & 0x02) == 0x02 && lockCode == 0x01) { //y = 1 + lock with same code
			state = locked;
			break;
		}
		else if ((PINA & 0x02) != 0x02 && lockCode == 0x01) { //incorrect lock code
			state = unlock;
			break;
		}		
		else if ((PINA & 0x02) == 0x02 && lockCode == 0x00) { //y = 1
			state = unlock;
			PORTB = 0x01;
			break;
		}
		else {
			state = pound1;
			break;
		}
		
		case unlock:
		if ((PINA & 0x80) == 0x80) { //A7
			state = locked;
			break;
		}
		else if ((PINA & 0x80) == 0x80 && lockCode == 0x01) { //incorrect lock code
			state = locked;
			break;
		}		
		else if ((PINA & 0x04) == 0x04) { //start of same lock code
			lockCode = 0x01;
			state = pound2;
			break;	
		}
		else {
			break;
		}
		
		case locked:
		state = pound1;
		PORTB = 0x00;
		break;
		
		default:
		state = pound1;
		PORTB = 0x00;
		break;
	}
	switch(state) { //state actions
		case start:
		break;
		
		case pound1:
		PORTC = 0x01;
		break;
	
		case pound2:
		PORTC = 0x02;
		break;
		
		case y3:
		PORTC = 0x03;
		break;
		
		case unlock:
		PORTC = 0x04;
		break;
		
		case locked:
		PORTC = 0x05;
		break;
		
		default:
		break;
	}
}

int main(void)
{
	/* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x01;
	DDRC = 0xFF; PORTC = 0xFF;
	PORTB = 0x00;
	state =	start;
	while (1)
	{
		tick();
	}
}

