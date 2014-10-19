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

uint8_t u8_redLightPin = 0;  //define a pin for Photo resistor
uint8_t u8_greenLightPin = 1;
uint8_t u8_blueLightPin = 2;
uint8_t u8_yellowLightPin = 3;

uint8_t u8_lastRedValue = 0;
uint8_t u8_lastGreenValue = 0;
uint8_t u8_lastBlueValue = 0;
uint8_t u8_lastYellowValue = 0;

uint16_t u16_lowestRedValue = 1024;
uint16_t u16_lowestGreenValue = 1024;
uint16_t u16_lowestBlueValue = 1024;
uint16_t u16_lowestYellowValue = 1024;

uint16_t u16_currentRedValue = 0;
uint16_t u16_currentGreenValue = 0;
uint16_t u16_currentBlueValue = 0;
uint16_t u16_currentYellowValue = 0;

uint16_t u16_redDifference = 0;
uint16_t u16_greenDifference = 0;
uint16_t u16_blueDifference = 0;
uint16_t u16_yellowDifference = 0;

uint16_t u16_redOnValue = 0;
uint16_t u16_greenOnValue = 0;
uint16_t u16_blueOnValue = 0;
uint16_t u16_yellowOnValue = 0;

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
    calibrate_sensors();
}

void play_simon() {
    if (u8_firstRun == 1) {
        simon_push_and_hover_button(YELLOW_BUTTON);
        find_color(u8_roundNum);
    }
}

/////////////////////////////////////////////// 
//
// Simon primitives
//
///////////////////////////////////////////////

void calibrate_sensors() {
    uint8_t i;
    uint16_t u16_tempYellowValue; // NEED A VALUE = analogRead(yellowLightPin);
    uint16_t u16_tempBlueValue; // NEED A VALUE = analogRead(blueLightPin);
    uint16_t u16_tempRedValue; // NEED A VALUE = analogRead(redLightPin);
    uint16_t u16_tempGreenValue; // NEED A VALUE = analogRead(greenLightPin);

    u16_tempYellowValue = 0;
    u16_tempBlueValue = 0;
    u16_tempRedValue = 0;
    u16_tempGreenValue = 0;

    for (i = 0; i < 100; i++) {
        u16_tempYellowValue = 0; // NEED A VALUE = analogRead(yellowLightPin);
        u16_tempBlueValue = 0; // NEED A VALUE = analogRead(blueLightPin);
        u16_tempRedValue = 0; // NEED A VALUE = analogRead(redLightPin);
        u16_tempGreenValue = 0; // NEED A VALUE = analogRead(greenLightPin);

        printf("Yellow: %i", u16_tempYellowValue);
        printf("\tBlue: %i", u16_tempBlueValue);
        printf("\tRed : %i", u16_tempRedValue);
        printf("\tGreen: %i", u16_tempGreenValue);

        if (u16_tempYellowValue < u16_lowestYellowValue) {
            u16_lowestYellowValue = u16_tempYellowValue;
        }
        if (u16_tempBlueValue < u16_lowestBlueValue) {
            u16_lowestBlueValue = u16_tempBlueValue;
        }
        if (u16_tempRedValue < u16_lowestRedValue) {
            u16_lowestRedValue = u16_tempRedValue;
        }
        if (u16_tempGreenValue < u16_lowestGreenValue) {
            u16_lowestGreenValue = u16_tempGreenValue;
        }
        DELAY_MS(10);
    }
}

void play_buttons(uint8_t u8_num) {
    uint8_t i;

    DELAY_MS(500);
    printf("Number of buttons in array: %i", u8_num);

    for(i = 0; i < u8_num; i++) {
        if (au8_buttonArray[i] == YELLOW_BUTTON) {
            DELAY_MS(500);
            printf("Pressing Yello\n");
            simon_push_button(YELLOW_BUTTON);
            DELAY_MS(250);
            u16_yellowOnValue = confirmColor(YELLOW_BUTTON);
            simon_hover_button(YELLOW_BUTTON);
            confirmColorOff(YELLOW_BUTTON, u16_yellowOnValue);
        } 
        else if (au8_buttonArray[i] == BLUE_BUTTON) {
            DELAY_MS(500);
            printf("Pressing Blue\n");
            simon_push_button(BLUE_BUTTON);
            DELAY_MS(250);
            u16_blueOnValue = confirmColor(BLUE_BUTTON);
            simon_hover_button(BLUE_BUTTON);
            confirmColorOff(BLUE_BUTTON, u16_blueOnValue);
        } 
        else if (au8_buttonArray[i] == RED_BUTTON) {
            DELAY_MS(500);
            printf("Pressing Red\n");
            simon_push_button(RED_BUTTON);
            DELAY_MS(250);
            u16_redOnValue = confirmColor(RED_BUTTON);
            simon_hover_button(RED_BUTTON);
            confirmColorOff(RED_BUTTON, u16_redOnValue);
        }
        else if (au8_buttonArray[i] == GREEN_BUTTON) {
            DELAY_MS(500);
            printf("Pressing Green\n");
            simon_push_button(GREEN_BUTTON);
            DELAY_MS(250);
            u16_greenOnValue = confirmColor(GREEN_BUTTON);
            simon_hover_button(GREEN_BUTTON);
            confirmColorOff(GREEN_BUTTON, u16_greenOnValue);
        } 
    }
}


