/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: simon_code.c
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Functions to record lights coming from the simon
* then push the correct buttons to play the game
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        10/5/2014               SECON 2015
*********************************************************************/

#include "simon_code.h"

/////////////////////////////////////////////// 
//
// Simon primitives
//
///////////////////////////////////////////////


/////////////////////////////////////////////// 
//
// Simon Usage
//
///////////////////////////////////////////////

void simon_init() {
    simon_retract_buttons();
}

// Retract function
void simon_retract_button(buttonID button) {
    switch (button) {
        case YELLOW_BUTTON:
            turn_servo_by_pulse(SIMON_YELLOW, YELLOW_RETRACT);
            break;
        case BLUE_BUTTON:
            turn_servo_by_pulse(SIMON_BLUE, BLUE_RETRACT);
            break;
        case RED_BUTTON:
            turn_servo_by_pulse(SIMON_RED, RED_RETRACT);
            break;
        case GREEN_BUTTON:
            turn_servo_by_pulse(SIMON_GREEN, GREEN_RETRACT);
            break;
        case START_BUTTON:
            turn_servo_by_pulse(SIMON_YELLOW, YELLOW_RETRACT);
            break;
        default:
            break;
    }
}

// Hover function
void simon_hover_button(buttonID button) {
    switch (button) {
        case YELLOW_BUTTON:
            turn_servo_by_pulse(SIMON_YELLOW, YELLOW_HOVER);
            break;
        case BLUE_BUTTON:
            turn_servo_by_pulse(SIMON_BLUE, BLUE_HOVER);
            break;
        case RED_BUTTON:
            turn_servo_by_pulse(SIMON_RED, RED_HOVER);
            break;
        case GREEN_BUTTON:
            turn_servo_by_pulse(SIMON_GREEN, GREEN_HOVER);
            break;
        case START_BUTTON:
            turn_servo_by_pulse(SIMON_YELLOW, YELLOW_HOVER);
            break;
        default:
            break;
    }
}

// Push function
void simon_push_button(buttonID button) {
    switch (button) {
        case YELLOW_BUTTON:
            turn_servo_by_pulse(SIMON_YELLOW, YELLOW_PUSH);
            break;
        case BLUE_BUTTON:
            turn_servo_by_pulse(SIMON_BLUE, BLUE_PUSH);
            break;
        case RED_BUTTON:
            turn_servo_by_pulse(SIMON_RED, RED_PUSH);
            break;
        case GREEN_BUTTON:
            turn_servo_by_pulse(SIMON_GREEN, GREEN_PUSH);
            break;
        case START_BUTTON:
            turn_servo_by_pulse(SIMON_YELLOW, YELLOW_PUSH);
            break;
        default:
            break;
    }
}

// Push and hover
void simon_push_and_hover_button(buttonID button) {
    simon_push_button(button);
    DELAY_MS(PUSH_WAIT);
    simon_hover_button(button);
}

// Retract all
void simon_retract_buttons() {
    simon_retract_button(YELLOW_BUTTON);
    simon_retract_button(BLUE_BUTTON);
    simon_retract_button(RED_BUTTON);
    simon_retract_button(GREEN_BUTTON);
}

// Hover all
void simon_hover_buttons() {
    simon_hover_button(YELLOW_BUTTON);
    simon_hover_button(BLUE_BUTTON);
    simon_hover_button(RED_BUTTON);
    simon_hover_button(GREEN_BUTTON);
}

// Push all
void simon_push_buttons() {
    simon_push_button(YELLOW_BUTTON);
    simon_push_button(BLUE_BUTTON);
    simon_push_button(RED_BUTTON);
    simon_push_button(GREEN_BUTTON);
}

// Push and hover all
void simon_push_and_hover_buttons() {
    simon_push_and_hover_button(YELLOW_BUTTON);
    simon_push_and_hover_button(BLUE_BUTTON);
    simon_push_and_hover_button(RED_BUTTON);
    simon_push_and_hover_button(GREEN_BUTTON);
}

