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
    turn_servo_by_pulse(ARM_POSITION, ARM_POSITION_UP);
    DELAY_MS(250);
    turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_BACK);
}

/////////////////////////////////////////////// 
//
// Game arm usage
//
///////////////////////////////////////////////

void game_arm_release() {
    turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_FORWARD);
    DELAY_MS(250);
    turn_servo_by_pulse(ARM_POSITION, ARM_POSITION_UP);
    DELAY_MS(250);
    turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_BACK);
    DELAY_MS(ARM_GRAB_WAIT);
}

void game_arm_pull_simon() {
    turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_FORWARD);
    DELAY_MS(250);
    game_arm_lower();
    DELAY_MS(250);
    turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_SIMON);
    DELAY_MS(ARM_GRAB_WAIT);
}

void game_arm_pull_rubiks() {
    turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_FORWARD);
    DELAY_MS(500);
    game_arm_lower();
    DELAY_MS(250);
    // This will jostle the cube into the MFGP
    turn_servo_by_pulse(ARM_SLIDE, 1900);
    DELAY_MS(450);
    turn_servo_by_pulse(ARM_SLIDE, 2100);
    DELAY_MS(450);
    turn_servo_by_pulse(ARM_SLIDE, 1700);
    DELAY_MS(450);
    turn_servo_by_pulse(ARM_SLIDE, 1800);
    DELAY_MS(450);
    turn_servo_by_pulse(ARM_SLIDE, 1500);
    DELAY_MS(450);
    turn_servo_by_pulse(ARM_SLIDE, 1700);
    DELAY_MS(450);

    turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_RUBIKS);
    DELAY_MS(500);
    turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_FORWARD);
    DELAY_MS(500);
    turn_servo_by_pulse(ARM_POSITION, ARM_POSITION_UP);
    DELAY_MS(250);
    turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_BACK);
    DELAY_MS(ARM_GRAB_WAIT);
}

void game_arm_pull_etch() {
    turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_FORWARD);
    DELAY_MS(250);
    game_arm_lower();
    DELAY_MS(250);
    turn_servo_by_pulse(ARM_SLIDE, ARM_SLIDE_ETCH);
    DELAY_MS(ARM_GRAB_WAIT);
}

void game_arm_lower() {
    turn_servo_by_pulse(ARM_POSITION, ARM_POSITION_DOWN_STEP_1);
    DELAY_MS(250);
    turn_servo_by_pulse(ARM_POSITION, ARM_POSITION_DOWN_STEP_2);
    DELAY_MS(250);
    turn_servo_by_pulse(ARM_POSITION, ARM_POSITION_DOWN_STEP_3);
}