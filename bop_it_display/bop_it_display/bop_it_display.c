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
unsigned char bottom_message[] = "Start -> Green";
unsigned char display[16];
unsigned char lcd_choice = 0x00;
unsigned char bottom_lcd_choice = 0x00;
unsigned char sevendeg_choice = 0x00;
unsigned char game_counter = 0x00;
unsigned char first_digit = '0';
unsigned char second_digit = '0';
//--------End Shared Variables-----------------------------

unsigned char isMessageChanging() {
	unsigned char temp[sizeof(message)];
	if (lcd_choice == CODE_DISPLAY_WELCOME) {
		strcpy(temp, "Welcome to Embedded Bop it!");
	} else if (lcd_choice == CODE_DISPLAY_GAME_STARTING) {
		strcpy(temp, "Let go of all input");
	} else if (lcd_choice == CODE_DISPLAY_PUSH_IT) {
		strcpy(temp, "Push it!");
	} else if (lcd_choice == CODE_DISPLAY_TWIST_IT) {
		strcpy(temp, "Twist it!");
	} else if (lcd_choice == CODE_DISPLAY_COVER_IT) {
		strcpy(temp, "Cover it!");
	} else if (lcd_choice == CODE_DISPLAY_CORRECT) {
		strcpy(temp, "Correct!");
	} else if (lcd_choice == CODE_DISPLAY_INCORRECT) {
		strcpy(temp, "Incorrect! Play Again?");
	} 

	
	if (strcmp(temp,message) == 0) {
		// return that there is NO change
		return 0;
	}	
	
	//update top message
	memset(message,'\0',strlen(message));
	strcpy(message, temp);
		
	//return that there is a change
	return 1;
}

void setBottomMessage() {
	unsigned char bottom_temp[sizeof(bottom_message)];
	
	if (bottom_lcd_choice == CODE_DISPLAY_WELCOME) {
		strcpy(bottom_temp, "Start -> Green");
	} else if (bottom_lcd_choice == CODE_DISPLAY_GAME_STARTING) {
		strcpy(bottom_temp, "Starting in ");
	} else if (bottom_lcd_choice == CODE_BUTTON1) {
		strcpy(bottom_temp, "Blue Button");
	} else if (bottom_lcd_choice == CODE_BUTTON2) {
		strcpy(bottom_temp, "Green Button");
	} else if (bottom_lcd_choice == CODE_BUTTON3) {
		strcpy(bottom_temp, "Purple Button");
	} else if (bottom_lcd_choice == CODE_BUTTON4) {
		strcpy(bottom_temp, "Silver Button");
	} else if (bottom_lcd_choice == CODE_BUTTON5) {
		strcpy(bottom_temp, "Red Button");
	} else if (bottom_lcd_choice == CODE_SPINKNOB) {
		strcpy(bottom_temp, "Spin Knob");
	} else if (bottom_lcd_choice == CODE_KEYPAD0) {
		strcpy(bottom_temp, "Keypad 0");
	} else if (bottom_lcd_choice == CODE_KEYPAD1) {
		strcpy(bottom_temp, "Keypad 1");
	} else if (bottom_lcd_choice == CODE_KEYPAD2) {
		strcpy(bottom_temp, "Keypad 2");
	} else if (bottom_lcd_choice == CODE_KEYPAD3) {
		strcpy(bottom_temp, "Keypad 3");
	} else if (bottom_lcd_choice == CODE_KEYPAD4) {
		strcpy(bottom_temp, "Keypad 4");
	} else if (bottom_lcd_choice == CODE_KEYPAD5) {
		strcpy(bottom_temp, "Keypad 5");
	} else if (bottom_lcd_choice == CODE_KEYPAD6) {
		strcpy(bottom_temp, "Keypad 6");
	} else if (bottom_lcd_choice == CODE_KEYPAD7) {
		strcpy(bottom_temp, "Keypad 7");
	} else if (bottom_lcd_choice == CODE_KEYPAD8) {
		strcpy(bottom_temp, "Keypad 8");
	} else if (bottom_lcd_choice == CODE_KEYPAD9) {
		strcpy(bottom_temp, "Keypad 9");
	} else if (bottom_lcd_choice == CODE_DISPLAY_CORRECT) {
		strcpy(bottom_temp, "Good Job!");
	} else if (bottom_lcd_choice == CODE_DISPLAY_INCORRECT) {
		strcpy(bottom_temp, "Final Score");
	}
	
	//update bottom message
	memset(bottom_message,'\0',strlen(bottom_message));
	strcpy(bottom_message, bottom_temp);
}

