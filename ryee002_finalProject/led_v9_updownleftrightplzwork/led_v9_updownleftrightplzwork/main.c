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

enum State {wait} state;
	
void udlr() {
	switch(state) {
		case wait:
		//PORTB = 0x00;
		Set_A2D_Pin(0x00);
		//convert();
		lr = ADC;
		Set_A2D_Pin(0x01);
		//convert();
		ud = ADC;
		if ((lr > 500 && lr < 600) && (ud > 500 && ud < 600) ) { //rest
			PORTB = 0xFF;
			state = wait;
		}
		
		else if( lr < 350  && abs(ud - 512) < 250) { //left  -> lower than average and ud is less than average
			PORTB = 0x02;
			state = wait;
		}
		
		else if( lr > 850 && abs(ud - 512) < 250 ) { //right -> higher than average and ud is less than average
			PORTB = 0x04;
			state = wait;
		}
		
		else if( ud > 850 && abs(lr - 512) < 250 ) { //up -> higher than average and lr is less than average
			PORTB = 0x01;
			state = wait;
		}
		
		else if( ud < 350 && abs(lr - 512) < 250 ) { //down
			PORTB = 0x08;
			state = wait;
		}
		
	}
}


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

