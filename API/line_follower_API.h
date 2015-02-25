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
#include "static_course_info.h"

#ifdef DEBUG_BUILD
#include <stdio.h>
#endif

#ifndef _LINE_FOLLOWER_API_H_
#define _LINE_FOLLOWER_API_H_

#define NUM_OF_REQUIRED_DETECTIONS      80

/**
 * @brief Initialize everything it takes to follow a line using a sensor array
 */
void line_follower_init(void);

/**
 * @brief Gets values from the sensor array representing a line
 *
 * @param pau16_sensorValues The values of the triple hi res sensors
 * @return A floating point representation of the line
 */
 float get_line(uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]);

/**
 * @brief Follow  line until the line sensor sees a box
 *
 * @param f_maxSpeed the max speed the robot will move at, specified by duty cycle
 */
void follow_line_to_box(float f_maxSpeed);

/**
 * @brief Follow until the robot gets back to wehre it branched from
 *
 * @param f_maxSpeed the max speed the robot will move at, specified by duty cycle
 */
void follow_line_back_to_main_line(float f_maxSpeed);

/**
 * @brief Recenter the robot over the line while moving forward
 *
 * @param f_maxSpeed Max speed the robot will travel
 * @param pau16_sensorValues Values of the triple hi res sensor array
**/
void correct_line_error(float f_maxSpeed, uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]);

/**
 * @brief Recenter the robot over the line while moving reverse
 *
 * @param f_maxSpeed Max speed the robot will travel
 * @param pau16_sensorValues Values of the triple hi res sensor array
**/
void correct_line_error_reverse(float f_maxSpeed, uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]);

/**
 * @brief Determine whether or not the robot has reached a box
 *
 * @param pau16_sensorValues The values of the triple hi res sensors
 * @return Whether or not there is a box
**/
uint8_t check_for_box(uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]);

/**
 * @brief Determine whether or not the robot has reached a left turn
 *
 * @param pau16_sensorValues The values of the triple hi res sensors
 * @return Whether or not there is a left turn
**/
uint8_t check_for_left_turn(uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]);

/**
 * @brief Handle a right angle left turn
**/
void handle_left_turn(void);

/**
 * @brief Handle a reverse right angle left turn
**/
void handle_reverse_left_turn(uint8_t u8_final);

/**
* @brief Determine whether or not the robot has reached a right turn
*
* @param pau16_sensorValues The values of the triple hi res sensors
* @return Whether or not there is a right turn
**/
uint8_t check_for_right_turn(uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]);

/**
 * @brief Handle a right angle right turn
**/
void handle_right_turn(void);

/**
 * @brief Handle a reverse right angle right turn
**/
void handle_reverse_right_turn(uint8_t u8_final);

/**
 * @brief Check for a line, used when turning to see if we're branching from the main line
**/
uint8_t check_for_line(uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]);

#endif
