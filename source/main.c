/*	Author: hhuyn019
 *  Partner(s) Name: Badr Asi
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char GetKeypadKey() {
	PORTC = 0xEF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('1'); }
	if (GetBit(PINC,1)==0) { return('4'); }
	if (GetBit(PINC,2)==0) { return('7'); }
	if (GetBit(PINC,3)==0) { return('*'); }
	
	PORTC = 0xDF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('2'); }
	if (GetBit(PINC,1)==0) { return('5'); }
	if (GetBit(PINC,2)==0) { return('8'); }
	if (GetBit(PINC,3)==0) { return('0'); }

	PORTC = 0xBF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('3'); }
	if (GetBit(PINC,1)==0) { return('6'); }
	if (GetBit(PINC,2)==0) { return('9'); }
	if (GetBit(PINC,3)==0) { return('#'); }

	PORTC = 0xBF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('A'); }
	if (GetBit(PINC,1)==0) { return('B'); }
	if (GetBit(PINC,2)==0) { return('C'); }
	if (GetBit(PINC,3)==0) { return('D'); }

	return('\0');
}

typedef struct _task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct)(int);
} task;

unsigned char led0_output = 0x00;
unsigned char led1_output = 0x00;
unsigned char pause = 0;

enum pauseButtonSM_States {pauseButton_wait, pauseButton_press, pauseButton_release} pauseButtonSM_State;

int pauseButtonSMTick(int pauseButtonSM_State) {
	unsigned char press = ~PINA & 0x01;
	switch(pauseButtonSM_State) {
		case pauseButton_wait:
			pauseButtonSM_State = press == 0x01? pauseButton_press: pauseButton_wait; break;
		case pauseButton_press:
			pauseButtonSM_State = pauseButton_release; break;
		case pauseButton_release:
			pauseButtonSM_State = press == 0x00? pauseButton_wait: pauseButton_press; break;
		default: pauseButtonSM_State = pauseButton_wait; break;
	}
	switch(pauseButtonSM_State) {
		case pauseButton_wait: break;
		case pauseButton_press:
			pause = (pause == 0) ? 1 : 0;
			break;
		case pauseButton_release: break;
	}
	return pauseButtonSM_State;
}

enum toggleLED0_States {toggleLED0_wait, toggleLED0_blink} toggleLED0_State;

int toggleLED0SMTick(int toggleLED0_State) {
	switch(toggleLED0_State) {
		case toggleLED0_wait: toggleLED0_State = !pause? toggleLED0_blink: toggleLED0_wait; break;
		case toggleLED0_blink: toggleLED0_State = pause? toggleLED0_wait: toggleLED0_blink; break;
		default: toggleLED0_State = toggleLED0_wait; break;
	}
	switch(toggleLED0_State) {
		case toggleLED0_wait: break;
		case toggleLED0_blink:
			led0_output = (led0_output == 0x00) ? 0x01 : 0x00;
			break;
	}
	return toggleLED0_State;
}

enum toggleLED1_States {toggleLED1_wait, toggleLED1_blink} toggleLED1_State;

int toggleLED1SMTick(int toggleLED1_State) {
	switch(toggleLED1_State) {
		case toggleLED1_wait: toggleLED1_State = !pause? toggleLED1_blink: toggleLED1_wait; break;
		case toggleLED1_blink: toggleLED1_State = pause? toggleLED1_wait: toggleLED1_blink; break;
		default: toggleLED1_State = toggleLED1_wait; break;
	}
	switch(toggleLED1_State) {
		case toggleLED1_wait: break;
		case toggleLED1_blink:
			led1_output = (led1_output == 0x00) ? 0x01 : 0x00;
			break;
	}
	return toggleLED1_State;
}

enum display_States {display_display} display_State;

int displaySMTick(int display_State) {
	unsigned char output;
	switch(display_State) {
		case display_display: display_State = display_display; break;
		default: display_State = display_display; break;
	}
	switch(dislay_State) {
		case display_display:
			output = led0_output | led1_output << 1;
			break;
	}
	PORTB = output;
	return display_State;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	unsigned char x;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
    /* Insert your solution below */
	while (1) {
		x = GetKeypadKey();
		switch(x) {
			case '\0': PORTB = 0x1F; break;
			case '1': PORTB = 0x01; break;
			case '2': PORTB = 0x02; break;
			case '3': PORTB = 0x03; break;
			case '4': PORTB = 0x04; break;
			case '5': PORTB = 0x05; break;
			case '6': PORTB = 0x06; break;
			case '7': PORTB = 0x07; break;
			case '8': PORTB = 0x08; break;
			case '9': PORTB = 0x09; break;
			case 'A': PORTB = 0x0A; break;
			case 'B': PORTB = 0x0B; break;
			case 'C': PORTB = 0x0C; break;
			case 'D': PORTB = 0x0D; break;
			case '*': PORTB = 0x0E; break;
			case '#': PORTB = 0x0F; break;
			case '0': PORTB = 0x00; break;
			default: PORTB = 0x1B; break;
		}
	}
	return 1;
}
