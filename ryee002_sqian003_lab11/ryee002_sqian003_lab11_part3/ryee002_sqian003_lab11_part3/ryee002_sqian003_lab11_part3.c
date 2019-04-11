/*	Partner(s) Name & E-mail: Robert Yee (ryee002), Stephanie Qian (Sqian003)
 *	Lab Section: 022
 *	Assignment: Lab # 11 Exercise # 3 
 *	Exercise Description: [optional - include for your own benefit]
 *	Outputing keypad # on LCD
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/*
 * ryee002_sqian003_lab11_part3.c
 *
 * Created: 2/24/2019 4:03:50 PM
 * Author : Robert Yee
 */ 

#include <avr/io.h>
#include <ucr/keypad.h>
#include "io.c"

int main(void)
{
    /* Replace with your application code */
	unsigned char x;
	DDRB = 0xF0; PORTB = 0x0F; 
	DDRC = 0xFF; PORTC = 0x00; //LCD
	DDRD = 0xFF; PORTD = 0x00; //LCD
	LCD_init();
	LCD_DisplayString(1, "Hello");
//	LCD_ClearScreen();
    while (1) 
    {
		x = GetKeypadKey(); 
		switch(x) {
			case '\0': break;
			case '1': LCD_DisplayString(1, "1"); break;
			case '2': LCD_DisplayString(1, "2"); break;
			case '3': LCD_DisplayString(1, "3"); break;
			case '4': LCD_DisplayString(1, "4"); break;
			case '5': LCD_DisplayString(1, "5"); break;
			case '6': LCD_DisplayString(1, "6"); break;
			case '7': LCD_DisplayString(1, "7"); break;
			case '8': LCD_DisplayString(1, "8"); break;
			case '9': LCD_DisplayString(1, "9"); break;
			case 'A': LCD_DisplayString(1, "A"); break;
			case 'B': LCD_DisplayString(1, "B"); break;
			case 'C': LCD_DisplayString(1, "C"); break;
			case 'D': LCD_DisplayString(1, "D"); break;
			case '*': LCD_DisplayString(1, "*"); break;
			case '0': LCD_DisplayString(1, "0"); break;
			case '#': LCD_DisplayString(1, "#"); break;
		}
    }
}

