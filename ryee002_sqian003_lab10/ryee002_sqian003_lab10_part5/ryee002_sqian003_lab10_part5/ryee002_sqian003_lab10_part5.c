/*
 * ryee002_sqian003_lab10_part5.c
 *
 * Created: 2/19/2019 9:02:14 AM
 * Author : Robert Yee
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

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

#define button0 (~PINA & 0x01) // button for increment
#define button1 (~PINA & 0x02) // button for decrement

enum States {START, WAIT, INC, DEC, RESET} state;
enum Set_B_States {SetB_Start, Set} B_state;

unsigned char tempB = 0x00;
unsigned long current = 0x00;
unsigned long period = 0x00; // variable to set period

void Button_Hold() {
	if(period == 1000 && current >= 3000)
	period = 400;
	else if(period == 1 && current >= 1000)
	period = 1000;
	if (period == 400 || period == 1000)
	{
		if(state == INC && tempB < 9)
		++tempB;
		else if(state == DEC && tempB > 0)
		--tempB;
	}
}

void Counter() {
	switch(state)	{ // Transition actions
		case START:
		state = WAIT;
		break;
		case WAIT:
		if(button0 && !button1) {
			state = INC;
			} else if(!button0 && button1) {
			state = DEC;
			} else if(button0 && button1) {
			state = RESET;
			} else {
			state = WAIT;
		}
		break;
		case INC:
		if(button0 && !button1)	{
			state = INC;
			Button_Hold();
			} else if(button0 && button1) {
			state = RESET;
			} else {
			state = WAIT;
			if(tempB < 9) {
				++tempB;
			}
		}
		break;
		case DEC:
		if(!button0 && button1) {
			state = DEC;
			Button_Hold();
			} else if(button0 && button1) {
			state = RESET;
			} else {
			state = WAIT;
			if(tempB > 0) {
				--tempB;
			}
		}
		break;
		case RESET:
		if(!button0 && !button1) {
			state = WAIT;
			} else {
			state = RESET;
		}
		break;
		default:
		break;
	}
	
	switch(state) { // State actions
		case START:
		break;
		case WAIT:
		current = 0;
		period = 1;
		break;
		case INC:
		break;
		case DEC:
		break;
		case RESET:
		tempB = 0;
		break;
		default:
		break;
	}
}

void SetB() {
	switch(B_state){ // Transition actions
		case SetB_Start:
		B_state = Set;
		break;
		case Set:
		B_state = Set;
		break;
	}
	
	switch(B_state){
		case SetB_Start: // State actions
		PORTB = 0x00;
		break;
		case Set:
		PORTB = tempB;
		break;
		default:
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure A's 8 pins as inputs, initialize to 1's
	DDRB = 0xFF; PORTB = 0x00; // Configure B's 8 pins as outputs, initialize to 0's
	
	unsigned long C_elapsedTime = 1;

	const unsigned long timerPeriod = 1; // initialize timerPeriod to 1ms
	
	TimerSet(timerPeriod);
	TimerOn();
	
	state = START;
	B_state = SetB_Start;
	period = timerPeriod;

	while(1) {
		if (C_elapsedTime >= period) {
			Counter();
			C_elapsedTime = 0;
		}
		SetB();
		while (!TimerFlag){}   // Wait for timer period
		TimerFlag = 0;         // Lower flag raised by timer
		C_elapsedTime += timerPeriod;
		current += timerPeriod;
	}
}

