/*	Partner(s) Name & E-mail: Robert Yee (ryee002) and Stephanie Qian (Sqian003)
 *	Lab Section: 022
 *	Assignment: Lab # 7 Exercise # 1 
 *	Exercise Description: [optional - include for your own benefit]
 *	Programming led increments and decrements
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/*
 * ryee002_sqian003_lab7_part1.c
 *
 * Created: 1/31/2019 1:00:20 PM
 * Author : Robert Yee
 */ 
#include <avr/io.h>
#include "io.c"

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	TCCR1B = 0x0B;
	// AVR output compare register OCR1A.
	OCR1A = 125;	
	TIMSK1 = 0x02; 
	//Initialize avr counter
	TCNT1=0;
	_avr_timer_cntcurr = _avr_timer_M;
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


enum States {init, wait, add, sub, reset} state;
unsigned char count;
unsigned char I;
unsigned char D;
void tick() {
	I = (~PINA) & 0x01;
	D = (~PINA) & 0x02;
	switch(state) {
		case init:
		state = wait;
		break;
		
		case wait:
		if(I && D) {
			state = reset;
		}
		else if (I && !D && (count < 9)) {
			state = add;
		}
		else if (!I && D && (count > 0)) {
			state = sub;
		}
		else {
			state = wait;
		}
		break;
		
		case add: 
		state = wait;
		break;
		
		case sub:
		state = wait;
		break;
		
		case reset:
		state = wait;
		break;
	}
	switch(state) {
		case init:
		count = 0;
		LCD_Cursor(1);
		//LCD_WriteData(count + '0');
		break;
		
		case wait:
		break;
		
		case add:
		count = count + 1;
		LCD_Cursor(1);
		LCD_WriteData (count + '0');
		break;
		
		case sub:
		count = count - 1;
		LCD_Cursor(1);
		LCD_WriteData(count + '0');
		break;
		
		case reset:
		count = 0;
		LCD_Cursor(1);
		LCD_WriteData(count + '0');
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; //2 bit inputs
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	TimerSet(200);
	TimerOn();
	state = init;
	// Initializes the LCD display
	LCD_init();
	LCD_Cursor(1);
	count = 0;
	LCD_WriteData(count + '0');
	
	while(1) {
		tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	
}
