/*	Partner(s) Name & E-mail: Robert Yee (ryee002), Stephanie Qian (sqian003)
 *	Lab Section: 022
 *	Assignment: Lab # 10  Exercise # 4 
 *	Exercise Description: [optional - include for your own benefit]
 *	Led + frequency change
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/*
 * ryee002_sqian003_lab10_part1.c
 *
 * Created: 2/17/2019 6:27:51 PM
 * Author : Robert Yee
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char combined = 0;

enum BL_states{BL_start, BL_off, BL_on} BL_state;
void Tick_BL();
static unsigned char blink = 0;

enum Three_states{Three_start, Three_1, Three_2, Three_3} Three_state;
void Tick_Three();
static unsigned char three = 0;

enum Sound_states{sound_start, sound_off, sound_add, sound_sub, sound_play} Sound_state;
void Tick_Sound();
unsigned char buttonAdd = 0x00;
unsigned char buttonSub = 0x00;
unsigned char buttonPlay = 0x00;
static unsigned long soundPlay = 0;
unsigned char intCnt = 0;

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


int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF; //input 
	DDRB = 0xFF; PORTB = 0x00; //output
	
	unsigned long timerBL = 1000;
	unsigned long timerThree = 300;
	const unsigned long timeSet = 100;
	
	TimerSet(timeSet);
	TimerOn();
	PWM_on();
	
	BL_state = BL_start;
	Three_state = Three_start;
	Sound_state = sound_start;
    while (1) 
    {
		if (timerBL >= 1000 ) {
			Tick_BL();
			timerBL = 0;
		}
		if (timerThree >= 300) {
			Tick_Three();
			timerThree = 0;
		}
		Tick_Sound();
		combined = blink + three;
		PORTB = combined;
		while(!TimerFlag){}; 
		TimerFlag = 0;
		timerBL += timeSet;
		timerThree += timeSet;
    }
}

void Tick_BL() {
	switch(BL_state) {
		case BL_start:
		BL_state = BL_off;
		break;
		
		case BL_off:
		BL_state = BL_on;
		break;
		
		case BL_on:
		BL_state = BL_off;
		break;
		
		default:
		BL_state = BL_start;
		break;
	}	
	switch(BL_state) {
		case BL_start:
		blink = 0x00;
		//PORTB = blink;
		break;
		
		case BL_off:
		blink = 0x00;
		//PORTB = blink;
		break;
		
		case BL_on:
		blink = 0x10;
		//PORTB = blink;
		break;
		
		default:
		blink = 0x00;
		//PORTB = blink;
		break;
	}
}

void Tick_Three() {
	switch(Three_state) {
		case Three_start:
		Three_state = Three_1;
		break;
		
		case Three_1:
		Three_state = Three_2;
		break;
		
		case Three_2:
		Three_state = Three_3;
		break;
		
		case Three_3:
		Three_state = Three_1;
		break;
		
		default:
		Three_state = Three_1;
		break;
	}
	switch(Three_state) {
		case Three_start:
		three = 0x00;
		//PORTB = three;
		break;
		
		case Three_1:
		three = 0x01;
		//PORTB = three;
		break;
		
		case Three_2:
		three = 0x02;
		//PORTB = three;
		break;
		
		case Three_3:
		three = 0x04;
		//PORTB = three;
		break;
		
		default:
		three = 0x00;
		//PORTB = three;
		break;
	}
}

void Tick_Sound (){
	buttonPlay = ~PINA & 0x01;
	buttonAdd = ~PINA & 0x02;
	buttonSub = ~PINA & 0x04;
	switch(Sound_state) {
		case sound_start:
		if(buttonAdd) {
			Sound_state = sound_add;			
		}
		else if(buttonSub) {
			Sound_state = sound_sub;
		}
		else if (buttonPlay) {
			Sound_state = sound_play;
		}
		else {
			Sound_state = sound_off;
		}
		break;
		
		case sound_off:
		Sound_state = sound_start;
		break;
		
		case sound_add:
		Sound_state = sound_start;
		break;
		
		case sound_sub:
		Sound_state = sound_start;
		break;
		
		case sound_play:
		Sound_state = sound_start;
		break;
		
		default:
		Sound_state = sound_start;
		break;
	}	
	switch(Sound_state) {
		case sound_start:
		break;
		
		case sound_off:
		soundPlay = 0;
		set_PWM(soundPlay);
		break;
		
		case sound_add:
		if (intCnt < 9) {
			++intCnt;
		}
		break;
		
		case sound_sub:
		if (intCnt > 0) {
			--intCnt;
		}
		break;
		
		case sound_play:
		if (intCnt == 1) {
			set_PWM(261.63);
		}
		else if (intCnt == 2) {
			set_PWM(293.66);
		}
		else if (intCnt == 3) {
			set_PWM(329.63);
		}
		else if (intCnt == 4) {
			set_PWM(349.23);
		}
		else if (intCnt == 5) {
			set_PWM(392.00);
		}
		else if (intCnt == 6) {
			set_PWM(440.00);
		}
		else if (intCnt == 7) {
			set_PWM(493.88);
		}
		else if (intCnt == 8) {
			set_PWM(523.25);
		}
		break;
		
		default:
		soundPlay = 0;
		set_PWM(soundPlay);
		break;
	}
}