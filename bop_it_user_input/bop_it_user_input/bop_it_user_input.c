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

// user input
unsigned char button1 = 0x00;
unsigned char button2 = 0x00;
unsigned char button3 = 0x00;
unsigned char button4 = 0x00;
unsigned char button5 = 0x00;
unsigned char spin_knob = 0x00;
unsigned char LED_spin_knob = 0x00;
unsigned char LED_buttons = 0x00;
unsigned char light_sensor = 0x00; //TODO: possibly remove
unsigned char keypad_value = 0x00;

//game information
unsigned char game_state = 0x00;
unsigned char game_selection = 0x00;
unsigned char game_counter = 0x00;
unsigned char game_score = 0x00;

//--------End Shared Variables-----------------------------

void sendMessage(unsigned char device, unsigned char message) {
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
	unsigned char keypad_temp = 0x00;
	
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
			a = PINA;	
			b = ~PINB;
			
			//light_sensor = GetBit(a, 0);
			spin_knob = GetBit(a, 1);
			
			button1 = GetBit(b, 0);
			button2 = GetBit(b, 1);
			button3 = GetBit(b, 2);
			button4 = GetBit(b, 3);
			button5 = GetBit(b, 4);
			keypad_temp = GetKeypadKey();
			
			if (keypad_temp != '\0') {
				if (keypad_temp == '7') {
					keypad_value = 0x00;
				} else if (keypad_temp == 'D') { 
					keypad_value = 0x01;
				} else if (keypad_temp == 'C') { 
					keypad_value = 0x02;
				} else if (keypad_temp == 'B') {
					keypad_value = 0x03; 
				} else if (keypad_temp == '#') {
					keypad_value = 0x04;
				} else if (keypad_temp == '9') { 
					keypad_value = 0x05;
				} else if (keypad_temp == '6') {
					keypad_value = 0x06; 
				} else if (keypad_temp == '0') {
					keypad_value = 0x07;
				} else if (keypad_temp == '8') { 
					keypad_value = 0x08;
				} else if (keypad_temp == '5') {
					keypad_value = 0x09;
				} else {
					keypad_value = 0x0F;
				}
			} else {
				keypad_value = 0x0B;
			}
			break;
		default: break;
	}
	return state;
}

// game logic
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

// lights to display and send message to secondary AVR
enum SM3_States { SM3_start };
int SMTick3(int state) {
	//State machine transitions
	switch (state) {
		case SM3_start:
			state = SM3_start;
			break;
		default:
			state = SM3_start;
			break;
	}
	//State machine actions
	switch(state) {
		case SM3_start: 	
			LED_spin_knob = 0x00;
			LED_buttons = 0x00;	
					
			if (button1) {
				LED_buttons = 0x08; //all blue
			} else if (button2) {
				LED_buttons = 0x02; //all green
			} else if (button3) {
				LED_buttons = 0x09; //purple (red + blue)
			} else if (button4) {
				LED_buttons = 0x0B; //silver (red + blue + green)
			} else if (button5) {
				LED_buttons = 0x01; //all red
			} else if (spin_knob) {
				LED_spin_knob = 0x01;
			} else if (keypad_value != 0x0B) {
				if (keypad_value == 0x0F) { //error key (something other than 0-9)
					//TODO: change A to F for fail!
					sendMessage(CODE_DEVICE_SEVENSEG, 0x0F);
				} else {
					if (keypad_value == 0x00) {
						sendMessage(CODE_DEVICE_SEVENSEG, 0x00);
					} else if (keypad_value == 0x01) {
						sendMessage(CODE_DEVICE_SEVENSEG, 0x01);
					} else if (keypad_value == 0x02) {
						sendMessage(CODE_DEVICE_SEVENSEG, 0x02);
					} else if (keypad_value == 0x03) {
						sendMessage(CODE_DEVICE_SEVENSEG, 0x03);
					} else if (keypad_value == 0x04) {
						sendMessage(CODE_DEVICE_SEVENSEG, 0x04);
					} else if (keypad_value == 0x05) {
						sendMessage(CODE_DEVICE_SEVENSEG, 0x05);
					} else if (keypad_value == 0x06) {
						sendMessage(CODE_DEVICE_SEVENSEG, 0x06);
					} else if (keypad_value == 0x07) {
						sendMessage(CODE_DEVICE_SEVENSEG, 0x07);
					} else if (keypad_value == 0x08) {
						sendMessage(CODE_DEVICE_SEVENSEG, 0x08);
					} else if (keypad_value == 0x09) {
						sendMessage(CODE_DEVICE_SEVENSEG, 0x09);
					} 				
				}
			} 			
			//set lights (if on)
			PORTD = (LED_spin_knob << 3) | (LED_buttons << 4);
		
			break;
		default: break;
	}
	return state;
}

