/*	Partner(s) Name & E-mail: Robert Yee (Ryee002), Stephanie Qian (Sqian003)
 *	Lab Section: 022
 *	Assignment: Lab # 9 Exercise # 2 
 *	Exercise Description: [optional - include for your own benefit]
 *	Decrementing and incrementing frequency sounds
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/*
 * ryee002_sqian003_lab9_part2.c
 *
 * Created: 2/9/2019 8:28:12 PM
 * Author : Robert Yee
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
enum States {init, start, add, sub} state;
	
unsigned char buttonA;
unsigned char buttonUp;
unsigned char buttonDown;
unsigned char fqLevel = 0;

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

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


void sound() {
	buttonA = (~PINA & 0x01);
	buttonUp = (~PINA & 0x02);
	buttonDown = (~PINA & 0x04);
	
	switch(state) 
	{
		case init:
		if (buttonA) {
			state = start;
		}
		else {
			state = init;
		}
		break;
		
		case start:
		if (buttonA) {
			state = init;
		}
		else if (buttonUp) {
			state = add;
		}
		else if (buttonDown) {
			state = sub;
		}
		else {
			state = start;
		}
		break;
		
		case add:
			state = start;
		break;
		
		case sub:
			state = start;
		break;
	}
	
	switch(state)
	{
		case init:
		fqLevel = 0;
		set_PWM(0);
		break;
		
		case start:
		
		break;
		
		case add:
		if (fqLevel < 9) {
			++fqLevel;
			if (fqLevel == 1) {
				set_PWM(261.63);
			}
			else if (fqLevel == 2) {
				set_PWM(293.66);
			}
			else if (fqLevel == 3) {
				set_PWM(329.63);
			}
			else if (fqLevel == 4) {
				set_PWM(349.23);
			}
			else if (fqLevel == 5) {
				set_PWM(392.00);
			}
			else if (fqLevel == 6) {
				set_PWM(440.00);
			}
			else if (fqLevel == 7) {
				set_PWM(493.88);
			}
			else if (fqLevel == 8) {
				set_PWM(523.25);
			}
		}
		break;
		
		case sub:
		if (fqLevel > 0) {
			--fqLevel;
			if (fqLevel == 1) {
				set_PWM(261.63);
			}
			else if (fqLevel == 2) {
				set_PWM(293.66);
			}
			else if (fqLevel == 3) {
				set_PWM(329.63);
			}
			else if (fqLevel == 4) {
				set_PWM(349.23);
			}
			else if (fqLevel == 5) {
				set_PWM(392.00);
			}
			else if (fqLevel == 6) {
				set_PWM(440.00);
			}
			else if (fqLevel == 7) {
				set_PWM(493.88);
			}
			else if (fqLevel == 8) {
				set_PWM(523.25);
			}
		}
		break;
	}
}

int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	PWM_on();
	TimerSet(250);
	TimerOn();
	state = init;
    while (1) 
    {
		while(!TimerFlag);
		sound();
		TimerFlag = 0;
    }
}

