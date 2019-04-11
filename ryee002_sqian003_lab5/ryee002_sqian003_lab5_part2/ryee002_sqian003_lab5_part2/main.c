#include <avr/io.h>
enum State {init, wait, pressInc, pressDec, releaseInc, releaseDec, reset} state;

unsigned char total = 0x07;
unsigned char buttonI = 0x00;
unsigned char buttonD = 0x00;


void tick() {
	switch(state) {
		case init:
		if(buttonI) {
			state = pressInc;
			break;
		}
		else if(buttonI && buttonD) {
			state = reset;
			break;
		}
		
		case pressInc:
		if(buttonI && !buttonD) {
			state = pressInc;
			break;
		}
		else if (!buttonI && !buttonD) {
			state = releaseInc;
			break;
		}
		else if (buttonI && buttonD) {
			state=reset;
			break;
		}
		
		case releaseInc:
		state = wait;
		break;
		
		case pressDec:
		if(!buttonI && buttonD) {
			state = pressDec;
			break;
		}
		else if (!buttonI && !buttonD) {
			state = releaseDec;
			break;
		}
		else if (buttonI && buttonD) {
			state=reset;
			break;
		}
		
		case releaseDec:
		state = wait;
		break;
		
		case wait:
		if (!buttonI && !buttonD) {
			state=wait;
			break;
		}
		else if (buttonI && buttonD) {
			state=reset;
			break;
		}
		else if (!buttonI && buttonD) {
			state=pressDec;
			break;
		}
		else if (buttonI && !buttonD) {
			state=pressInc;
			break;
		}
		
		case reset:
		break;
	}
	switch(state) {
		case init:
		break;
		
		case wait:
		break;
		
		case pressInc:
		break;
		
		case releaseInc:
		if (total+1 <= 9) {
			++total;
			PORTC = total;
			break;
		}
		break;
		
		case pressDec:
		break;
		
		case releaseDec:
		if (total-1 >= 0) {
			--total;
			PORTC = total;
			break;
		}
		break;
		
		case reset:
		total = 0;
		PORTC = total;
		break;
	}
}

int main(void)
{
	/* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	buttonI = ~PINA & 0x01;
	buttonD = ~PINA & 0x02;
	PORTC = total;
	while (1)
	{
		tick();
	}
}