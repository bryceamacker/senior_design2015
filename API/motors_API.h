/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: motors_API.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Functions to help control DC motors
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        9/20/2014               SECON 2015
*********************************************************************/

#include "pic24_all.h"
#include <stdbool.h>

#ifdef DEBUG_BUILD
#include <stdio.h>
#endif

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

#define ENCODER_YELLOW_PIN              _RG6
#define ENCODER_WHITE_PIN               _RG7

#define GET_ENCODER_DATA()              ((ENCODER_YELLOW_PIN << 1) | ENCODER_WHITE_PIN)
#define ROT_MAX                         1120
#define ENCODER_INTERRUPT_PERIOD        100 // microseconds

#define MOTOR_PWM_PERIOD 20000  // desired period, in us

#define LEFT_DIRECTION                  0
#define RIGHT_DIRECTION                 1

#define STOPPED                         0
#define FORWARD_MOVEMENT                1
#define BACKWARD_MOVEMENT               2

#define DEGREE_90_TURN_TIME             2600 // At 15 % speed
#define PREPARE_TURN_TIME               1500 // At 15 % speed

#define WHEEL_DIAMETER                  1000 // in milimeters

#define M_PI 3.14159265358979323846

///////////////////////////////////////////////
//
// Motor config
//
///////////////////////////////////////////////
/**
 * @brief Initializes every thing for the motors
 */
void motors_init();

void config_encoders(void);

/**
 * @brief Configs timer 2 to drive motors
 */
void config_motor_timer2(void);

/**
 * @brief Configs timer 3 to drive motors
 */
void config_motor_timer3(void);

void config_encoder_interrupts(void);

/**
 * @brief Configure the output comparator for motor usage
 * @details Uses OC2
 */
void motor_config_output_compare2(void);

/**
 * @brief Configure the output comparator for motor usage
 * @details Uses OC3
 */
void motor_config_output_compare3(void);

/**
 * @brief Configure the output comparator for motor usage
 * @details Uses OC4
 */
void motor_config_output_compare4(void);

/**
 * @brief Configure the output comparator for motor usage
 * @details Uses OC5
 */
void motor_config_output_compare5(void);

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

void process_right_rotary_data();
void process_left_rotary_data();


float get_right_motor_location();
float get_left_motor_location();

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

void prepare_for_90_degree_turn(float f_duty);
void turn_90_degrees(float f_duty, uint8_t u8_direction);

void set_prepare_time(uint16_t u16_newTime);
void set_turn_time(uint16_t u16_newTime);

#endif
