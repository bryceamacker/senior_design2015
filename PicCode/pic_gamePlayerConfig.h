/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: pic_gamePlayerConfig.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Setup for the SECON 2015 game player PIC 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        9/20/2014               SECON 2015
*********************************************************************/

#include "pic24_all.h"
#include "motorsAPI.h"
#include "etch_code.h"
#include "rubiks_code.h"
#include "simon_code.h"
#include "card_code.h"
#include "platform_control.h"
#include "game_arm_control.h"

#ifdef DEBUG_BUILD
#include <stdio.h>
#endif

/**
 * @brief Initialize the game player pic
 */
void pic_game_player_init(void);

/**
 * @brief Handles serial commands
 * 
 * @param  u8_c command to be run
 */
void serial_command(uint8_t u8_c);

/**
 * @brief Print out serial menu
 */
void serial_menu(void);

/**
 * @brief Print out list of servos for indvidual control
 */
void servo_menu(void);

/**
 * @brief Print out list of Simon arms
 */
void simon_menu(void);

/**
 * @brief Control a specific servo 
 * 
 * @param u8_servo Servo to be controlled
 */
void set_servo(char u8_servo);