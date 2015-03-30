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

#define KP_DEFAULT      0.4 //0.2
#define KD_DEFAULT      7   //5

extern float KP;
extern float KD;

/**
 * @brief Initialize everything it takes to follow a line using a sensor array
 */
void line_follower_init(void);

/**
 * @brief Follow  line until the line sensor sees a box
 *
 * @param f_maxSpeed the max speed the robot will move at
 * @param u8_expectedTurn What type of turn to expect off of main line
 */
void follow_line_to_box(float f_maxSpeed, char u8_expectedTurn);

/**
 * @brief Follow  line until the line sensor sees a box using a PID scheme
 *
 * @param f_maxSpeed the max speed the robot will move at
 * @param u8_expectedTurn What type of turn to expect off of main line
 */
void follow_line_to_box_pid(float f_maxSpeed, char u8_expectedTurn);

/**
 * @brief Follow in reverse until the robot gets back to wehre it branched from
 *
 * @param f_maxSpeed the max speed the robot will move at
 */
void follow_line_back_to_main_line(float f_maxSpeed);

/**
 * @brief Follow in until the robot gets back to wehre it branched from
 *
 * @param f_maxSpeed the max speed the robot will move at
 */
void follow_line_back_to_main_line_reverse(float f_maxSpeed);

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
 * @brief Recenter the robot over the line while moving forward using a PID scheme
 *
 * @param f_maxSpeed Max speed the robot will travel
 * @param u8_direction Direction to move
**/
void correct_line_error_pid(float f_maxSpeed, uint8_t u8_direction);

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
 *
 * @param u8_curve Whether or not to use a curved turn
**/
void handle_left_turn(uint8_t u8_curve);

/**
 * @brief Handle a reverse right angle left turn
 *
 * @param u8_curve Whether or not to use a curved turn
**/
void handle_reverse_left_turn(uint8_t u8_curve);

/**
* @brief Determine whether or not the robot has reached a right turn
*
* @param pau16_sensorValues The values of the triple hi res sensors
* @return Whether or not there is a right turn
**/
uint8_t check_for_right_turn(uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]);

/**
 * @brief Handle a right angle right turn
 *
 * @param u8_curve Whether or not to use a curved turn
**/
void handle_right_turn(uint8_t u8_curve);

/**
 * @brief Handle a reverse right angle right turn
 *
 * @param u8_curve Whether or not to use a curved turn
**/
void handle_reverse_right_turn(uint8_t u8_curve);

/**
 * @brief Reverse until the robot gets back on a line
**/
void reverse_until_line();

/**
 * @brief Reverse until the robot gets back to a branch
**/
void reverse_until_branch();

/**
 * @brief Check for a line, used when turning to see if we're branching from the main line
**/
uint8_t check_for_line(uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]);

/**
 * @brief Set a new value for KP
 *
 * @param f_newValue The new value for KP
**/
void set_KP(float f_newValue);

void turn_until_line_centered(float f_speed, uint8_t u8_direction);

/**
 * @brief Set a new value for KD
 *
 * @param f_newValue The new value for KD
**/
void set_KD(float f_newValue);

#endif
