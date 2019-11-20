#include <avr/io.h>
#include "timer.h"
#include <stdio.h>
#include <string.h>
#include "io.h"

volatile unsigned char TimerFlag = 0;

unsigned char SetBit(unsigned char pin, unsigned char number, unsigned char bin_value)
{
	return (bin_value ? pin | (0x01 << number) : pin & ~(0x01 << number));
}

unsigned char GetBit(unsigned char port, unsigned char number)
{
	return ( port & (0x01 << number) );
}

unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0) return b;
		a = b;
        b = c;
	}
	return 0;
}

typedef struct _task {
	signed char state;
	unsigned long int period; 
	unsigned long int elapsedTime;
	int (*TickFct)(int);
} task;

enum LCD_States {wait, next} LCD_State;
int start = 0;
int end = 1;
int pos = 16;
unsigned char myString[] = "CS120B is Legend... wait for it DARY!";
unsigned char tempString[100];


tick() {
	switch(LCD_State) {
		case -1:
			start = 1;
			end = 1;
			pos = 16;
			LCD_State = wait;
			break;
		case wait:
			if(pos < 1) {
				LCD_State = next;
			}
			break;
		case next:
			if(start > 36) {
				start = 1;
				end = 1;
				pos = 16;
				LCD_State = wait;
			}
			break;
		default:
			break;
	}
	switch(LCD_State) {
		case wait:
			strncpy(tempString, myString, end);
			LCD_DisplayString(pos, tempString);
			++end;
			--pos;
			break;
		case next:
			strncpy(tempString, myString + start, end);
			LCD_DisplayString(pos, tempString);
			++start;
			break;
		default:
			break;
	}
}


int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	LCD_init();

	unsigned long int SMTick1_calc = 150;

	unsigned long int tmpGCD = 1;
	unsigned long int GCD = tmpGCD;
	unsigned long int SMTick1_period = SMTick1_calc/GCD;

	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	task1.state = -1;
	task1.period = SMTick1_period;
	task1.elapsedTime = SMTick1_period;
	task1.TickFct = &tick;
	TimerSet(GCD);
	TimerOn();
	unsigned short i;
	while(1) {
		for ( i = 0; i < numTasks; i++ ) {
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
}
