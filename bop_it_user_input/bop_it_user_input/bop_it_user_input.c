/*
* Michael Brevard mbrev001@ucr.edu:
* Lab Section 022:
* Assignment: Final Project. Bop It (user input controller)
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include "usart_ATmega1284.h"
#include "Utilities.h"
#include "Codes.h"
#include "io.h"

int main(void) {
	
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	//DDRC = 0xFF; PORTC = 0x00; 
	DDRD = 0xFF; PORTD = 0x00; 
	
	unsigned char button1 = 0x00;
	unsigned char button2 = 0x00;
	unsigned char button3 = 0x00;
	unsigned char button4 = 0x00;
	unsigned char button5 = 0x00;
	unsigned char spin_knob = 0x00;
	initUSART(0);
	USART_Flush(0);
	
    while(1)
    {
		button1 = GetBit(~PINB, 0);
		button2 = GetBit(~PINB, 1);
		button3 = GetBit(~PINB, 2);
		button4 = GetBit(~PINB, 3);
		button5 = GetBit(~PINB, 4);
		spin_knob = GetBit(PINA, 2);

		if (button1 && USART_IsSendReady(0)) {
			if (USART_IsSendReady(0)) {
				USART_Send(CODE_BUTTON1, 0);
			}
		} else if (button2 && USART_IsSendReady(0)) {
			if (USART_IsSendReady(0)) {
				USART_Send(CODE_BUTTON2, 0);
			}
		} else if (button3 && USART_IsSendReady(0)) {
			if (USART_IsSendReady(0)) {
				USART_Send(CODE_BUTTON3, 0);
			}
		} else if (button4 && USART_IsSendReady(0)) {
			if (USART_IsSendReady(0)) {
				USART_Send(CODE_BUTTON4, 0);
			}
		} else if (button5 && USART_IsSendReady(0)) {
			if (USART_IsSendReady(0)) {
				USART_Send(CODE_BUTTON5, 0);
			}
		} else if (spin_knob && USART_IsSendReady(0)) {
			if (USART_IsSendReady(0)) {
				USART_Send(CODE_SPINKNOB, 0);
			}
		} else if (USART_IsSendReady(0)) {
			if (USART_IsSendReady(0)) {
				USART_Send(CODE_NOTHING, 0);
			}
		}			
    }
}