// update game counter
enum SM4_States { SM4_start };
int SMTick4(int state) {
	//State machine transitions
	switch (state) {
		case SM4_start:
			state = SM4_start;
			break;
		default:
			state = SM4_start;
			break;
	}
	//State machine actions
	switch(state) {
		case SM4_start: 	
			if (game_counter > 0) {
				game_counter = game_counter - 1;
			} 
			break;
		default: break;
	}
	return state;
}

// game play
enum SM5_States { SM5_start };
int SMTick5(int state) {
	//State machine transitions
	switch (state) {
		case SM5_start:
			state = SM5_start;
			break;
		default:
			state = SM5_start;
			break;
	}
	//State machine actions
	switch(state) {
		case SM5_start: 		
			//game play
			if (game_state == CODE_IN_GAME_PLAY) {
				if (game_selection == CODE_NOTHING) { //choice game piece
					//decide 1 through 5
					game_selection = rand() % 16 + 1;
					
					//TODO: make game_counter fluctuate based on correct right responses
					game_counter = 0x09;
				
				} else if (game_counter > 0) { //    if answer has been chosen (right or wrong)
					if (button1) {
						game_state = (game_selection == CODE_BUTTON1) ? CODE_DISPLAY_CORRECT : CODE_DISPLAY_INCORRECT;
					} else if (button2) {
						game_state = (game_selection == CODE_BUTTON2) ? CODE_DISPLAY_CORRECT : CODE_DISPLAY_INCORRECT;
					} else if (button3) { 
						game_state = (game_selection == CODE_BUTTON3) ? CODE_DISPLAY_CORRECT : CODE_DISPLAY_INCORRECT;
					} else if (button4) {
						game_state = (game_selection == CODE_BUTTON4) ? CODE_DISPLAY_CORRECT : CODE_DISPLAY_INCORRECT;
					} else if (button5) {
						game_state = (game_selection == CODE_BUTTON5) ? CODE_DISPLAY_CORRECT : CODE_DISPLAY_INCORRECT;
					} else if (spin_knob) {
						game_state = (game_selection == CODE_SPINKNOB) ? CODE_DISPLAY_CORRECT : CODE_DISPLAY_INCORRECT;
					} else if (keypad_value == 0x00) {
						game_state = (game_selection == CODE_KEYPAD0) ? CODE_DISPLAY_CORRECT : CODE_DISPLAY_INCORRECT;
					} else if (keypad_value == 0x01) {
						game_state = (game_selection == CODE_KEYPAD1) ? CODE_DISPLAY_CORRECT : CODE_DISPLAY_INCORRECT;
					} else if (keypad_value == 0x02) {
						game_state = (game_selection == CODE_KEYPAD2) ? CODE_DISPLAY_CORRECT : CODE_DISPLAY_INCORRECT;
					} else if (keypad_value == 0x03) {
						game_state = (game_selection == CODE_KEYPAD3) ? CODE_DISPLAY_CORRECT : CODE_DISPLAY_INCORRECT;
					} else if (keypad_value == 0x04) {
						game_state = (game_selection == CODE_KEYPAD4) ? CODE_DISPLAY_CORRECT : CODE_DISPLAY_INCORRECT;
					} else if (keypad_value == 0x05) {
						game_state = (game_selection == CODE_KEYPAD5) ? CODE_DISPLAY_CORRECT : CODE_DISPLAY_INCORRECT;
					} else if (keypad_value == 0x06) {
						game_state = (game_selection == CODE_KEYPAD6) ? CODE_DISPLAY_CORRECT : CODE_DISPLAY_INCORRECT;
					} else if (keypad_value == 0x07) {
						game_state = (game_selection == CODE_KEYPAD7) ? CODE_DISPLAY_CORRECT : CODE_DISPLAY_INCORRECT;
					} else if (keypad_value == 0x08) {
						game_state = (game_selection == CODE_KEYPAD8) ? CODE_DISPLAY_CORRECT : CODE_DISPLAY_INCORRECT;
					} else if (keypad_value == 0x09) {
						game_state = (game_selection == CODE_KEYPAD9) ? CODE_DISPLAY_CORRECT : CODE_DISPLAY_INCORRECT;
					} 	
					
					//something was select (correct or incorrect)
					if (game_state != CODE_IN_GAME_PLAY) {
						game_counter = 0x02;
						game_selection = CODE_NOTHING;
					}
					
				} else { //time ran out
					game_state = CODE_DISPLAY_INCORRECT;
					game_selection = CODE_NOTHING;
				}
				
				//add to their score
				if (game_state == CODE_DISPLAY_CORRECT) {
					game_score = game_score + 1;
				}
			} else if (game_state == CODE_DISPLAY_WELCOME || game_state == CODE_DISPLAY_INCORRECT) {
				game_selection = CODE_NOTHING;
				game_score = 0x00;
				if (button2) { //green button
					game_state = CODE_DISPLAY_GAME_STARTING;
					game_counter = 0x05;
					
					//send message first time but rest is taken care of state machine 4
					sendMessage(CODE_DEVICE_GAME_COUNTER, game_counter);
				}
			} else if (game_state == CODE_DISPLAY_GAME_STARTING || game_state == CODE_DISPLAY_CORRECT) {
				game_selection = CODE_NOTHING;
				if (game_counter == 0) {
					game_state = CODE_IN_GAME_PLAY;
				} else {
					sendMessage(CODE_DEVICE_GAME_COUNTER, game_counter);
				} 
			} 
			
			//send message
			if (game_state == CODE_IN_GAME_PLAY) {
				sendMessage(CODE_DEVICE_GAME_COUNTER, game_score);
				sendMessage(CODE_DEVICE_GAME_PIECE, game_selection);
			} else {
				sendMessage(CODE_DEVICE_LCD, game_state);
			}
			
			break;
		default: break;
	}
	return state;
}

