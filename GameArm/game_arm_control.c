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
    uint16_t u16_arm1Diff;
    uint16_t u16_arm2Diff;
    uint16_t u16_arm1Step;
    uint16_t u16_arm2Step;
    uint8_t u8_numSteps;
    // uint8_t u8_i;

    u8_numSteps = 80;

    if (DUAL_ARMS == 0) {
        turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_FORWARD);
    } else {
        u16_arm1Diff = ARM_SLIDE_FORWARD - ARM_SLIDE_BACK;
        u16_arm2Diff = ARM_SLIDE_BACK2 - ARM_SLIDE_FORWARD2;

        u16_arm1Step = u16_arm1Diff/u8_numSteps;
        u16_arm2Step = u16_arm2Diff/u8_numSteps;

        // for (u8_i=1;u8_i<=u8_numSteps;u8_i++) {
        //     turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_BACK + (u16_arm1Step*u8_i));
        //     turn_servo_by_pulse(ARM_SLIDE2, ARM_SLIDE_BACK2 - (u16_arm2Step*u8_i));
        //     DELAY_MS(ARM_SLIDE_DELAY);
        // }

        turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_FORWARD);
        turn_servo_by_pulse(ARM_SLIDE2, ARM_SLIDE_FORWARD2);
    }
    DELAY_MS(ARM_WAIT);
}

void game_arm_slide_back() {
    uint16_t u16_arm1Diff;
    uint16_t u16_arm2Diff;
    uint16_t u16_arm1Step;
    uint16_t u16_arm2Step;
    uint8_t u8_numSteps;
    // uint8_t u8_i;

    u8_numSteps = 80;

    if (DUAL_ARMS == 0) {
        turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_BACK);
    } else {
        u16_arm1Diff = ARM_SLIDE_FORWARD - ARM_SLIDE_BACK;
        u16_arm2Diff = ARM_SLIDE_BACK2 - ARM_SLIDE_FORWARD2;

        u16_arm1Step = u16_arm1Diff/u8_numSteps;
        u16_arm2Step = u16_arm2Diff/u8_numSteps;

        // for (u8_i=1;u8_i<=u8_numSteps;u8_i++) {
        //     turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_FORWARD - (u16_arm1Step*u8_i));
        //     turn_servo_by_pulse(ARM_SLIDE2, ARM_SLIDE_FORWARD2 + (u16_arm2Step*u8_i));
        //     DELAY_MS(ARM_SLIDE_DELAY);
        // }

        turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_BACK);
        turn_servo_by_pulse(ARM_SLIDE2, ARM_SLIDE_BACK2);
    }
    DELAY_MS(ARM_WAIT);
}
