/*
* Michael Brevard mbrev001@ucr.edu:
* Lab Section 022:
* Assignment: Final Project. Bop It (display controller)
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "Codes.h"
#include "io.h"
#include "usart_ATmega1284.h"
#include "Utilities.h"

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
unsigned char const LENGTH_OF_LCD = 16;
unsigned char array_position = 0;
unsigned char message[] = "Welcome to Embedded Bop it!";
unsigned char display[16];
unsigned char user_choice = 0;
//--------End Shared Variables-----------------------------

unsigned char isMessageChanging() {
	unsigned char temp[sizeof(message)];
	if (user_choice == CODE_BUTTON1) {
		strcpy(temp, "Button 1");
	} else if (user_choice == CODE_BUTTON2) {
		strcpy(temp, "Button 2");
	} else if (user_choice == CODE_BUTTON3) {
		strcpy(temp, "Button 3");
	} else if (user_choice == CODE_BUTTON4) {
		strcpy(temp, "Button 4");
	} else if (user_choice == CODE_BUTTON5) {
		strcpy(temp, "Button 5");
	} else if (user_choice == CODE_NOTHING) {
		strcpy(temp, "Nothing");
	} else if (user_choice == CODE_SPINKNOB) {
		strcpy(temp, "Spin Knob");
	} else {
		strcpy(temp, "Uncaught value");
	}
	
	if (strcmp(temp,message) == 0) {
		return 0;
	} else {
		memset(message,'\0',strlen(message));
		strcpy(message, temp);
		return 1;
	}
}


//--------User defined FSMs--------------------------------
enum SM1_States { SM1_start };
// Monitors button connected to PA0. When the button is
// pressed, shared variable "pause" is toggled.
int SMTick1(int state) {
	unsigned char cursor_position = 1;
	unsigned char counter = 0x00;
	
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
			if (isMessageChanging()) {
				array_position = 0;
				//memset(display,0,strlen(display));
			}
		
			for (counter = 0, cursor_position = array_position; counter < LENGTH_OF_LCD; cursor_position = cursor_position + 1, counter = counter + 1) {
				if (cursor_position > strlen(message)) {
					cursor_position = 0;
					display[counter] = ' ';
				} else if (cursor_position - 1 >= 0) {
					display[counter] = message[cursor_position - 1]; 
				}
				
			}
			
			for (counter = 0, cursor_position = 0; counter < LENGTH_OF_LCD; cursor_position = cursor_position + 1, counter = counter + 1) {
				LCD_Cursor(cursor_position);
				LCD_WriteData(display[counter]);
			}	
			array_position = array_position + 1;
			if (array_position == strlen(message)) {
				array_position = 0;
			}			
			break;
		default: break;
	}
	return state;
}

/**
* get received message and save as user choice
*/
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
			if (USART_HasReceived(1)) {
				user_choice = USART_Receive(1);
			} 
			break;
		default: break;
	}
	return state;
}

// Implement scheduler code from PES.
int main() {
	// Set Data Direction Registers
	// Buttons PORTA[0-7], set AVR PORTA
	// to pull down logic
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	// Period for the tasks
	unsigned long int SMTick1_calc = 500;
	unsigned long int SMTick2_calc = 1;
	
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
	task *tasks[] = { &task1 , &task2 };
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
	// Scheduler for-loop iterator
	unsigned short i;
	LCD_init();
	LCD_ClearScreen();
	
	initUSART(1);
	USART_Flush(1);
	
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