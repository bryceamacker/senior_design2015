/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: motorsAPI.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Function declerations and constants to help 
* control DC motors
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        9/20/2014               SECON 2015
*********************************************************************/

#include "pic24_all.h"
#include <stdio.h>
#include <stdbool.h>

#ifndef _MOTORS_API_H_
#define _MOTORS_API_H_

#define CONFIG_RIGHT_MOTOR_IN1()        CONFIG_RD1_AS_DIG_OUTPUT()
#define CONFIG_RIGHT_MOTOR_IN2()        CONFIG_RD2_AS_DIG_OUTPUT()

#define CONFIG_LEFT_MOTOR_IN1()         CONFIG_RD3_AS_DIG_OUTPUT()
#define CONFIG_LEFT_MOTOR_IN2()         CONFIG_RD4_AS_DIG_OUTPUT()

#define LIN1_PULSE                      OC2RS   // Pin D1
#define LIN2_PULSE                      OC3RS   // Pin D2
#define RIN1_PULSE                      OC4RS   // Pin D3
#define RIN2_PULSE                      OC5RS   // Pin D4

#define MOTOR_PWM_PERIOD 20000  // desired period, in us

/////////////////////////////////////////////// 
//
// Motor config
//
///////////////////////////////////////////////
/**
 * @brief Initializes every thing for the motors
 */
void motors_init();

/**
 * @brief Configs timer 2 to drive motors
 */
void config_motor_timer2(void);

/**
 * @brief Configs timer 3 to drive motors
 */
void config_motor_timer3(void);

/**
 * @brief Configure the output comparator for motor usage
 * @details Uses OC2
 */
void config_output_compare2(void);

/**
 * @brief Configure the output comparator for motor usage
 * @details Uses OC3
 */
void config_output_compare3(void);

/**
 * @brief Configure the output comparator for motor usage
 * @details Uses OC4
 */
void config_output_compare4(void);

/**
 * @brief Configure the output comparator for motor usage
 * @details Uses OC5
 */
void config_output_compare5(void);

/////////////////////////////////////////////// 
//
// Motor primitives
//
///////////////////////////////////////////////
/**
 * @brief Drive the left motor forward
 * 
 * @param duty speed for the left motor PWM
 */
void left_motor_fwd(float f_duty);

/**
 * @brief Drive the left motor reverse
 * 
 * @param duty speed for the left motor PWM
 */
void left_motor_reverse(float f_duty);

/**
 * @brief Stop the left motor
 */
void left_motor_stop(void);

/**
 * @brief Drive the right motor forward
 * 
 * @param duty speed for the right motor PWM
 */
void right_motor_fwd(float f_duty);

/**
 * @brief Drive the right motor reverse
 * 
 * @param duty speed for the right motor PWM
 */
void right_motor_reverse(float f_duty);

/**
 * @brief Stop the right motor
 */
void right_motor_stop(void);

/////////////////////////////////////////////// 
//
// Motor usage
//
///////////////////////////////////////////////
/**
 * @brief Stop both the motors
 */
void motors_stop(void);

/**
 * @brief Turn the robot right
 * 
 * @param duty speed to the turn the robot at
 */
void motors_turn_right(float f_duty);

/**
 * @brief Turn the robot left
 * 
 * @param duty speed to the turn the robot at
 */
void motors_turn_left(float f_duty);

/**
 * @brief Move the robot forward
 * 
 * @param duty speed to move the robot forward at
 */
void motors_move_forward(float f_duty);

/**
 * @brief Move the robot back
 * 
 * @param duty speed to move the robot back at
 */
void motors_move_reverse(float f_duty);

#endif