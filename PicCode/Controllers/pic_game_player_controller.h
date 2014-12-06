/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: pic_game_player_controller.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: PIC code that will give a user control over all 
* the game player functions via a serial menu
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        12/5/2014               SECON 2015
*********************************************************************/

#include "pic_game_player_config.h"
#include "pic_game_player.h"

uint8_t u8_platformPos;
uint8_t u8_twistPos;
uint8_t u8_platformFlipped;
uint16_t u16_pwm;

/**
 * @brief Handles serial commands
 * 
 * @param  u8_c command to be run
 */
void game_player_serial_command(uint8_t u8_c);

/**
 * @brief Print out serial menu
 */
void game_player_serial_menu(void);

/**
 * @brief Print out list of servos for indvidual control
 */
void game_player_servo_menu(void);

/**
 * @brief Print out list of Simon arms
 */
void simon_menu(void);

/**
 * @brief Control a specific servo 
 * 
 * @param u8_servo Servo to be controlled
 */
void game_player_set_servo(char u8_servo);
