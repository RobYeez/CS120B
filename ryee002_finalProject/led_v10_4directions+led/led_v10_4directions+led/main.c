/*	Partner(s) Name & E-mail: Robert Yee (ryee002)
 *	Lab Section: 022
 *	Assignment: Final Project 
 *	Exercise Description: [optional - include for your own benefit]
 *	Working mutli directional joystick
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

/*
 * led_v5_leftshiftLED.c
 *
 * Created: 3/10/2019 7:57:38 PM
 * Author : Robert Yee
 */ 

#include <avr/io.h>

#include "timer.h"
//joystick code

void adc_init() // call this before while loop to init adc
{
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

void Set_A2D_Pin(unsigned char pinNum)
{
	ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;

	static unsigned char i = 0;
	for(i = 0; i < 15; ++i)
	{
		asm("nop");
	}
}

void convert()
{
	ADCSRA |=(1<<ADSC);//start ADC conversion
	while ( !(ADCSRA & (1<<ADIF)));//wait till ADC conversion
	
}
//shift reg code

unsigned char i = 0;

#define SET_BIT(p,i) (p |= (1 << i))
#define CLR_BIT(p,i) (p &= ~(1 << i))
#define GET_BIT(p,i) (p & (1 << i))
#define SHIFT_REG PORTC

void transmit_data(unsigned char data){
	unsigned char tmp = data;
	
	SET_BIT(SHIFT_REG, 3);				//set SRCLR high
	CLR_BIT(SHIFT_REG, 1);				//set RCLK low
	
	for(i = 0; i < 8; i++){
		CLR_BIT(SHIFT_REG, 2);			//set SRCLK low
		tmp = GET_BIT(data, i);
		if(tmp)
		SET_BIT(SHIFT_REG, 0);	//set SER to data to be transmitted
		else
		CLR_BIT(SHIFT_REG, 0);
		SET_BIT(SHIFT_REG, 2);			//set SRCLK high
	}
	
	SET_BIT(SHIFT_REG, 1);				//set RCLK high
}

unsigned short lr = 0x0000;
unsigned short ud = 0x0000;

enum State {wait, left, right, up, down} state;
unsigned char pattern = 0x80;
unsigned char row = 0x1E;
unsigned char cnt = 0x00;
	
void udlr() {
	switch(state) {
		case wait:
		Set_A2D_Pin(0x00);
		lr = ADC;
		Set_A2D_Pin(0x01);
		ud = ADC;
		if ((lr > 500 && lr < 600) && (ud > 500 && ud < 600) ) { //rest
			state = wait;
		}
		
		else if( lr < 350  && abs(ud - 512) < 250) { //left  -> lower than average and ud is less than average
			state = left;
		}
		
		else if( lr > 850 && abs(ud - 512) < 250 ) { //right -> higher than average and ud is less than average
			state = right;
		}
		
		else if( ud > 850 && abs(lr - 512) < 250 ) { //up -> higher than average and lr is less than average
			state = up;
		}
		
		else if( ud < 350 && abs(lr - 512) < 250 ) { //down -> lower than average and lr is less than average
			state = down;
		}
		break;
		
		case left:
		state = wait;
		break;
		
		case right:
		state = wait;
		break;
		
		case up:
		state = wait;
		break;
		
		case down:
		state = wait;
		break;
	}
	switch(state) {
		case wait:
			break;
		
		case left:
			if(pattern == 0x80) {
				pattern = 0x01;
			}
			else {
				pattern = pattern << 1;
			}
			break;
			
		case right:
			if (pattern == 0x01) {
				pattern = 0x80;
			}
			else {
				pattern = pattern >> 1;
			}
			break;
			
		case up:
			if (cnt > 0) {
				--cnt;
			}
			if (cnt == 0) {
				row = 0x1E;
			}
			else if (cnt == 1) {
				row = 0x1D;
			}
			else if (cnt == 2) {
				row = 0x1B;
			}
			else if (cnt == 3) {
				row = 0x17;
			}
			else if (cnt == 4) {
				row = 0x0F;
			}
			else if (cnt == 5) {
				row = 0x1E;
				cnt = 0;
			}
			break;
			
		case down:
			if (cnt < 5) {
				++cnt;
			}
			if (cnt == 0) {
				row = 0x1E;
			}
			else if (cnt == 1) {
				row = 0x1D;
			}
			else if (cnt == 2) {
				row = 0x1B;
			}
			else if (cnt == 3) {
				row = 0x17;
			}
			else if (cnt == 4) {
				row = 0x0F;
			}
			else if (cnt == 5) {
				row = 0x1E;
				cnt = 0;
			}
			break;
	}
	if((~PINA & 0x04) == 0x04)
	{
		
	}
	
	PORTD = row;
	transmit_data(pattern);
}
/*
enum State2 {wait, buttonOn} state2;
char array[40][40];
void buttonPress() {
	switch(state2) {
		case wait:
		if (buttonOn) {
			state2 = buttonOn;
		}
		else {
			state2 = wait;
		}
		
		case buttonOn:
		if(buttonOn) {
			scanf("%d", &array[pattern][row];
		}
		else {
			state2 = wait;
		}	
	}
	
	PORTD = row;
	transmit_data(pattern)
}
*/
int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00; //led
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    adc_init();
	TimerSet(100);
	TimerOn();
	while (1) 
    {
		while(!TimerFlag);
		TimerFlag = 0;
		udlr();
    }
}

