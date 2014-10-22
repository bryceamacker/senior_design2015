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
#include <stdio.h>

uint8_t u8_yellowLightPin = 3;
uint8_t u8_blueLightPin = 2;
uint8_t u8_redLightPin = 0;
uint8_t u8_greenLightPin = 1;

int16_t i16_lowestYellowValue = 0;
int16_t i16_lowestBlueValue = 0;
int16_t i16_lowestRedValue = 0;
int16_t i16_lowestGreenValue = 0;

int16_t i16_currentYellowValue = 0;
int16_t i16_currentBlueValue = 0;
int16_t i16_currentRedValue = 0;
int16_t i16_currentGreenValue = 0;

int16_t i16_yellowDifference = 0;
int16_t i16_blueDifference = 0;
int16_t i16_redDifference = 0;
int16_t i16_greenDifference = 0;

int16_t i16_yellowOnValue = 0;
int16_t i16_blueOnValue = 0;
int16_t i16_redOnValue = 0;
int16_t i16_greenOnValue = 0;

uint8_t au8_buttonArray[8];
uint8_t u8_roundNum = 1;
uint8_t u8_firstRun = 0;
uint8_t u8_button;

/////////////////////////////////////////////// 
//
// Simon usage
//
///////////////////////////////////////////////

void simon_init() {
    u8_firstRun = 1;
    simon_retract_buttons();
}

void prepare_to_play() {
    simon_hover_buttons();
    // calibrate_sensors(); I don't think we need calibration with the new changes, but this it where it goes if needed
    DELAY_MS(1000);
}

void play_simon() {
    prepare_to_play();
    while (u8_roundNum < 8) {
        #ifdef DEBUG
        printf("\nROUND %i\n", u8_roundNum);
        #endif DEBUG

        if (u8_firstRun == 1) {
            simon_push_button(YELLOW_BUTTON);
            find_color(u8_roundNum);
            simon_hover_button(YELLOW_BUTTON);
            DELAY_MS(250);
            play_buttons(u8_roundNum);
            u8_firstRun = 0;
        } else {
            find_color(u8_roundNum);
            play_buttons(u8_roundNum);
        }
        u8_roundNum++;
    }
}

/////////////////////////////////////////////// 
//
// Simon primitives
//
///////////////////////////////////////////////

void calibrate_sensors() {
    uint8_t i;
    int16_t i16_tempYellowValue = read_photo_transistor(YELLOW_TRANS);
    int16_t i16_tempBlueValue = read_photo_transistor(BLUE_TRANS);
    int16_t i16_tempRedValue = read_photo_transistor(RED_TRANS);
    int16_t i16_tempGreenValue = adc_read(GREEN_LIGHT); 

    for (i = 0; i < 100; i++) {
        i16_tempYellowValue = read_photo_transistor(YELLOW_TRANS);
        i16_tempBlueValue = read_photo_transistor(BLUE_TRANS);
        i16_tempRedValue = read_photo_transistor(RED_TRANS);
        i16_tempGreenValue = read_photo_transistor(GREEN_TRANS);

        if (i16_tempYellowValue < i16_lowestYellowValue) {
            i16_lowestYellowValue = i16_tempYellowValue;
        }
        if (i16_tempBlueValue < i16_lowestBlueValue) {
            i16_lowestBlueValue = i16_tempBlueValue;
        }
        if (i16_tempRedValue < i16_lowestRedValue) {
            i16_lowestRedValue = i16_tempRedValue;
        }
        if (i16_tempGreenValue < i16_lowestGreenValue) {
            i16_lowestGreenValue = i16_tempGreenValue;
        }
        DELAY_MS(10);
    }
    #ifdef DEBUG
    printf("Calibrated\n");
    printf("Yellow: %i\n", i16_lowestYellowValue);
    printf("Blue: %i\n", i16_lowestBlueValue);
    printf("Red: %i\n", i16_lowestRedValue);
    printf("Green: %i\n", i16_lowestGreenValue);
    printf("\n");
    #endif
}

