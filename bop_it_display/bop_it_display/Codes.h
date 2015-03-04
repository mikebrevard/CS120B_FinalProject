/*
* Michael Brevard mbrev001@ucr.edu:
* Lab Section 022:
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#ifndef CODES_H_
#define CODES_H_

unsigned char const CODE_NOTHING = 0;

// buttons
unsigned char const CODE_BUTTON1 = 1; 
unsigned char const CODE_BUTTON2 = 2; 
unsigned char const CODE_BUTTON3 = 3; 
unsigned char const CODE_BUTTON4 = 4; 
unsigned char const CODE_BUTTON5 = 5; 

// spin knob
unsigned char const CODE_SPINKNOB = 6; 

// light sensor
unsigned char const CODE_LIGHTSENSOR = 7; 

// keypad
unsigned char const CODE_KEYPAD0 = 8;
unsigned char const CODE_KEYPAD1 = 9;
unsigned char const CODE_KEYPAD2 = 10;
unsigned char const CODE_KEYPAD3 = 11;
unsigned char const CODE_KEYPAD4 = 12;
unsigned char const CODE_KEYPAD5 = 13;
unsigned char const CODE_KEYPAD6 = 14;
unsigned char const CODE_KEYPAD7 = 15;
unsigned char const CODE_KEYPAD8 = 16;
unsigned char const CODE_KEYPAD9 = 17;

// game states
unsigned char const CODE_GAME_WELCOME = 18;
unsigned char const CODE_GAME_SCORES = 19;
unsigned char const CODE_GAME_PLAY = 20;

// game play codes
unsigned char const CODE_ANSWER_RIGHT = 21;
unsigned char const CODE_ANSWER_WRONG = 22;

// other catch
unsigned char const CODE_UNCAUGHT = 23;
unsigned char const CODE_BLANK = 24;

#endif /* CODES_H_ */