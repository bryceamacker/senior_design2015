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

#define BUFFSIZE 64

typedef enum {
    PLAY_SIMON,
    PLAY_ETCH,
    PLAY_RUBIK,
    PLAY_CARDS,
    IDLE
} picGamePlayerState;

typedef enum  {
    STATE_WAIT_FOR_ADDR,
    STATE_WAIT_FOR_WRITE_DATA,
    STATE_SEND_READ_DATA,
    STATE_SEND_READ_LAST
} STATE;


volatile char  sz_i2cInString[BUFFSIZE+1];
volatile char sz_currentStateString[BUFFSIZE];
volatile uint16_t u16_index;
volatile picGamePlayerState st_picState;
volatile STATE e_mystate = STATE_WAIT_FOR_ADDR;

/**
 * @brief Handles I2C commands
 * 
 * @param  psz_s1 I2C in string
 */
void I2C_check_command(volatile char *psz_s1);

/**
 * @brief Determines the size of a given string
 * 
 * @param psz_1 The string to be measured
 * @return the length of the string
 */
int16_t getStringLength(char* psz_1);

#endif