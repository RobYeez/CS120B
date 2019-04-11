/*	Partner(s) Name & E-mail: Robert Yee (ryee002), Stephanie Qian (sqian003)
 *	Lab Section: 022
 *	Assignment: Lab # 9 Exercise # 1 
 *	Exercise Description: [optional - include for your own benefit]
 *	Displaying sound
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/*
 * ryee002_sqian003_lab9_part1.c
 *
 * Created: 2/7/2019 1:04:46 PM
 * Author : Robert Yee
 */ 

#include <avr/io.h>
enum States {init, start, on, off} state;
unsigned char buttonA;
unsigned char buttonB;
unsigned char buttonC;
// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << COM0A0) | (1 << WGM00);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}

void sound() {
	buttonA = (~PINA & 0x01);
	buttonB = (~PINA & 0x02);
	buttonC = (~PINA & 0x04);
	switch(state){
		case init:
		state = start;
		break;
	
		case start:
		if (buttonA && ~buttonB & ~buttonC) {
			state = on;
		}
		else if (~buttonA && buttonB & ~buttonC) {
			state = on;
		}
		else if (~buttonA && ~buttonB & buttonC) {
			state = on;
		}
		else {
			state = start;
		}	
		break;
	
		case on: //transition back to start after reciving input
		if (buttonA && ~buttonB & ~buttonC) {
			state = on;
		}
		else if (~buttonA && buttonB & ~buttonC) {
			state = on;
		}
		else if (~buttonA && ~buttonB & buttonC) {
			state = on;
		}
		else {
			state = off;
		}
		break;
	
		case off: //same as on
		state = start;
		break;
	}
	
	switch(state) {
		case init:
		set_PWM(0);
		break;
		
		case start:
		set_PWM(0);
		break;
		
		case on:
		if (buttonA && ~buttonB & ~buttonC) {
			set_PWM(261.63);
		}
		else if (~buttonA && buttonB & ~buttonC) {
			set_PWM(293.66);
		}
		else if (~buttonA && ~buttonB & buttonC) {
			set_PWM(329.63);
		}
		break;
		
		case off:
		set_PWM(0);
		break;
	}
	
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; //Input
	DDRB = 0xFF; PORTB = 0x00; //Output
    /* Replace with your application code */
    PWM_on();
	state = init;
	while (1) 
    {
		sound();
    }
	//PWM_off();
}

