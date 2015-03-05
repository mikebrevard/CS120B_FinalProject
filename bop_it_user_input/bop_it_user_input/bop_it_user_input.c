/*
* Michael Brevard mbrev001@ucr.edu:
* Lab Section 022:
* Assignment: Final Project. Bop It (user input controller)
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "usart_ATmega1284.h"
#include "Utilities.h"
#include "Codes.h"
#include "io.h"

//--------Task scheduler data structure--------------------
// Struct for Tasks represent a running process in our
// simple real-time operating system.
/*Tasks should have members that include: state, period, a
measurement of elapsed time, and a function pointer.*/
typedef struct _task {
	//Task's current state, period, and the time elapsed
	// since the last tick
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	//Task tick function
	int (*TickFct)(int);
} task;

//--------End Task scheduler data structure----------------
//--------Shared Variables---------------------------------
// unsigned char const LENGTH_OF_LCD = 16;
unsigned char button1 = 0x00;
unsigned char button2 = 0x00;
unsigned char button3 = 0x00;
unsigned char button4 = 0x00;
unsigned char button5 = 0x00;
unsigned char spin_knob = 0x00;
unsigned char light_sensor = 0x00;
unsigned char const DEVICE_LCD = 0x00;
unsigned char const DEVICE_SEVENSEG = 0x80;
//--------End Shared Variables-----------------------------

void sendMessage(unsigned char device, unsigned char message) {
	//device seven seg = 0x80
	//device lcd = 0x00
	if (USART_IsSendReady(0)) {
		USART_Send(device | message, 0);
	}
}

//--------User defined FSMs--------------------------------
// get user input from devices
enum SM1_States { SM1_start };
int SMTick1(int state) {
	unsigned char a = 0x00;
	unsigned char b = 0x00;
	
	//State machine transitions
	switch (state) {
		case SM1_start:
			state = SM1_start;
			break;
		default:
			state = SM1_start;
			break;
	}
	//State machine actions
	switch(state) {
		case SM1_start: 	
			b = ~PINB;
			a = PINA;
			light_sensor = GetBit(a, 0);
			spin_knob = GetBit(a, 1);
			
			button1 = GetBit(b, 0);
			button2 = GetBit(b, 1);
			button3 = GetBit(b, 2);
			button4 = GetBit(b, 3);
			button5 = GetBit(b, 4);
			
			if (button1) {
				sendMessage(DEVICE_SEVENSEG, 0x0C);
			} else if (button2) {
				sendMessage(DEVICE_SEVENSEG, 0x02);
			} else if (button3) {
				sendMessage(DEVICE_SEVENSEG, 0x03);
			} else if (button4) {
				sendMessage(DEVICE_SEVENSEG, 0x04);
			} else if (button5) {
				sendMessage(DEVICE_SEVENSEG, 0x05);
			} else if (spin_knob) {
				sendMessage(DEVICE_SEVENSEG, 0x0A);
				PORTA = a | 0x0C;
			} else if (light_sensor) {
				sendMessage(DEVICE_SEVENSEG, 0x0E);
			}
			break;
		default: break;
	}
	return state;
}

// set lights (green LEDs and color LED)
enum SM2_States { SM2_start };
int SMTick2(int state) {
	//State machine transitions
	switch (state) {
		case SM2_start:
			state = SM2_start;
			break;
		default:
			state = SM2_start;
			break;
	}
	//State machine actions
	switch(state) {
		case SM2_start: 				

			break;
		default: break;
	}
	return state;
}

// Implement scheduler code from PES.
int main() {
	DDRA = 0xFC; PORTA = 0x03;
	DDRB = 0x00; PORTB = 0xFF;
	//DDRC = 0xFF; PORTC = 0x00; 
	DDRD = 0xFF; PORTD = 0x00; 
	
	// Period for the tasks
	unsigned long int SMTick1_calc = 1;
	unsigned long int SMTick2_calc = 10;
	
	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);
	
	//Greatest common divisor for all tasks
	// or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;
	
	//Recalculate GCD periods for scheduler
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick2_period = SMTick2_calc/GCD;

	//Declare an array of tasks
	static task task1, task2;
	task *tasks[] = { &task1, &task2 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	// Task 1
	task1.state = -1;
	task1.period = SMTick1_period;
	task1.elapsedTime = SMTick1_period;
	task1.TickFct = &SMTick1;
	// Task 2
	task2.state = -1;
	task2.period = SMTick2_period;
	task2.elapsedTime = SMTick2_period;
	task2.TickFct = &SMTick2;
	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();
	
	initUSART(0);
	USART_Flush(0);
	
	// Scheduler for-loop iterator
	unsigned short i;
	
	while(1) {		
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
	// Error: Program should not exit!
	return 0;
}