/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: game_arm_control.h
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

#include "servos_API.h"
#ifdef DEBUG_BUILD
#include <stdio.h>
#endif

#ifndef GAME_ARM_CONTROL_H_
#define GAME_ARM_CONTROL_H_

#define ARM_POSITION_UP             1500
#define ARM_POSITION_DOWN           550

#define ARM_SLIDE_LEFT_BACK         1450
#define ARM_SLIDE_LEFT_FORWARD      700
#define ARM_SLIDE_LEFT_DIFFERENCE   (ARM_SLIDE_LEFT_BACK - ARM_SLIDE_LEFT_FORWARD)

#define ARM_SLIDE_RIGHT_BACK        1350
#define ARM_SLIDE_RIGHT_FORWARD     2100
#define ARM_SLIDE_RIGHT_DIFFERENCE  (ARM_SLIDE_RIGHT_FORWARD - ARM_SLIDE_RIGHT_BACK)

#define ARM_WAIT                    1000

///////////////////////////////////////////////
//
// Game arm config
//
///////////////////////////////////////////////
/**
 * @brief initialize the arm that pulls in all the games
 */
void game_arm_init(void);

///////////////////////////////////////////////
//
// Game arm usage
//
///////////////////////////////////////////////
/**
 * @brief Lets go of whatever game currently in the robot, and resets back to the inital position
 * @details [long description]
 */
void game_arm_release(void);

/**
 * @brief Pulls in the Simon toy
 */
void game_arm_pull_simon(void);

/**
 * @brief Pulls in the Rubiks cube
 * @details [long description]
 */
void game_arm_pull_rubiks(void);

/**
 * @brief Pulls in the Etch-a-Sketch before the platform is lowered
 */
void game_arm_pull_etch(void);

/**
 * @brief Holds in the Etch-a-Sketch after the platform is lowered
 */
void game_arm_hold_etch(void);

/**
 * @brief Lower the arm to the ground safely
 */
void game_arm_lower(void);

/**
 * @brief Raise the arm up
 */
void game_arm_raise(void);

/**
 * @brief Slide game arm all the way forward
 */
void game_arm_slide_forward(void);

/**
 * @brief Slide game arm all the way back
 */
void game_arm_slide_back(void);

void game_arm_slide(uint8_t u8_percentage);


#endif
