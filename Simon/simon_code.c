#include "simon_code.h"

/////////////////////////////////////////////// 
//
// Simon primitives
//
///////////////////////////////////////////////


// Push functions
void simon_push_yellow_button() {
    turn_servo_by_pulse(SIMON_YELLOW, YELLOW_PUSH);
}

void simon_push_blue_button() {
    turn_servo_by_pulse(SIMON_BLUE, BLUE_PUSH);
}

void simon_push_red_button() {
    turn_servo_by_pulse(SIMON_RED, RED_PUSH);
}

void simon_push_green_button() {
    turn_servo_by_pulse(SIMON_GREEN, GREEN_PUSH);
}

// Hover functions
void simon_hover_yellow_button() {
    turn_servo_by_pulse(SIMON_YELLOW, YELLOW_HOVER);
}

void simon_hover_blue_button() {
    turn_servo_by_pulse(SIMON_BLUE, BLUE_HOVER);
}

void simon_hover_red_button() {
    turn_servo_by_pulse(SIMON_RED, RED_HOVER);
}

void simon_hover_green_button() {
    turn_servo_by_pulse(SIMON_GREEN, GREEN_HOVER);
}

// Retract functions
void simon_retract_yellow() {
    turn_servo_by_pulse(SIMON_YELLOW, YELLOW_RETRACT);
}

void simon_retract_blue() {
    turn_servo_by_pulse(SIMON_BLUE, BLUE_RETRACT);
}

void simon_retract_red() {
    turn_servo_by_pulse(SIMON_RED, RED_RETRACT);
}

void simon_retract_green() {
    turn_servo_by_pulse(SIMON_GREEN, GREEN_RETRACT);
}

/////////////////////////////////////////////// 
//
// Simon Usage
//
///////////////////////////////////////////////

void simon_init() {
    simon_retract_buttons();
}

void simon_push_button(buttonID button) {
    switch(button) {
        case YELLOW_BUTTON:
            simon_push_yellow_button();
            DELAY_MS(PUSH_WAIT);
            simon_hover_buttons();
            break;
        case BLUE_BUTTON:
            simon_push_blue_button();
            DELAY_MS(PUSH_WAIT);
            simon_hover_buttons();
            break;
        case RED_BUTTON:
            simon_push_red_button();
            DELAY_MS(PUSH_WAIT);
            simon_hover_buttons();
            break;
        case GREEN_BUTTON:
            simon_push_green_button();
            DELAY_MS(PUSH_WAIT);
            simon_hover_buttons();
            break;
        case START_BUTTON:
            simon_push_yellow_button();
            DELAY_MS(PUSH_WAIT);
            simon_hover_buttons();
            break;
        default:
            simon_push_yellow_button();
            DELAY_MS(PUSH_WAIT);
            simon_hover_buttons();
            break;
    }
}

void simon_hover_buttons(void) {
    simon_hover_yellow_button();
    simon_hover_blue_button();
    simon_hover_red_button();
    simon_hover_green_button();
}

void simon_retract_buttons(void) {
    simon_retract_yellow();
    simon_retract_blue();
    simon_retract_red();
    simon_retract_green();
}
