/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName:
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

#include "pic_motorControllerConfig.h"
#include <string.h>

#ifndef _PIC_MOTOR_CONTROLLER_H_
#define _PIC_MOTOR_CONTROLLER_H_

#define PIC_GAME_PLAYER_ADDR 0x20

#define BUFFSIZE 64
char sz_sendString[BUFFSIZE];
char sz_recieveString[BUFFSIZE];

uint16_t pau16_sensorValues[SENSOR_NUM];
uint16_t u16_position;

int16_t i16_error;
int16_t i16_lineCenter;

uint8_t u8_detectingSensors;
uint8_t u8_foundObjective;

uint8_t u8_leftTurn;
uint8_t u8_rightTurn;

uint8_t u8_gameNumber;

uint8_t i;

/**
 * @brief Determines the size of a given string
 * 
 * @param psz_1 The string to be measured
 * @return the length of the string
 */
int16_t getStringLength(char* psz_1);

#endif