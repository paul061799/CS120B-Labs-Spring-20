/*	Author: pnguy086
 *  Partner(s) Name:
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

enum States{Start, Init, Press0_Release1, Press1_Release0, Press01, Reset} state;

unsigned char count = 0; //updated in transitions

void Tick() {
    switch(state){ //state transitions
        case Start:
            count = 0x07;
            state = Init;
            break;
        case Init:
            if(PINA == 0x01){
                state = Press0_Release1;
            } else if (PINA == 0x02) {
                state = Press1_Release0;
            } else if (PINA == 0x03) {
                state = Press01;
            } else {
                state = Init;
            }
            break;
        case Press0_Release1:
            if(PINA == 0x01){
                state = Press0_Release1;
            } else if (PINA == 0x02) {
                state = Press1_Release0;
            } else if (PINA == 0x03) {
                state = Press01;
            } else if (PINA == 0x00) {
                state = Reset;
            }
            break;
        case Press1_Release0:
            if(PINA == 0x01){
                state = Press0_Release1;
                if(PORTC < 0x09){
                    PORTC++;
                }
            } else if (PINA == 0x02) {
                state = Press1_Release0;
            } else if (PINA == 0x03) {
                state = Press01;
            } else if (PINA == 0x00) {
                state = Reset;
            }
            break;
        case Press01:
            if(PINA == 0x01){
                state = Press0_Release1;
            } else if (PINA == 0x02) {
                state = Press1_Release0;
            } else if (PINA == 0x03) {
                state = Press01;
            } else if (PINA == 0x00) {
                state = Reset;
            }
            break;
        case Reset:
            state = Init;
            break;
        default:
            printf("State Transition Error\n");
            break;
    } //state transitions

    switch(state){
        case Start:
        case Init:
        case Press01:
            break;
        case Press0_Release1:
            if(count > 9) { count++; }
        case Press1_Release0:
            if(count < 0) { count --; }
        case Reset:
            break;
        default:
            printf("State Action Error \n");
            break;
    }

    PORTC = count;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    /* Insert your solution below */
    state = Start;
    while (1) {
      Tick();
    }
    return 1;
}