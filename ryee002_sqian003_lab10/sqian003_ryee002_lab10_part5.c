/*	Partner(s) Name & E-mail: Robert Yee (Ryee002), Stephanie Qian (Sqian003)
 *	Lab Section: 022
 *	Assignment: Lab # 10 Exercise # 5
 *	Exercise Description: [optional - include for your own benefit]
 *	Incrementing/decrementing binary at different speeds by holding/pressing buttons
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char total = 0x00;

enum press_State {press_init, press_wait, press_pressInc, press_pressDec, press_releaseInc, press_releaseDec, press_reset} press_state;
void Tick_press();
static unsigned char press = 0x00;

enum one_State {one_init, one_wait, one_pressInc, one_pressDec, one_reset} one_state;
void Tick_one();
static unsigned char one = 0x00;

enum three_State {three_init, three_wait, three_pressInc, three_pressDec, three_releaseInc, three_releaseDec, three_reset} three_state;
void Tick_three();
static unsigned char three = 0x00;

unsigned char buttonI = 0x00;
unsigned char buttonD = 0x00;

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned long timer_one = 1000;
	unsigned long timer_three = 400;
	const unsigned long timeSet = 100;
	TimerSet(timeSet);
	TimerOn();
	press_state = press_init;
	one_state = one_init;
	three_state = three_init;
	while (1)
	{
		if (timer_one >= 1000 ) {
			Tick_one();
			timer_one = 0;
		}
		else if (timer_three >= 3000) {
			Tick_three();
			timer_three = 0;
		}
		total = press + one + three;
		if (total > 9) {
			total = 9;
		}
		if (total < 0 ) {
			total = 0;
		}
		PORTB = total;
		while(!TimerFlag){};
		TimerFlag = 0;
		timer_one += timeSet;
		timer_three += timeSet;
	}
}

void Tick_press() {
	buttonI = ~PINA & 0x01;
	buttonD = ~PINA & 0x02;
	
	switch(press_state) {
		case press_init:
		if(buttonI) {
			press_state = press_pressInc;
			break;
		}
		else if(buttonI && buttonD) {
			press_state = press_reset;
			break;
		}
		
		case press_pressInc:
		if(buttonI && !buttonD) {
			press_state = press_pressInc;
			break;
		}
		else if (!buttonI && !buttonD) {
			press_state = press_releaseInc;
			break;
		}
		else if (buttonI && buttonD) {
			press_state=press_reset;
			break;
		}
		
		case press_releaseInc:
		press_state = press_wait;
		break;
		
		case press_pressDec:
		if(!buttonI && buttonD) {
			press_state = press_pressDec;
			break;
		}
		else if (!buttonI && !buttonD) {
			press_state = press_releaseDec;
			break;
		}
		else if (buttonI && buttonD) {
			press_state=press_reset;
			break;
		}
		
		case press_releaseDec:
		press_state = press_wait;
		break;
		
		case press_wait:
		if (!buttonI && !buttonD) {
			press_state=press_wait;
			break;
		}
		else if (buttonI && buttonD) {
			press_state=press_reset;
			break;
		}
		else if (!buttonI && buttonD) {
			press_state=press_pressDec;
			break;
		}
		else if (buttonI && !buttonD) {
			press_state=press_pressInc;
			break;
		}
		
		case press_reset:
		press_state = press_wait;
		break;
	}
	switch(press_state) {
		case press_init:
		press = 0;
		break;
		
		case press_wait:
		break;
		
		case press_pressInc:
		break;
		
		case press_releaseInc:
		if (press+1 <= 9) {
			++press;
			break;
		}
		break;
		
		case press_pressDec:
		break;
		
		case press_releaseDec:
		if (press-1 >= 0) {
			--press;
			break;
		}
		break;
		
		case press_reset:
		press = 0;
		break;
	}
}

void Tick_one() {
	buttonI = ~PINA & 0x01;
	buttonD = ~PINA & 0x02;
	
	switch(one_state) {
		case one_init:
		if(buttonI) {
			one_state = one_pressInc;
			break;
		}
		else if(buttonI && buttonD) {
			one_state = one_reset;
			break;
		}
		
		case one_pressInc:
		if(buttonI && !buttonD) {
			one_state = one_pressInc;
			break;
		}
		else if (!buttonI && !buttonD) {
			one_state = one_wait;
			break;
		}
		else if (buttonI && buttonD) {
			one_state=one_reset;
			break;
		}
		
		case one_pressDec:
		if(!buttonI && buttonD) {
			one_state = one_pressDec;
			break;
		}
		else if (!buttonI && !buttonD) {
			one_state = one_wait;
			break;
		}
		else if (buttonI && buttonD) {
			one_state=one_reset;
			break;
		}
		
		case one_wait:
		if (!buttonI && !buttonD) {
			one_state=one_wait;
			break;
		}
		else if (buttonI && buttonD) {
			one_state=one_reset;
			break;
		}
		else if (!buttonI && buttonD) {
			one_state=one_pressDec;
			break;
		}
		else if (buttonI && !buttonD) {
			one_state=one_pressInc;
			break;
		}
		
		case one_reset:
		one_state = one_wait;
		break;
	}
	switch(one_state) {
		case one_init:
		one = 0;
		break;
		
		case one_wait:
		break;
		
		case one_pressInc:
		if (one+1 <= 9) {
			++one;
			break;
		}
		break;
		
		case one_pressDec:
		if (one-1 >= 0) {
			--one;
			break;
		}
		break;
		
		case one_reset:
		one = 0;
		break;
	}
}

void Tick_three() {
	buttonI = ~PINA & 0x01;
	buttonD = ~PINA & 0x02;
	
	switch(three_state) {
		case three_init:
		if(buttonI) {
			three_state = three_pressInc;
			break;
		}
		else if(buttonI && buttonD) {
			three_state = three_reset;
			break;
		}
		
		case three_pressInc:
		if(buttonI && !buttonD) {
			three_state = three_pressInc;
			break;
		}
		else if (!buttonI && !buttonD) {
			three_state = three_releaseInc;
			break;
		}
		else if (buttonI && buttonD) {
			three_state=three_reset;
			break;
		}
		
		case three_releaseInc:
		three_state = three_wait;
		break;
		
		case three_pressDec:
		if(!buttonI && buttonD) {
			three_state = three_pressDec;
			break;
		}
		else if (!buttonI && !buttonD) {
			three_state = three_releaseDec;
			break;
		}
		else if (buttonI && buttonD) {
			three_state=three_reset;
			break;
		}
		
		case three_releaseDec:
		three_state = three_wait;
		break;
		
		case three_wait:
		if (!buttonI && !buttonD) {
			three_state=three_wait;
			break;
		}
		else if (buttonI && buttonD) {
			three_state=three_reset;
			break;
		}
		else if (!buttonI && buttonD) {
			three_state=three_pressDec;
			break;
		}
		else if (buttonI && !buttonD) {
			three_state=three_pressInc;
			break;
		}
		
		case three_reset:
		three_state = three_wait;
		break;
	}
	switch(three_state) {
		case three_init:
		three = 0;
		break;
		
		case three_wait:
		break;
		
		case three_pressInc:
		break;
		
		case three_releaseInc:
		if (three+1 <= 9) {
			++three;
			break;
		}
		break;
		
		case three_pressDec:
		break;
		
		case three_releaseDec:
		if (three-1 >= 0) {
			--three;
			break;
		}
		break;
		
		case three_reset:
		three = 0;
		break;
	}
}