uint8_t find_color(uint8_t u8_numberOfButtons) {
    // Attempt to identify color for 200 milliseconds each
    uint8_t u8_color = YELLOW_BUTTON;
    uint8_t u8_detectedButtons = 0;
    do {
        u16_currentRedValue = 0; // NEED A VALUE = analogRead(redLightPin);
        u16_currentGreenValue = 0; // NEED A VALUE = analogRead(greenLightPin);
        u16_currentBlueValue = 0; // NEED A VALUE = analogRead(blueLightPin);  
        u16_currentYellowValue = 0; // NEED A VALUE = analogRead(yellowLightPin);

        u16_redDifference = u16_currentRedValue - u16_lowestRedValue;
        u16_greenDifference = u16_currentGreenValue - u16_lowestGreenValue;
        u16_blueDifference = u16_currentBlueValue - u16_lowestBlueValue;
        u16_yellowDifference = u16_currentYellowValue - u16_lowestYellowValue;
  
        if (u16_yellowDifference > 200)
        {
            au8_buttonArray[u8_detectedButtons] = YELLOW_BUTTON;
            u8_detectedButtons++;
            printf("Detected Yellow\n");
            u8_color = YELLOW_BUTTON;
            confirmColorOff(YELLOW_BUTTON, u16_currentYellowValue);
        }
        else if (u16_blueDifference > 200)
        {
            au8_buttonArray[u8_detectedButtons] = BLUE_BUTTON;
            u8_detectedButtons++;
            printf("Detected Blue\n");
            u8_color = BLUE_BUTTON;
            confirmColorOff(BLUE_BUTTON, u16_currentBlueValue);
        }
        else if (u16_redDifference > 250)
        {
            au8_buttonArray[u8_detectedButtons] = RED_BUTTON;
            u8_detectedButtons++;
            printf("Detected Red\n");
            u8_color = RED_BUTTON;
            confirmColorOff(RED_BUTTON, u16_currentRedValue);
        }
        else if (u16_greenDifference > 200)
        {
            au8_buttonArray[u8_detectedButtons] = GREEN_BUTTON;
            u8_detectedButtons++;
            printf("Detected Green\n");
            u8_color = GREEN_BUTTON;
            confirmColorOff(GREEN_BUTTON, u16_currentGreenValue);
        }
        //short DELAY_MS for faster response to light.
        DELAY_MS(10); 
    } while(u8_detectedButtons < u8_numberOfButtons);
}

uint8_t confirmColor(uint8_t u8_color) {
    // Attempt to identify color for 200 milliseconds each
    while(1) {
        u16_currentRedValue = 0; // NEED A VALUE = analogRead(redLightPin);
        u16_currentGreenValue = 0; // NEED A VALUE = analogRead(greenLightPin);
        u16_currentBlueValue = 0; // NEED A VALUE = analogRead(blueLightPin);  
        u16_currentYellowValue = 0; // NEED A VALUE = analogRead(yellowLightPin);
        
        u16_redDifference = u16_currentRedValue - u16_lowestRedValue;
        u16_greenDifference = u16_currentGreenValue - u16_lowestGreenValue;
        u16_blueDifference = u16_currentBlueValue - u16_lowestBlueValue;
        u16_yellowDifference = u16_currentYellowValue - u16_lowestYellowValue;
  
        if (u16_redDifference > 250 && u8_color == RED_BUTTON)
        {
            return u16_currentRedValue;
        }
        else if (u16_greenDifference > 200 && u8_color == GREEN_BUTTON)
        {
            return u16_currentGreenValue;
        }
        else if (u16_blueDifference > 200 && u8_color == BLUE_BUTTON)
        {
            return u16_currentBlueValue;
        }      
        else if (u16_yellowDifference > 200 && u8_color == YELLOW_BUTTON)
        {
            return u16_currentYellowValue;
        }

        // Short DELAY_MS for faster response to light.
        DELAY_MS(10);
    }
}

uint8_t confirmColorOff(uint8_t u8_color, uint16_t u16_onValue) {
    // Attempt to identify color for 200 milliseconds each
    while(1) {
        printf("Waiting to turn off\n");
        u16_currentRedValue = 0; // NEED A VALUE = analogRead(redLightPin);
        u16_currentGreenValue = 0; // NEED A VALUE = analogRead(greenLightPin);
        u16_currentBlueValue = 0; // NEED A VALUE = analogRead(blueLightPin);  
        u16_currentYellowValue = 0; // NEED A VALUE = analogRead(yellowLightPin);

        u16_redDifference = u16_onValue - u16_currentRedValue;
        u16_greenDifference = u16_onValue - u16_currentGreenValue;
        u16_blueDifference = u16_onValue - u16_currentBlueValue;
        u16_yellowDifference = u16_onValue - u16_currentYellowValue;
  
        if (u16_redDifference > 180 && u8_color == RED_BUTTON)
        {
            return 1;
        }
        else if (u16_greenDifference > 150 && u8_color == GREEN_BUTTON)
        {
            return 1;
        }
        else if (u16_blueDifference > 150 && u8_color == BLUE_BUTTON)
        {
            return 1;
        }      
        else if (u16_yellowDifference > 150 && u8_color == YELLOW_BUTTON)
        {
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

