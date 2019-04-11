/*
 * led_v3_shiftreg+led.c
 *
 * Created: 3/10/2019 5:25:12 PM
 * Author : Robert Yee
 */ 

#include <avr/io.h>

#include "timer.h"
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



//led matrix code
//--------------------------------------
// LED Matrix Demo SynchSM
// Period: 100 ms
//--------------------------------------
enum Demo_States { shift };
int Demo_Tick ( int state ) {
	// Local Variables
	static unsigned char pattern = 0x80 ; // LED pattern - 0: LED off; 1: LED on
	static unsigned char row = 0xFE ; // Row(s) displaying pattern.
	// 0: display pattern on row
	// 1: do NOT display pattern on row
	// Transitions
	switch ( state ) {
		case shift :
		break;
		default :
		state = shift;
		break;
	}
	// Actions
	switch ( state ) {
		case shift :
		if ( row == 0xEF && pattern == 0x01 ) { // Reset demo
			pattern = 0x80;
			row = 0xFE;
			} else if ( pattern == 0x01 ) { // Move LED to start of next row
			pattern = 0x80;
			row = ( row << 1 ) | 0x01;
			} else { // Shift LED one spot to the right on current row
			pattern >>= 1;
		}
		break;
		default :
		break;
	}
	transmit_data(pattern) ; // Pattern to display
	PORTD = row ; // Row(s) displaying pattern
	return state;
}


int main(void)
{
    /* Replace with your application code */
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	TimerSet(100);
	TimerOn();
	
    while (1) 
    {
		while(!TimerFlag);
		TimerFlag = 0;
		Demo_Tick(shift);
    }
}

