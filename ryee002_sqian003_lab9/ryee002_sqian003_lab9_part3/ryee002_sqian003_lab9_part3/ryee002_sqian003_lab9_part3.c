/*	Partner(s) Name & E-mail: Robert Yee (ryee002), Stephanie Qian (sqian003)
 *	Lab Section: 022
 *	Assignment: Lab # 9  Exercise # 3 
 *	Exercise Description: [optional - include for your own benefit]
 *	Challenge with 3 arrays? Making a short melody
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/*
 * ryee002_sqian003_lab9_part3.c
 *
 * Created: 2/9/2019 9:20:56 PM
 * Author : Robert Yee
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
void TimerOn()
{
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}
void TimerOff()
{
	TCCR1B = 0x00;
}
void TimerISR()
{
	TimerFlag = 1;
}
ISR(TIMER1_COMPA_vect)
{
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0)
	{
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
void TimerSet(unsigned long M)
{
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

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
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}


enum States {init, start, play, count, pause, wait}state;
	//cc gg aa g | ff ee dd c | gg ff ee d | gg ff ee d | cc gg aa g |ff ee dd c
long music[42] = {523.25, 523.25, 392.00, 392.00, 440.00, 440.00, 392.00, 349.23, 349.23, 329.63, 329.63, 293.66, 293.63, 261.63, 
	392.00, 392.00, 349.23, 349.23, 329.63, 329.63, 293.66, 392.00, 392.00, 349.23, 349.23, 329.63, 329.63, 293.66, 
	523.25, 523.25, 392.00, 392.00, 440.00, 440.00, 392.00,349.23, 349.23, 329.63, 329.63, 293.66, 293.63, 261.63};
long time[42] = {50, 50, 50, 50, 50, 50, 25, 50, 50, 50, 50, 50, 50, 25, 50, 50, 50, 50, 50, 50, 25, 50, 50, 50, 50, 50, 50, 25, 50, 50, 50, 50, 50, 50, 25};
unsigned char i = 0x00;
unsigned char m = 0x00;
unsigned char n = 0x00;

void sound()
{
	switch(state) //transitions
	{
		case init:
		{
			state = start;
			break;
		}
		case start:
		{
			if((~PINA & 0x01) == 0x01)
			{
				state = play; 
				break;
			}
			else
			{
				state = start; 
				break;
			}
		}
		
		case play:
		{
			if(i <= time[m])
			{
				state = play; 
				break;
			}
			else if(i > time[m])
			{
				++n;
				state = count;
				break;
			}
		}
		case count:
		{
			if(n < 30)
			{
				++m;
				state = play;
				break;
			}
			else if(n >= 30)
			{
				state = pause; 
				break;
			}
		}
		
		case pause:
		{
			if((~PINA & 0x01) == 0x01)
			{
				state = wait; 
				break;
			}
			else
			{
				state = pause;
				break;
			}
		}
		case wait:
		{
			if((~PINA & 0x01) == 0x01)
			{
				state = wait;
				break;
			}
			else
			{
				state = start;
				break;
			}
		}
		
	}
	switch(state) //state actions
	{
		case init:
		break;
		
		case start:
		{
			i = 0;
			m = 0;
			n = 0;
			break;
		}
		
		case play:
		{
			set_PWM(music[m]);
			++i;
			break;
		}
		
		case count:
		{
			set_PWM(0);
			break;
		}
		
		case pause:
		break;
		
		case wait:
		break;
	}
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	PWM_on();
	
	TimerSet(50);
	TimerOn();
	state = init;
	while(1)
	{
		sound();
		while (!TimerFlag);
		TimerFlag = 0;
	}
}
