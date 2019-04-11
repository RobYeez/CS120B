/*
 * joystickv2_baileyhelp.c
 *
 * Created: 3/10/2019 3:25:45 PM
 * Author : Robert Yee
 */ 
#include <avr/io.h>
#include "io.c"

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

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	//init ur ports ADCs should all be on PORTA so just pick two pins from A as input
	LCD_init();
	adc_init();
	unsigned short ud = 0; // up down value
	unsigned short lr = 0; // lr value
	unsigned char string[32];
	LCD_ClearScreen();
	//LCD_Cursor(1);
	//LCD_WriteData(0 + 'h');
	//PORTB = 0x01;
	while(1)
	{
		Set_A2D_Pin(0x01); // this is A4, change accordingly to what you wired your left right to
		lr = ADC;
		Set_A2D_Pin(0x02); // this is A5, change accordingly to what you wired your up down to
		ud = ADC;
		sprintf(string,"x: %d, y: %d", lr, ud);
		LCD_DisplayString(1,string);
		//PORTB = lr;
		delay_ms(100); // change to whatever for testing
	}

}