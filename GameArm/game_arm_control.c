/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: game_arm_control.c
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Controller for the arm that grabs all the games
* and pulls them into the robot
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        11/5/2014               SECON 2015
*********************************************************************/

#include "game_arm_control.h"

///////////////////////////////////////////////
//
// Game arm config
//
///////////////////////////////////////////////

void game_arm_init() {
    // Slide forward
    game_arm_slide_forward();

    // Raise the arm
    game_arm_raise();

    // Slide it back
    game_arm_slide_back();
}

///////////////////////////////////////////////
//
// Game arm usage
//
///////////////////////////////////////////////

void game_arm_release() {
    #ifdef DEBUG_BUILD
    printf("Releasing Game\n");
    #endif

    // Slide forward
    game_arm_slide_forward();

    // Raise the arm
    game_arm_raise();

    // Slide it back
    game_arm_slide_back();
}

void game_arm_pull_simon() {
    #ifdef DEBUG_BUILD
    printf("Grabbing Simon\n");
    #endif

    // Slide forward
    game_arm_slide_forward();

    // Put the arm down
    game_arm_lower();

    // Grab the Simon
    game_arm_slide_back();

    // Release Simon, we don't need to hold it
    game_arm_release();
}

void game_arm_pull_rubiks() {
    #ifdef DEBUG_BUILD
    printf("Grabbing Rubiks\n");
    #endif

    // Slide forward
    game_arm_slide_forward();

    // Put the arm down
    game_arm_lower();

    // This will jostle the cube into the MFGP
    game_arm_slide(80);
    game_arm_slide(100);
    game_arm_slide(60);
    game_arm_slide(80);
    game_arm_slide(40);
    game_arm_slide(60);
    game_arm_slide(20);
    game_arm_slide(40);
    game_arm_slide(0);

    game_arm_release();
}

void game_arm_pull_etch() {
    #ifdef DEBUG_BUILD
    printf("Grabbing Etch\n");
    #endif

    // Slide forward
    game_arm_slide_forward();

    // Put the arm down
    game_arm_lower();

    // Grab Etch
    game_arm_slide(60);

    // Slide forward
    game_arm_slide_forward();

    // Raise the arm
    game_arm_raise();

    // Slide it back
    game_arm_slide_back();
}

void game_arm_hold_etch() {
    #ifdef DEBUG_BUILD
    printf("Holding Etch\n");
    #endif

    // Slide forward
    game_arm_slide_forward();

    // Put the arm down
    game_arm_lower();

    // Hold the etch
    game_arm_slide(80);
}

void game_arm_lower() {
    // Step down the arm so it's not too sudden
    turn_servo_by_pulse(ARM_POSITION, ARM_POSITION_DOWN);
    DELAY_MS(ARM_WAIT);
}

void game_arm_raise() {
    turn_servo_by_pulse(ARM_POSITION, ARM_POSITION_UP);
    DELAY_MS(ARM_WAIT);
}

void game_arm_slide_forward() {
    game_arm_slide(100);
}

void game_arm_slide_back() {
    game_arm_slide(0);
}

void game_arm_slide(uint8_t u8_percentage) {
    uint16_t u16_newLeftPulse;
    uint16_t u16_newRightPulse;
    uint16_t u16_leftPulseDifference;
    uint16_t u16_rightPulseDifference;

    u16_leftPulseDifference = (u8_percentage/100.0) * ARM_SLIDE_LEFT_DIFFERENCE;
    u16_rightPulseDifference = (u8_percentage/100.0) * ARM_SLIDE_RIGHT_DIFFERENCE;

    u16_newLeftPulse = ARM_SLIDE_LEFT_BACK - u16_leftPulseDifference;
    u16_newRightPulse = ARM_SLIDE_RIGHT_BACK + u16_rightPulseDifference;

    turn_servo_by_pulse(ARM_SLIDE_LEFT, u16_newLeftPulse);
    turn_servo_by_pulse(ARM_SLIDE_RIGHT, u16_newRightPulse);
    DELAY_MS(ARM_WAIT);
}
