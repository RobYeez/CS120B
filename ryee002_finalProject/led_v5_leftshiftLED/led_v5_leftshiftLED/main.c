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


unsigned short joystick = 0x0000;
unsigned char pattern = 0x80;
unsigned char row = 0x1E;
enum JoystickStates{wait, left, right} state;

void JoyStick_Tick()
{
	
	Set_A2D_Pin(0x01);
	convert();
	joystick = ADC;
	switch(state)
	{
		case wait:
		if(joystick < 350)
		state = left;
		else if(joystick > 700)
		state = right;
		else
		state = wait;
		break;
		case left:
		if(joystick < 350)
		state = left;
		else
		state = wait;
		break;
		case right:
		if(joystick > 700)
		state = right;
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
		case left:
		if(pattern == 0x80) //if all the way to the left
		pattern = 0x01;		//start at right
		else {
		pattern = pattern << 1; //shift left
		}
		break;
		case right:
		if(pattern == 0x01) //if all the way to the left
		pattern = 0x80;		//sart at right
		else {
		pattern = pattern >> 1;  //shift right
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

