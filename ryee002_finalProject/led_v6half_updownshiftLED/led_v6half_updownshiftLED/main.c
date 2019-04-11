/*
 * led_v6half_updownshiftLED.c
 *
 * Created: 3/10/2019 9:30:16 PM
 * Author : Robert Yee
 */ 

#include <avr/io.h>
#include "timer.h"
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
unsigned short temp = 0x0000;
unsigned char pattern = 0x80;
unsigned char row = 0xFE;

enum LRStates{leftRight, upDown} state;


void LR_Tick()
{
	switch(state)
	{
		case leftRight:
		Set_A2D_Pin(0x00);
		joystick = ADC;
		if(joystick < 350)
		{
			if(pattern != 0x01)
			pattern = pattern >> 1;
		}
		else if(joystick > 700)
		{
			if(pattern != 0x80)
			pattern = pattern << 1;
		}
		state = upDown;
		break;
		case upDown:
		Set_A2D_Pin(0x02);
		temp = ADC;
		if(temp < 350)
		{
			if(row != 0x10)
			row = row << 1;
		}
		else if(temp > 700)
		{
			if(row != 0x01)
			row = row >> 1;
		}
		state = leftRight;
		break;
		default:
		break;
	}
	PORTD = pattern;	
	transmit_data(~row);
}
int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	adc_init();
    while (1) 
    {
		while(!TimerFlag);
		TimerFlag = 0;
		LR_Tick();
    }
}