void play_buttons(uint8_t u8_num) {
    uint8_t i;

    DELAY_MS(500);

    for(i = 0; i < u8_num; i++) {
        printf ("Button: %i\n", i+1);
        if (au8_buttonArray[i] == YELLOW_BUTTON) {
            #ifdef DEBUG
            printf("Pressing Yellow\n");
            #endif

            DELAY_MS(500);
            simon_push_button(YELLOW_BUTTON);
            i16_yellowOnValue = confirm_color(YELLOW_BUTTON);
            simon_hover_button(YELLOW_BUTTON);
            confirm_color_off(YELLOW_BUTTON, i16_yellowOnValue);
        } 
        else if (au8_buttonArray[i] == BLUE_BUTTON) {
            #ifdef DEBUG
            printf("Pressing Blue\n");
            #endif

            DELAY_MS(500);
            simon_push_button(BLUE_BUTTON);
            i16_blueOnValue = confirm_color(BLUE_BUTTON);
            simon_hover_button(BLUE_BUTTON);
            confirm_color_off(BLUE_BUTTON, i16_blueOnValue);
        } 
        else if (au8_buttonArray[i] == RED_BUTTON) {
            #ifdef DEBUG
            printf("Pressing Red\n");
            #endif

            DELAY_MS(500);
            simon_push_button(RED_BUTTON);
            i16_redOnValue = confirm_color(RED_BUTTON);
            simon_hover_button(RED_BUTTON);
            confirm_color_off(RED_BUTTON, i16_redOnValue);
        }
        else if (au8_buttonArray[i] == GREEN_BUTTON) {
            #ifdef DEBUG
            printf("Pressing Green\n");
            #endif

            DELAY_MS(500);
            simon_push_button(GREEN_BUTTON);
            i16_greenOnValue = confirm_color(GREEN_BUTTON);
            simon_hover_button(GREEN_BUTTON);
            confirm_color_off(GREEN_BUTTON, i16_greenOnValue);
        } 
    }
}


void find_color(uint8_t u8_numberOfButtons) {
    // Attempt to identify color for 200 milliseconds each
    uint8_t i;
    uint8_t u8_color;
    uint8_t u8_detectedButtons;

    u8_color = YELLOW_BUTTON;
    u8_detectedButtons = 0;
    do {
        i16_currentYellowValue = read_photo_transistor(YELLOW_TRANS);
        i16_currentBlueValue = read_photo_transistor(BLUE_TRANS);
        i16_currentRedValue = read_photo_transistor(RED_TRANS);
        i16_currentGreenValue = read_photo_transistor(GREEN_TRANS);

        i16_yellowDifference = i16_currentYellowValue - i16_lowestYellowValue;
        i16_blueDifference = i16_currentBlueValue - i16_lowestBlueValue;
        i16_redDifference = i16_currentRedValue - i16_lowestRedValue;
        i16_greenDifference = i16_currentGreenValue - i16_lowestGreenValue;

        if (i16_yellowDifference > YELLOW_LIGHT_THRESH_HOLD)
        {
            #ifdef DEBUG
            printf("Detected Yellow: %i\n", i16_currentYellowValue);
            #endif

            au8_buttonArray[u8_detectedButtons] = YELLOW_BUTTON;
            u8_detectedButtons++;
            DELAY_MS(50);
            u8_color = YELLOW_BUTTON;
            confirm_color_off(YELLOW_BUTTON, read_photo_transistor(YELLOW_TRANS));
        }
        else if (i16_blueDifference > BLUE_LIGHT_THRESH_HOLD)
        {
            #ifdef DEBUG
            printf("Detected Blue: %i\n", i16_currentBlueValue);
            #endif

            au8_buttonArray[u8_detectedButtons] = BLUE_BUTTON;
            u8_detectedButtons++;            
            DELAY_MS(50);
            u8_color = BLUE_BUTTON;
            confirm_color_off(BLUE_BUTTON, read_photo_transistor(BLUE_TRANS));
        }
        else if (i16_redDifference > RED_LIGHT_THRESH_HOLD)
        {
            #ifdef DEBUG
            printf("Detected Red: %i\n", i16_currentRedValue);
            #endif

            au8_buttonArray[u8_detectedButtons] = RED_BUTTON;
            u8_detectedButtons++;
            DELAY_MS(50);
            u8_color = RED_BUTTON;
            confirm_color_off(RED_BUTTON, read_photo_transistor(RED_TRANS));
        }
        else if (i16_greenDifference > GREEN_LIGHT_THRESH_HOLD)
        {
            #ifdef DEBUG
            printf("Detected Green: %i\n", i16_currentGreenValue);
            #endif

            au8_buttonArray[u8_detectedButtons] = GREEN_BUTTON;
            u8_detectedButtons++;
            DELAY_MS(50);
            u8_color = GREEN_BUTTON;
            confirm_color_off(GREEN_BUTTON, read_photo_transistor(GREEN_TRANS));
        }
        //short DELAY_MS for faster response to light.
        DELAY_MS(10); 
    } while(u8_detectedButtons < u8_numberOfButtons);

    printf ("\nOur buttons\n");

    #ifdef DEBUG
    for (i = 0; i < u8_numberOfButtons; i++) {
        if (au8_buttonArray[i] == YELLOW_BUTTON){
            printf("Button %i: Yellow\n", i+1);
        }
        else if (au8_buttonArray[i] == BLUE_BUTTON){
            printf("Button %i: Blue\n", i+1);
        }
        else if (au8_buttonArray[i] == RED_BUTTON){
            printf("Button %i: Red\n", i+1);
        }
        else if (au8_buttonArray[i] == GREEN_BUTTON){
            printf("Button %i: Green\n", i+1);
        }
    }
    printf("\n");
    #endif
}