// Implement scheduler code from PES.
int main() {
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF; PORTD = 0x00; 
	
	// Period for the tasks
	unsigned long int SMTick1_calc = 1;
	unsigned long int SMTick2_calc = 5;
	unsigned long int SMTick3_calc = 100;
	unsigned long int SMTick4_calc = 1000;
	unsigned long int SMTick5_calc = 5;
	
	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);
	tmpGCD = findGCD(tmpGCD, SMTick3_calc);
	tmpGCD = findGCD(tmpGCD, SMTick4_calc);
	tmpGCD = findGCD(tmpGCD, SMTick5_calc);
	
	//Greatest common divisor for all tasks
	// or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;
	
	//Recalculate GCD periods for scheduler
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick2_period = SMTick2_calc/GCD;
	unsigned long int SMTick3_period = SMTick3_calc/GCD;
	unsigned long int SMTick4_period = SMTick4_calc/GCD;
	unsigned long int SMTick5_period = SMTick5_calc/GCD;

	//Declare an array of tasks
	static task task1, task2, task3, task4, task5;
	task *tasks[] = { &task1, &task2, &task3, &task4, &task5 };
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
	// Task 3
	task3.state = -1;
	task3.period = SMTick3_period;
	task3.elapsedTime = SMTick3_period;
	task3.TickFct = &SMTick3;
	// Task 4
	task4.state = -1;
	task4.period = SMTick4_period;
	task4.elapsedTime = SMTick4_period;
	task4.TickFct = &SMTick4;
	// Task 5
	task5.state = -1;
	task5.period = SMTick5_period;
	task5.elapsedTime = SMTick5_period;
	task5.TickFct = &SMTick5;
	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();
	
	//set USART
	initUSART(0);
	USART_Flush(0);
	
	//set game state
	game_state = CODE_DISPLAY_WELCOME;
	
	
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