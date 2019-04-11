/* Partner 1 Name & E-mail: Stephanie Qian, sqian003@ucr.edu
* Partner 2 Name & E-mail: Robert Yee, ryee002@ucr.edu
* Lab Section: 022
* Assignment: Lab #2 Exercise #3
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tempA = 0x00;
	unsigned char tempB = 0x00;
	unsigned char tempC = 0x00;
	unsigned char tempD = 0x00;
	while (1)
	{
		unsigned char cntavail = 0x00;
		// Read input
		tempA = PINA & 0x01;
		tempB = PINA & 0x02;
		tempC = PINA & 0x04;
		tempD = PINA & 0x08;
		// check if the pins are 1 or 0
		if(tempA == 0x00) {
			cntavail++;
		}
		if(tempB == 0x00) {
			cntavail++;
		}
		if(tempC == 0x00) {
			cntavail++;
		}
		if(tempD == 0x00) {
			cntavail++;
		}
		// output
		if (cntavail == 0x00) {
			cntavail=0x80;
		}
		PORTC = cntavail;
	}
}