/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: pic.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Declarations and constants for the overall 
* functionality of the SECON 2015 robot.
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        9/20/2014               SECON 2015
*********************************************************************/

#include "pic_gamePlayerConfig.h"
#include <stdio.h>
#include <string.h>

#ifndef PIC_H_
#define PIC_H_


#define BUFSIZE 64

volatile char  sz_1[BUFSIZE+1];
volatile char  sz_2[BUFSIZE+1];
volatile uint16_t u16_index;

typedef enum {
    PLAY_SIMON      = 0,
    PLAY_ETCH       = 1,
    PLAY_RUBIK      = 2,
    PLAY_CARDS      = 3
} picTwoCommands_t;

typedef enum  {
  STATE_WAIT_FOR_ADDR,
  STATE_WAIT_FOR_WRITE_DATA,
  STATE_SEND_READ_DATA,
  STATE_SEND_READ_LAST
} STATE;

volatile STATE e_mystate = STATE_WAIT_FOR_ADDR;

/**
 * @brief Handles serial commands
 * 
 * @param  u8_c command to be run
 */
void serial_command(uint8_t u8_c);

/**
 * @brief Handles I2C commands
 * 
 * @param  psz_s1 I2C in string
 */
void I2C_check_command(volatile char *psz_s1);

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

#endif