unsigned char showScore() {
	unsigned f = 0x00;
	unsigned s = 0x00;

	//f is 1's value
	//s is the 10's value
	f = game_counter % 10;
	s = game_counter / 10;

	if (f == 0x00) {
		first_digit = '0';
	} else if (f == 0x01) {
		first_digit = '1';
	} else if (f == 0x02) {
		first_digit = '2';
	} else if (f == 0x03) {
		first_digit = '3';
	} else if (f == 0x04) {
		first_digit = '4';
	} else if (f == 0x05) {
		first_digit = '5';
	} else if (f == 0x06) {
		first_digit = '6';
	} else if (f == 0x07) {
		first_digit = '7';
	} else if (f == 0x08) {
		first_digit = '8';
	} else if (f == 0x09) {
		first_digit = '9';
	} 
	
	if (s == 0x00) {
		second_digit = '0';
	} else if (s == 0x01) {
		second_digit = '1';
	} else if (s == 0x02) {
		second_digit = '2';
	} else if (s == 0x03) {
		second_digit = '3';
	} else if (s == 0x04) {
		second_digit = '4';
	} else if (s == 0x05) {
		second_digit = '5';
	} else if (s == 0x06) {
		second_digit = '6';
	} else if (s == 0x07) {
		second_digit = '7';
	} else if (s == 0x08) {
		second_digit = '8';
	} else if (s == 0x09) {
		second_digit = '9';
	} 
	
	return (game_counter != 0 && lcd_choice != CODE_DISPLAY_WELCOME && lcd_choice != CODE_DISPLAY_CORRECT) ? 1 : 0;
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
			
			// bottom message display
			setBottomMessage();
			for (counter = 0, cursor_position = LENGTH_OF_LCD + 1; counter < LENGTH_OF_LCD * 2; cursor_position = cursor_position + 1, counter = counter + 1) {
				if (counter < strlen((bottom_message))) {
					LCD_Cursor(cursor_position);
					LCD_WriteData(bottom_message[counter]);
				} else {
					LCD_Cursor(cursor_position);
					LCD_WriteData(' ');
				}
			}
			
			//display score
			if (showScore()) {
				LCD_Cursor(31);
				LCD_WriteData(second_digit);
				LCD_Cursor(32);
				LCD_WriteData(first_digit);
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
	unsigned char receive = 0x00;
	unsigned char device = 0x00;
	
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
				receive = USART_Receive(1);
				device = receive & 0xC0;
				receive = receive & 0x3F;
				
				if (device == CODE_DEVICE_GAME_PIECE) {
					if ((receive >= CODE_BUTTON1 & receive <= CODE_BUTTON5) || (receive >= CODE_KEYPAD0 & receive <= CODE_KEYPAD9)) {
						lcd_choice = CODE_DISPLAY_PUSH_IT;
					} else if (receive == CODE_SPINKNOB) {
						lcd_choice = CODE_DISPLAY_TWIST_IT;
					}
					
					if (isMessageChanging()) {
						array_position = 0;
					}
					bottom_lcd_choice = receive;
					
				} else if (device == CODE_DEVICE_LCD) {
					lcd_choice = receive;
					bottom_lcd_choice = receive;
					if (isMessageChanging()) {
						array_position = 0;
					}
				} else if (device == CODE_DEVICE_SEVENSEG) {
					sevendeg_choice = receive;
				} else if (device == CODE_DEVICE_GAME_COUNTER) {
					game_counter = receive;
				}			
			} 
			break;
		default: break;
	}
	return state;
}

/**
* get received message and show on seven seg display
*/
enum SM3_States { SM3_start };
int SMTick3(int state) {
	unsigned char const NUMBER_0 = 0x18;
	unsigned char const NUMBER_1 = 0x7B;
	unsigned char const NUMBER_2 = 0x2C;
	unsigned char const NUMBER_3 = 0x29;
	unsigned char const NUMBER_4 = 0x4B;
	unsigned char const NUMBER_5 = 0x89;
	unsigned char const NUMBER_6 = 0x88;
	unsigned char const NUMBER_7 = 0x3B;
	unsigned char const NUMBER_8 = 0x08;
	unsigned char const NUMBER_9 = 0x0B;
	unsigned char const NUMBER_A = 0x0A;
	unsigned char const NUMBER_C = 0x9C;
	unsigned char const NUMBER_E = 0x8C;
	unsigned char const NUMBER_F = 0x8A;
	unsigned char const NUMBER_DP = 0xF7;
	
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
			if (sevendeg_choice == 0)  {
				PORTA = NUMBER_0;
			} else if (sevendeg_choice == 0x01)  {
				PORTA = NUMBER_1;
			} else if (sevendeg_choice == 0x02)  {
				PORTA = NUMBER_2;
			} else if (sevendeg_choice == 0x03)  {
				PORTA = NUMBER_3;
			} else if (sevendeg_choice == 0x04)  {
				PORTA = NUMBER_4;
			} else if (sevendeg_choice == 0x05)  {
				PORTA = NUMBER_5;
			} else if (sevendeg_choice == 0x06)  {
				PORTA = NUMBER_6;
			} else if (sevendeg_choice == 0x07)  {
				PORTA = NUMBER_7;
			} else if (sevendeg_choice == 0x08)  {
				PORTA = NUMBER_8;
			} else if (sevendeg_choice == 0x09)  {
				PORTA = NUMBER_9;
			} else if (sevendeg_choice == 0x0A)  {
				PORTA = NUMBER_A;
			} else if (sevendeg_choice == 0x0C)  {
				PORTA = NUMBER_C;
			} else if (sevendeg_choice == 0x0E)  {
				PORTA = NUMBER_E;
			} else if (sevendeg_choice == 0x0F)  {
				PORTA = NUMBER_F;
			} else {
				PORTA = NUMBER_DP;
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
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	// Period for the tasks
	unsigned long int SMTick1_calc = 500;
	unsigned long int SMTick2_calc = 1;
	unsigned long int SMTick3_calc = 10;
	
	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);
	tmpGCD = findGCD(tmpGCD, SMTick3_calc);
	
	//Greatest common divisor for all tasks
	// or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;
	
	//Recalculate GCD periods for scheduler
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick2_period = SMTick2_calc/GCD;
	unsigned long int SMTick3_period = SMTick3_calc/GCD;

	//Declare an array of tasks
	static task task1, task2, task3;
	task *tasks[] = { &task1 , &task2, &task3 };
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
	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();
	// Scheduler for-loop iterator
	unsigned short i;
	LCD_init();
	LCD_ClearScreen();
	
	initUSART(1);
	USART_Flush(1);
	
	//start with welcome
	sevendeg_choice = 0xFF;
	lcd_choice = CODE_DISPLAY_WELCOME;
	
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