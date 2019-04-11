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


unsigned short joystick = 0x0000;
unsigned char pattern = 0x80;
unsigned char row = 0xFE;
enum JoystickStates{wait, up, down} state;

void JoyStick_Tick()
{
	//convert();
	Set_A2D_Pin(0x02);
	joystick = ADC;
	switch(state)
	{
		case wait:
		if(joystick < 350)
		state = up;
		else if(joystick > 700)
		state = down;
		else
		state = wait;
		break;
		
		case up:
		if(joystick < 350)
		state = up;
		else
		state = wait;
		break;
		
		case down:
		if(joystick > 700)
		state = down;
		else
		state = wait;
		break;
		
		default:
		break;
	}
	switch(state)
	{
		case wait:
		break;
		case up:
		//if(row == 0x10) //if all the way to the  top
		//row = 0x01;		//start at bottom
		//else {
			row = row << 1 | 0x01;
			//if (row == 0x02)
			//row = 0x01; //hardcode shifting
			//else if (row == 0x04)
			//row = 0x02;
			//else if (row == 0x08)
			//row = 0x04;
			//else if (row == 0x10)
			//row = 0x08;
		//}
		
		break;
		
		case down:
		if(row == 0x01) //if all the way to the bottom
		row = 0xFE;		//start at top
		else {
		row = row >> 1;
			//if (row == 0x08)
			//row = 0x01;
			//else if (row == 0x04)
			//row = 0x08;
			//else if (row == 0x02)
			//row = 0x04;
			//else if (row == 0x01)
			//row = 0x02;
			//else if (row == 0x10)
			//row = 0x01;
		}
		break;
	}
	PORTD = row;
	transmit_data(pattern);
}

int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    adc_init();
	TimerSet(100);
	TimerOn();
	while (1) 
    {
		while(!TimerFlag);
		TimerFlag = 0;
		JoyStick_Tick();
    }
}

