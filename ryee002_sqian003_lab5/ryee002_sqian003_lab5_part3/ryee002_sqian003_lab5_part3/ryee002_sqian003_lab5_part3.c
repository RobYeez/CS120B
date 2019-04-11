/*	Partner(s) Name & E-mail: Robert Yee (Ryee002), Stephanie Qian (Sqian003)
 *	Lab Section: 022
 *	Assignment: Lab # 5 Exercise #  
 *	Exercise Description: [optional - include for your own benefit]
 *	Custom light blink thing
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/*
 * ryee002_sqian003_lab5_part3.c
 *
 * Created: 1/27/2019 5:31:37 PM
 * Author : Robert Yee
 */ 

#include <avr/io.h>
enum States {start, wait, ready, side2, side4, side6, none, all} state;
unsigned char blinkCnt = 0x00;
unsigned char tempA;
unsigned char sideCnt = 0x00;

void Tick() {
	tempA = ~PINA;
	switch(state) {
		case start:		//shift onto wait/start of everything
		state = wait;
		break;
		
		case wait:
		if (tempA & 0x1) { //you have to wait for button to be released before it can continue
			state = wait;
		}
		else {
			state = ready;
		}
		break;
		
		case ready:		//determine which case it will go to
		if (tempA & 0x1) {
			if (sideCnt == 0) {
				++sideCnt;
				state = side2;
			}
			else if (sideCnt == 1) {
				++sideCnt;
				state = side4;
			}
			else if (sideCnt == 2) {
				++sideCnt;
				state = side6;
			}
			else if (sideCnt == 3){
				if (blinkCnt != 3) {
					++blinkCnt;
					state = all;
				}
				else {
					state = start;
				}
			}
		}
		else {
			state = ready;
		}
		break;
		
		case side2:
		state = wait;
		break;
		
		case side4:
		state = wait;
		break;

		case side6:
		state = wait;
		break;
		
		case none:
		state = wait;
		break;
		
		case all:
		if (blinkCnt != 2) {
			state = none;
		}
		else {
			state = wait;
		}
		break;
	}
	
	switch(state) {
		case start:
		blinkCnt = 0;
		sideCnt = 0;
		break;
		
		case wait:
		break;
		
		case ready:
		break;
		
		case side2:
		PORTC = 0x21;
		break;
		
		case side4:
		PORTC = 0x33;
		break;
		
		case side6:
		PORTC = 0x3F;
		break;
		
		case none:
		PORTC = 0x00;
		break;
		
		case all:
		PORTC = 0x3F;
		break;
	}
}

int main(void)
{
	/* Replace with your application code */
	DDRA = 0x0; PORTA = 0xF;
	DDRC = 0xFF; PORTC = 0x00;
	while (1)
	{
		Tick();
	}
}
