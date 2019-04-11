#include <avr/io.h>
#include <ucr/bit.h>

// Returns '\0' if no key pressed, else returns char '1', '2', ... '9', 'A', ...
// If multiple keys pressed, returns leftmost-topmost one
// Keypad must be connected to port C
/* Keypad arrangement
        PC4 PC5 PC6 PC7
   col  1   2   3   4
row
PC0 1   1 | 2 | 3 | A
PC1 2   4 | 5 | 6 | B
PC2 3   7 | 8 | 9 | C
PC3 4   * | 0 | # | D
*/
unsigned char GetKeypadKey() {

	PORTB = 0xEF; // Enable col 4 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINB,0)==0) { return('1'); }
	if (GetBit(PINB,1)==0) { return('4'); }
	if (GetBit(PINB,2)==0) { return('7'); }
	if (GetBit(PINB,3)==0) { return('*'); }

	// Check keys in col 2
	PORTB = 0xDF; // Enable col 5 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINB,0)==0) { return('2'); }
	// ... *****FINISH*****

	// Check keys in col 3
	PORTB = 0xBF; // Enable col 6 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	// ... *****FINISH*****

	// Check keys in col 4	
	// ... *****FINISH*****

	return('\0'); // default value

}

int main(void)
{
	unsigned char x;
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	DDRC = 0x00; PORTC = 0xFF; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	while(1) {
		x = GetKeypadKey();
		switch (x) {
			case '\0': PORTC = 0x1F; break; // All 5 LEDs on
			case '1': PORTC = 0x01; break; // hex equivalent
			case '2': PORTC = 0x02; break;

			// . . . ***** FINISH *****

			case 'D': PORTC = 0x0D; break;
			case '*': PORTC = 0x0E; break;
			case '0': PORTC = 0x00; break;
			case '#': PORTC = 0x0F; break;
			default: PORTC = 0x1B; break; // Should never occur. Middle LED off.
		}
	}
}
