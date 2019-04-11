/*
 * led_v11_arrayMatrix.c
 *
 * Created: 3/14/2019 12:21:43 AM
 * Author : Robert Yee
 */ 

#include <avr/io.h>

//#include "timer.h"
#include <ucr/scheduler.h>
volatile unsigned char TimerFlag = 0;
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

//shift reg code
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
	PORTD = row;
	transmit_data(pattern);
}
//test to see if you can output 2D array coordinates
//bit access
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}


/*
enum State2 {wait1, buttonOn} state2;
char array[40][40];
unsigned char tempCol;
unsigned char tempVal;
unsigned char butOn = 0x00;
void buttonPress() {
	switch(state2) {
		case wait:
		if (butOn) {
			state2 = buttonOn;
		}
		else {
			state2 = wait;
		}
		
		case buttonOn:
		if(buttonOn) {
			array[patternCnt][rowCnt] = 1; <- using rowCnt and patterCnt to mark locationse
		}
		else {
			state2 = wait;
		}
	}
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (array[i][j] == 1) {
				tempCol = SetBit(tempCol, i , 1);
				tempVal = SetBit(tempVal, j, 0);
			}
		}
	}
	transmit_data(tempVal);
	PORTD = tempCol;
}
*/
///*
enum TestState {wait3} state3;
char testArray[14][14];

void Test_state() {
	unsigned columnVal = 0x00;
	unsigned rowVal = 0xFF;
	switch(state3) {
		case wait3:
		
		//columnVal = SetBit(columnVal, 4, 1); //left right
		//rowVal = SetBit(rowVal, 1, 0); //up down
		testArray[0][2] = 1;
		testArray[0][3] = 1;
		testArray[0][5] = 1;
		testArray[0][6] = 1;
		testArray[1][1] = 1;
		testArray[1][4] = 1;
		testArray[1][7] = 1;
		testArray[2][1] = 1;
		testArray[2][7] = 1;
		testArray[3][2] = 1;
		testArray[3][6] = 1;
		testArray[4][3] = 1;
		testArray[4][4] = 1;
		testArray[4][5] = 1;
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 8; ++j) {
				if(testArray[i][j] == 1) {
					columnVal = SetBit(columnVal, j, 1); //left right
					rowVal = SetBit(rowVal, i, 0); //up down
					PORTD = rowVal;
					transmit_data(columnVal);
				}
			}
			
		}
			}
	//PORTD = rowVal;
	//transmit_data(columnVal);
	};
//*/
/*
enum TestState {init, s1, s2, s3, s4} LEDstate;
void Test_state()
{
	static unsigned char pattern = 0x80;
	static unsigned char row = 0x00;
	switch(LEDstate)
	{
		case init:
		LEDstate = s1;
		break;
		case s1:
		LEDstate = s2;
		break;
		case s2:
		LEDstate = s3;
		break;
		case s3:
		LEDstate = s4;
		break;
		case s4:
		LEDstate = s1;
		break;
		default:
		LEDstate = init;
		break;
		
	}
	switch(LEDstate)
	{
		case init:
		break;
		case s1:
		pattern = 0x20;
		row = ~0x0E;
		break;
		case s2:
		pattern = 0x10;
		row = ~0x0A;
		break;
		case s3:
		pattern = 0x08;
		row = ~0x0A;
		break;
		case s4:
		pattern = 0x04;
		row = ~0x0E;
		break;
		default:
		break;
	}
	transmit_data(pattern);		// Pattern to display
	PORTD = row;		// Row(s) displaying pattern
};		
*/		
//find gcd
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
		b = c;
	}
	return 0;
}



int main(void)
{
	/* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00; //led
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	adc_init();
	
	unsigned long int SMTick1_calc = 1000;
	unsigned long int SMTick2_calc = 250;
	
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);
	unsigned long int GCD = tmpGCD;
	
	TimerSet(GCD);
	TimerOn();
	
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick2_period = SMTick2_calc/GCD;

	
	static task task1, task2;
	task *tasks[] = { &task1, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// Task 1
	task1.state = wait;//Task initial state.
	task1.period = SMTick1_period;//Task Period.
	task1.elapsedTime = SMTick1_period;//Task current elapsed time.
	task1.TickFct = &udlr;//Function pointer for the tick.

	// Task 2
	task2.state = wait3;//Task initial state. (init)
	task2.period = SMTick2_period;//Task Period.
	task2.elapsedTime = SMTick2_period;//Task current elapsed time.
	task2.TickFct = &Test_state;//Function pointer for the tick.

	
	
	while (1)
	{
		for ( int i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}

		while(!TimerFlag);
		TimerFlag = 0;
		
	}
	return 0;
}