uint8_t confirm_color(uint8_t u8_color) {
    // Attempt to identify color for 200 milliseconds each
    while(1) {
        i16_currentYellowValue = read_photo_transistor(YELLOW_TRANS);
        i16_currentBlueValue = read_photo_transistor(BLUE_TRANS);
        i16_currentRedValue = read_photo_transistor(RED_TRANS);
        i16_currentGreenValue = read_photo_transistor(GREEN_TRANS);
        
        i16_yellowDifference = i16_currentYellowValue - i16_lowestYellowValue;
        i16_blueDifference = i16_currentBlueValue - i16_lowestBlueValue;
        i16_redDifference = i16_currentRedValue - i16_lowestRedValue;
        i16_greenDifference = i16_currentGreenValue - i16_lowestGreenValue;
  
        if (i16_yellowDifference > YELLOW_LIGHT_THRESH_HOLD && u8_color == YELLOW_BUTTON)
        {
            #ifdef DEBUG
            printf("Yellow on: %i\n", i16_currentYellowValue);
            #endif
            return i16_currentYellowValue;
        }
        else if (i16_blueDifference > BLUE_LIGHT_THRESH_HOLD && u8_color == BLUE_BUTTON)
        {
            #ifdef DEBUG
            printf("Blue on: %i\n", i16_currentBlueValue);
            #endif
            return i16_currentBlueValue;
        }      
        else if (i16_redDifference > RED_LIGHT_THRESH_HOLD && u8_color == RED_BUTTON)
        {
            #ifdef DEBUG
            printf("Red on: %i\n", i16_currentRedValue);
            #endif
            return i16_currentRedValue;
        }
        else if (i16_greenDifference > GREEN_LIGHT_THRESH_HOLD && u8_color == GREEN_BUTTON)
        {
            #ifdef DEBUG
            printf("Green on: %i\n", i16_currentGreenValue);
            #endif
            return i16_currentGreenValue;
        }

        // Short DELAY_MS for faster response to light.
        DELAY_MS(10);
    }
}

uint8_t confirm_color_off(uint8_t u8_color, int16_t i16_onValue) {
    // Attempt to identify color for 200 milliseconds each
    #ifdef DEBUG
    printf("Waiting to turn off\n");
    #endif
    while(1) {
        i16_currentYellowValue = read_photo_transistor(YELLOW_TRANS);
        i16_currentBlueValue = read_photo_transistor(BLUE_TRANS);
        i16_currentRedValue = read_photo_transistor(RED_TRANS);
        i16_currentGreenValue = read_photo_transistor(GREEN_TRANS);

        i16_yellowDifference = i16_onValue - i16_currentYellowValue;
        i16_blueDifference = i16_onValue - i16_currentBlueValue;
        i16_redDifference = i16_onValue - i16_currentRedValue;
        i16_greenDifference = i16_onValue - i16_currentGreenValue;


        if (i16_currentYellowValue < (i16_lowestYellowValue + 20) && u8_color == YELLOW_BUTTON)
        {
            #ifdef DEBUG
            printf("Yellow off: %i\n", i16_currentYellowValue);
            #endif
            return 1;
        }
        else if (i16_currentBlueValue < (i16_lowestBlueValue + 20) && u8_color == BLUE_BUTTON)
        {
            #ifdef DEBUG
            printf("Blue off: %i\n", i16_currentBlueValue);
            #endif
            return 1;
        }      
        else if (i16_currentRedValue < (i16_lowestRedValue + 20) && u8_color == RED_BUTTON)
        {
            #ifdef DEBUG
            printf("Red off: %i\n", i16_currentRedValue);
            #endif
            return 1;
        }
        else if (i16_currentGreenValue < (i16_lowestGreenValue + 20) && u8_color == GREEN_BUTTON)
        {
            #ifdef DEBUG
            printf("Green off: %i\n", i16_currentGreenValue);
            #endif
            return 1;
        }

        // Short DELAY_MS for faster response to light.
        DELAY_MS(10);
    }
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

