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
    turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_SIMON);
    DELAY_MS(ARM_WAIT);
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
    turn_servo_by_pulse(ARM_SLIDE, 1900);
    DELAY_MS(ARM_WAIT);
    turn_servo_by_pulse(ARM_SLIDE, 2100);
    DELAY_MS(ARM_WAIT);
    turn_servo_by_pulse(ARM_SLIDE, 1700);
    DELAY_MS(ARM_WAIT);
    turn_servo_by_pulse(ARM_SLIDE, 1800);
    DELAY_MS(ARM_WAIT);
    turn_servo_by_pulse(ARM_SLIDE, 1500);
    DELAY_MS(ARM_WAIT);
    turn_servo_by_pulse(ARM_SLIDE, 1700);
    DELAY_MS(ARM_WAIT);

    // Final pull on the rubiks
    turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_RUBIKS);
    DELAY_MS(ARM_WAIT);

    // Slide forward
    game_arm_slide_forward();

    // Raise the arm
    game_arm_raise();

    // Slide it back
    game_arm_slide_back();
}

void game_arm_pull_etch() {
    #ifdef DEBUG_BUILD
    printf("Grabbing Etch\n");
    #endif

    // Slide forward
    game_arm_slide_forward();

    // Put the arm down
    game_arm_lower();

    // Jostle it in
    turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_ETCH);
    DELAY_MS(ARM_WAIT);

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
    turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_ETCH);
    DELAY_MS(ARM_WAIT);
}

void game_arm_lower() {
    // Step down the arm so it's not too sudden
    turn_servo_by_pulse(ARM_POSITION, ARM_POSITION_DOWN_STEP_1);
    DELAY_MS(ARM_WAIT);
    turn_servo_by_pulse(ARM_POSITION, ARM_POSITION_DOWN_STEP_2);
    DELAY_MS(ARM_WAIT);
    turn_servo_by_pulse(ARM_POSITION, ARM_POSITION_DOWN_STEP_3);
    DELAY_MS(ARM_WAIT);
}

void game_arm_raise() {
    turn_servo_by_pulse(ARM_POSITION, ARM_POSITION_UP);
    DELAY_MS(ARM_WAIT);
}

void game_arm_slide_forward() {
    turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_FORWARD);
    DELAY_MS(ARM_WAIT);
}

void game_arm_slide_back() {
    turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_BACK);
    DELAY_MS(ARM_WAIT);
}
