/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: pic_navigation.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: PIC that will use a sensor array and motors to 
* follow a line to a game then communicate to another PIC via I2C
* so that it can play the game
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        10/01/2014              SECON 2015
*********************************************************************/

#include "pic_navigation_config.h"
#include <string.h>

#ifdef DEBUG_BUILD
#include <stdio.h>
#warning "Motor Controlller: DEBUG BUILD"
#endif

#ifndef _PIC_MOTOR_CONTROLLER_H_
#define _PIC_MOTOR_CONTROLLER_H_

#define PIC_GAME_PLAYER_ADDR 0x20
#define BUFFSIZE 64

typedef enum {
    SIMON =     0,
    RUBIKS =    1,
    ETCH =      2,
    CARD =      3
} gameID;

char sz_recieveString[BUFFSIZE];

uint8_t u8_currentGame;

/**
 * @brief Tells the game player to play a game
 * @details Sends a command over I2C to the game player PIC
 * then waits until the game has been played and prints out
 * a success message
 * 
 * @param game The game to be played
 */
void play_game(gameID game);

#endif