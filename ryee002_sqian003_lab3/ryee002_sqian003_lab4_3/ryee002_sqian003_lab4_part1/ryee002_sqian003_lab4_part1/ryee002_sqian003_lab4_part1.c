/*	Partner(s) Name & E-mail:Robert Yee (Ryee002), Stephanie Qian (Sqian003)
 *	Lab Section: 022			
 *	Assignment: Lab # 4 Exercise # 1 
 *	Exercise Description: [optional - include for your own benefit]
 *	State machine problem 1 PB0 <-> PB1 Triggered by PA0
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/*
 * ryee002_sqian003_lab4_part1.c
 *
 * Created: 1/17/2019 12:20:42 PM
 * Author : Robert Yee
 */ 
/*
PB0 and PB1 each connect to an LED, and PB0's LED is initially on. 
Pressing a button connected to PA0 turns off PB0's LED and turns on PB1's LED, staying that way after button release. 
Pressing the button again turns off PB1's LED and turns on PB0's LED. 
*/

#include <avr/io.h>
enum States {start, s1, s2} state;

void Tick() {
	switch (state) { //transitions
		case start:
			state = s1;
			break;
		case s1:
			if((PINA & 0x01) == 0x01) {
				state = s2;
			}
			else {
				state = s1;
			}
			break;
		case s2:
			if((PINA & 0x01) == 0x01) {
				state = s1;
			}
			else {
				state = s2;
			}
			break;
		default:
			state = s1;
			break;
	}
	switch (state) { //state actions
		case start:
			break;
		case s1:
			PORTB = 0x01;
			break;
		case s2:
			PORTB = 0x02;
			break;
		default:
			PORTB = 0x01;
			break; 
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0x01;	//1 bit inputs
	DDRB = 0x02; PORTB = 0x02;  //2 bit outputs
	state = start;
	while(1) {
		Tick();
	}    
	
}

