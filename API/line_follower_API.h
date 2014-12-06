/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: line_follower_API.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Uses a sensor array and two DC motors to follow
* a line
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Bryce Amacker         10/01/2014              SECON 2015
*********************************************************************/

#include "pic24_all.h"
#include "sensor_array_API.h"
#include "motors_API.h"
#include <stdint.h>

#ifdef DEBUG_BUILD
#include <stdio.h>
#endif

#ifndef _LINE_FOLLOWER_API_H_
#define _LINE_FOLLOWER_API_H_

/**
 * @brief Initialize everything it takes to follow a line using a sensor array
 */
void line_follower_init(void);

/**
 * @brief Gets values from the sensor array representing a line
 * 
 * @param pau16_sensorValues Contains the values from the sensor array
 * @return A floating point representation of the line
 */
float get_line(uint16_t* pau16_sensorValues);

/**
 * @brief Follow  line until the line sensor sees a box
 * 
 * @param f_maxSpeed the max speed the robot will move at, specified by duty cycle
 */
void follow_line_to_box(float f_maxSpeed);

/**
 * @brief Follow a line
 * 
 * @param pau16_sensorValues The current sensor array values
 * @param f_maxSpeed the max speed to move the robot
 */
void follow_line(uint16_t* pau16_sensorValues, float f_maxSpeed);

/**
 * @brief Follow a line back
 * 
 * @param pau16_sensorValues The current sensor array values
 * @param f_maxSpeed the max speed to move the robot
 */
void follow_line_back(uint16_t* pau16_sensorValues, float f_maxSpeed);

/**
 * @brief Print the sensor array values
 * 
 * @param pau16_sensorValues The array values
 */
void print_line_data(uint16_t* pau16_sensorValues);

#endif