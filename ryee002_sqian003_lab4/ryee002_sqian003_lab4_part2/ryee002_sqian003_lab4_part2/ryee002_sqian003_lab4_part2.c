/*	Partner(s) Name & E-mail: Robert Yee (Ryee002), Stephanie Qian (Sqian003)
 *	Lab Section: 022
 *	Assignment: Lab # 4 Exercise # 2 
 *	Exercise Description: [optional - include for your own benefit]
 *	Incrementing total for states
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/*
 * ryee002_sqian003_lab4_part2.c
 *
 * Created: 1/20/2019 10:11:43 PM
 * Author : Robert Yee
 */ 

#include <avr/io.h>
enum State {init, stateI, stateD, stateNone} state;
unsigned char counterTotal = 0x00;
unsigned char tempA;

void tick() {
		tempA = (~PINA & 0x03);
	switch (state) {
		case init: //initalize it to increment state
		if((tempA) == 0x01) { 
			state = stateI;
			break;
		}
		else if ((tempA) == 0x02) {
			state = stateD;
			break;
		}
		else if ((tempA) == 0x03) {
			state = stateNone;
			break;
		}
		state =init;
		break;
		
		case stateI:
		if((tempA) == 0x01) { 
			state = stateI;
			break;
		}
		else if ((tempA) == 0x02) {
			state = stateD;
			break;
		}
		else if ((tempA) == 0x03) {
			state = stateNone;
			break;
		}
		state =init;
		break;
		
		case stateD:
		if((tempA) == 0x01) { 
			state = stateI;
			break;
		}
		else if ((tempA) == 0x02) {
			state = stateD;
			break;
		}
		else if ((tempA) == 0x03) {
			state = stateNone;
			break;
		}
		state =init;
		break;
				
		case stateNone:
		if((tempA) == 0x01) { 
			state = stateI;
			break;
		}
		else if ((tempA) == 0x02) {
			state = stateD;
			break;
		}
		else if ((tempA) == 0x03) {
			state = stateNone;
			break;
		}
		state =init;
		break;
		
		
		}
		switch (state) {
		case init:
		break;
		
		case stateI:
		if(counterTotal != 0x09) {
			++counterTotal;
		}
		PORTC = counterTotal;
		break;
		
		case stateD:
		if (counterTotal != 0x00) {
			--counterTotal;
		}
		PORTC = counterTotal;
		break;
		
		case stateNone:
		counterTotal = 0x00;
		PORTC = counterTotal;
		break;
	}
	
}

int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF; // 8 bits but only need 2
	DDRC = 0xFF; PORTC = 0x00; // 8 bits but only need 5(get 9)
	counterTotal = 0x07;
	PORTC = counterTotal;
    while (1) 
    {
		tick();
    }
}

