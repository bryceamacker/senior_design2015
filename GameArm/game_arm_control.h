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

#include "servosAPI.h"

#ifndef GAME_ARM_CONTROL_H_
#define GAME_ARM_CONTROL_H_

#define ARM_POSITION_UP         1500
#define ARM_POSITION_DOWN       1500

#define ARM_SLIDE_BACK          1500
#define ARM_SLIDE_FORWARD       1500
#define ARM_SLIDE_SIMON         1500
#define ARM_SLIDE_RUBIKS        1500
#define ARM_SLIDE_ETCH          1500

#define ARM_GRAB_WAIT           1500

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
 * @brief Pulls in the Etch-a-Sketch
 */
void game_arm_pull_etch(void);

#endif