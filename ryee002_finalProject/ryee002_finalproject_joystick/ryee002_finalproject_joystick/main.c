/* Partner(s) Name & E-mail:
Justin Cano (Jcano001@ucr.edu) & Winrich Sy (wsy001@ucr.edu)

* Lab Section: 22

* Assignment: Lab #6 Exercise #2

* Exercise Description: [optional - include for your own benefit]

*

* I acknowledge all content contained herein, excluding template or example

* code, is my own original work.

*/
// The following is sample code for how to create a main.c that works with scheduler.h
// This program will cause D0 to blink on and off every 1500ms. D4D5D6 will rotate every 500ms.
// This code will work with the ATMega1284 or ATMega32

#include <avr/io.h>
#include <ucr/scheduler.h>

void A2D_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant
	//          analog to digital conversions.
}

// Pins on PORTA are used as input for A2D conversion
// The value of pinNum determines the pin on PORTA
//    used for A2D conversion
// Valid values range between 0 and 7, where the value
//    represents the desired pin for A2D conversion
void Set_A2D_Pin(unsigned char pinNum) {
	ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
}

unsigned char pattern = 0x80;// LED pattern - 0: LED off; 1: LED on
unsigned char row = ~0x01;// Row(s) displaying pattern.
// 0: display pattern on row
// 1: do NOT display pattern on row

unsigned char GetBit(unsigned char x, unsigned char k)
{
	return ((x & (0x01 << k)) != 0);
}

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b)
{
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned short V_LR;
enum readLR_States {LR,left,right};
int readLR_Tick(int state) {
	Set_A2D_Pin(1);
	unsigned int i = 0;
	V_LR = ADC;
	switch(state) {
		case LR:
		V_LR = ADC;
		if( V_LR < 400 )
		state = left;
		else if ( V_LR > 600 )
		state = right;
		else
		state = LR;
		break;
		case right:
		V_LR = ADC;
		if( V_LR >= 600 && V_LR < 750 )
		tasks[1].period = 1000;
		else if( V_LR >= 750 && V_LR < 850 )
		tasks[1].period = 500;
		else if( V_LR >= 850 && V_LR < 950 )
		tasks[1].period = 250;
		else
		tasks[1].period = 100;
		
		if( V_LR > 600 )
		state = right;
		else
		state = LR;
		break;
		case left:
		V_LR = ADC;
		if( V_LR <= 400 && V_LR > 350 )
		tasks[1].period = 1000;
		else if( V_LR <= 350 && V_LR > 250 )
		tasks[1].period = 500;
		else if( V_LR <= 250 && V_LR > 150 )
		tasks[1].period = 250;
		else
		tasks[1].period = 100;
		if( V_LR < 400 )
		state = left;
		else
		state = LR;
		break;
		default:
		state = LR;
		break;
	}

	switch(state) {
		case LR:
		break;
		case left:
		if( pattern == 0x80 )
		break;
		pattern = pattern << 1;
		break;
		case right:
		if( pattern == 0x01 )
		break;
		pattern = pattern >> 1;
		break;
		default:
		break;
	}

	return state;
}

unsigned short V_UD;
enum readUD_States {UD,up,down};
int readUD_Tick(int state) {
	Set_A2D_Pin(0);
	unsigned int i = 0;
	V_UD = ADC;
	//if(!GetBit(PORTD,4) && !GetBit(PORTD,0))
	//row = ~0x01;
	switch(state) {
		case UD:
		V_UD = ADC;
		if( V_UD < 400 )
		state = down;
		else if ( V_UD > 600 )
		state = up;
		else
		state = UD;
		break;
		case up:
		V_UD = ADC;
		
		if( V_UD >= 600 && V_UD < 750 )
		tasks[2].period = 1000;
		else if( V_UD >= 750 && V_UD < 850 )
		tasks[2].period = 500;
		else if( V_UD >= 850 && V_UD < 950 )
		tasks[2].period = 250;
		else
		tasks[2].period = 100;
		
		if( V_UD > 600 )
		state = up;
		else
		state = UD;
		break;
		case down:
		V_UD = ADC;
		
		if( V_UD <= 400 && V_UD > 350 )
		tasks[2].period = 1000;
		else if( V_UD <= 350 && V_UD > 250 )
		tasks[2].period = 500;
		else if( V_UD <= 250 && V_UD > 150 )
		tasks[2].period = 250;
		else
		tasks[2].period = 100;
		
		if( V_UD < 400 )
		state = down;
		else
		state = UD;
		break;
		default:
		state = UD;
		break;
	}

	switch(state) {
		case UD:
		break;
		case up:
		if( !GetBit(PORTD,0) )
		break;
		row = ~(~row >> 1) | 0xE0;
		break;
		case down:
		if( !GetBit(PORTD,4) )
		break;
		row = ~(~row << 1);
		break;
		default:
		break;
	}

	return state;
}

//--------------------------------------
// LED Matrix Demo SynchSM
// Period: 100 ms
//--------------------------------------
enum Demo_States {init,s1};
int Demo_Tick(int state) {
	// Local Variables
	static unsigned char p,r;
	// Transitions
	switch (state) {
		case init:
		state = s1;
		break;
		case s1:
		state = s1;
		break;
		default:
		state = init;
		break;
	}

	// Actions
	switch (state) {
		case init:
		break;
		case s1:
		p = pattern;
		r = row;
		break;
		default:
		break;
	}

	PORTC = p; // Pattern to display
	PORTD = r; // Row(s) displaying pattern

	return state;
}



int main(void) {
	A2D_init();

	// initialize ports
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	tasksNum = 3; // declare number of tasks
	task tsks[3]; // initialize the task array
	tasks = tsks; // set the task array

	// define tasks
	unsigned char i=0; // task counter
	tasks[i].state = -1;
	tasks[i].period = 1;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &Demo_Tick;
	i++;

	tasks[i].state = -1;
	tasks[i].period = 150;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &readLR_Tick;
	i++;
	
	tasks[i].state = -1;
	tasks[i].period = 150;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &readUD_Tick;
	
	TimerSet(1); // value set should be GCD of all tasks
	TimerOn();
	
	while(1) {} // task scheduler will be called by the hardware interrupt

}