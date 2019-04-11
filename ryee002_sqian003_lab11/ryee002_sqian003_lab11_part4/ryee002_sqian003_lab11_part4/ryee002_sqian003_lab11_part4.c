/*	Partner(s) Name & E-mail: Robert Yee (ryee002), Stephanie Qian (Sqian003)
 *	Lab Section: 022
 *	Assignment: Lab # 11 Exercise # 4 
 *	Exercise Description: [optional - include for your own benefit]
 *	Overwriting characters
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
/*
 * ryee002_sqian003_lab11_part4.c
 *
 * Created: 2/24/2019 7:14:32 PM
 * Author : Robert Yee
 */ 

#include <avr/io.h>
#include <ucr/keypad.h>
#include "io.c"
#include "timer.h"

enum States{start, output} state;
void input();	
unsigned char cursor;
unsigned char x;

int main(void)
{
    /* Replace with your application code */
	DDRB = 0xF0; PORTB = 0x0F;
	DDRC = 0xFF; PORTC = 0x00; //LCD
	DDRD = 0xFF; PORTD = 0x00; //LCD
	
	TimerOn();
	TimerSet(500);
	
	LCD_init();
	LCD_DisplayString(1, "Kill Me Please");
	
	state = start;
	cursor = 1;
	LCD_Cursor(cursor);
    while (1) 
    {
				while(!TimerFlag) {};
				TimerFlag = 0;
				input();
    }
}

void input() {
	switch(state) {
		case start:
		state = output;
		break;
		
		case output:
		x = GetKeypadKey();
		if (x != '\0') {
			LCD_WriteData(x);
			if(cursor == 17) {
				cursor = 1;
			}
			LCD_Cursor(cursor);
			++cursor;
		}
		state = output;
		break;	
	}
	/*
	switch(state) {
		case start:
		break;
		
		case output:
		break;
	}
	*/
}

