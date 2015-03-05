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
#include "navigation_port_mapping.h"

#ifdef DEBUG_BUILD
#include <stdio.h>
#endif

#ifndef _MOTORS_API_H_
#define _MOTORS_API_H_

#define CONFIG_RIGHT_MOTOR_IN1()        CONFIG_RD1_AS_DIG_OUTPUT()
#define CONFIG_RIGHT_MOTOR_IN2()        CONFIG_RD2_AS_DIG_OUTPUT()

#define CONFIG_LEFT_MOTOR_IN1()         CONFIG_RD3_AS_DIG_OUTPUT()
#define CONFIG_LEFT_MOTOR_IN2()         CONFIG_RD4_AS_DIG_OUTPUT()

#define ROT_MAX                         1120 - 1                // Max encoder value

#define MOTOR_PWM_PERIOD                20000                   // desired period, in us

#define RIGHT_MOTOR_ADJUSTMENT          1.0
#define LEFT_MOTOR_ADJUSTMENT           1.09

#define LEFT_DIRECTION                  0                       // Boolean for left turning direction
#define RIGHT_DIRECTION                 1                       // Boolean for left turning direction

#define STOPPED                         0                       // Stopped motion
#define FORWARD_MOVEMENT                1                       // Forward motion
#define BACKWARD_MOVEMENT               2                       // Backward motion

#define PREPARE_90_TURN_DISTANCE        180
#define DEGREE_90_TURN_REVS             1.0

#define FINISH_180_TURN_DISTANCE        220

#define BACK_AWAY_FROM_GAME_DISTANCE    100
#define MOVE_INTO_BOX_DISTANCE          160

#define M_PI                            3.14159265358979323846  // pi constant
#define WHEEL_DIAMETER                  80.65                   // in milimeters
#define WHEEL_CIRCUMFERENCE             (WHEEL_DIAMETER * M_PI) // in milimeters

#define BASE_SPEED_DEFAULT              20

#define LINE_WIDTH                      25

#define START_BOX_DRIVE_DISTANCE        200

extern float BASE_SPEED;

typedef enum {
    RIGHT_TURN =                    1,
    LEFT_TURN =                     2,
    PREPARE_TURN =                  3,
    FINISH_TURN =                   4,
    FINISH_REVERSE_TURN =           5,
    BACK_AWAY_FROM_BOX =            6,
    MOVE_PAST_START_BOX =           7,
    MOVE_PAST_BRANCH =              8,
    PREPARE_REVERSE_TURN =          9,
    MOVE_FORWARD_DISTANCE =         10,
    MOVE_REVERSE_DISTANCE =         11,
    PLAY_GAME_PAUSE =               12,
    FINISH_180_TURN =               13,
    MOVE_INTO_BOX =                 14,
    TURN_180 =                      15
} motorRoutines;

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
 * @brief Configs the interrupts for each encoder
 */
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
void left_motor_fwd(float f_speed);

/**
 * @brief Drive the left motor reverse
 *
 * @param duty speed for the left motor PWM
 */
void left_motor_reverse(float f_speed);

/**
 * @brief Stop the left motor
 */
void left_motor_stop(void);

/**
 * @brief Drive the right motor forward
 *
 * @param duty speed for the right motor PWM
 */
void right_motor_fwd(float f_speed);

/**
 * @brief Drive the right motor reverse
 *
 * @param duty speed for the right motor PWM
 */
void right_motor_reverse(float f_speed);

/**
 * @brief Stop the right motor
 */
void right_motor_stop(void);

/**
 * @brief Process the encoder for the right motoer
**/
void process_right_rotary_data(void);

/**
 * @brief Process the encoder for the right motoer
**/
void process_left_rotary_data(void);

/**
 * @brief Get the current location of the right motor
 *
 * @return the revolution count and fraction
**/
float get_right_motor_location(void);

/**
 * @brief Get the current location of the left motor
 *
 * @return the revolution count and fraction
**/
float get_left_motor_location(void);

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
 * @param f_speed speed to the turn the robot at
 */
void motors_turn_right(float f_speed);

/**
 * @brief Turn the robot left
 *
 * @param f_speed speed to the turn the robot at
 */
void motors_turn_left(float f_speed);

/**
 * @brief Move the robot forward
 *
 * @param f_speed speed to move the robot forward at
 */
void motors_move_forward(float f_speed);

/**
 * @brief Move the robot back
 *
 * @param f_speed speed to move the robot back at
 */
void motors_move_reverse(float f_speed);

/**
 * @brief Prepare the robot for a 90 degree turn using delays
 *
 * @param f_speed speed to move the robot
**/
void prepare_for_90_degree_turn(float f_speed);

/**
 * @brief Turn the robot by 90 degrees
 *
 * @param f_speed speed to move the robot
 * @param u8_direction Direction to turn
**/
void turn_90_degrees(float f_speed, uint8_t u8_direction);

/**
 * @brief Turn the robot by 180 degrees
 *
 * @param f_speed speed to move the robot
 * @param u8_direction Direction to turn
**/
void turn_180_degrees(float f_speed, uint8_t u8_direction);

/**
 * @brief Move back to compensate for a 180 turn
 *
 * @param f_speed speed to move the robot
**/
void finish_180_degree_turn(float f_speed);

/**
 * @brief Turn the left motor by a number of revolutions
 *
 * @param f_revolutions Number of revolutions to turn
 * @param f_speed Speed to turn the motor
**/
void move_right_motor_by_revolutions(float f_revolutions, float f_speed);

/**
 * @brief Turn the right motor by a number of revolutions
 *
 * @param f_revolutions Number of revolutions to turn
 * @param f_speed Speed to turn the motor
**/
void move_left_motor_by_revolutions(float f_revolutions, float f_speed);

/**
 * @brief Move the right motor by mm
 *
 * @param f_distance Milimeters to move the motor
 * @param f_speed Speed to turn the motor
**/
void move_right_motor_by_distance(float f_distance, float f_speed);

/**
 * @brief Move the left motor by mm
 *
 * @param f_distance Milimeters to move the motor
 * @param f_speed Speed to turn the motor
**/
void move_left_motor_by_distance(float f_distance, float f_speed);

/**
 * @brief Turn the robot by a number of revolutions
 *
 * @param f_revolutions Number of revolutions to turn
 * @param f_speed Speed to turn the robot
**/
void move_by_revolutions(float f_revolutions, float f_speed);

/**
 * @brief Move the robot by mm
 *
 * @param f_distance Milimeters to move the robot
 * @param f_speed Speed to turn the robot
**/
void move_by_distance(float f_distance, float f_speed);

/**
 * @brief Check for a new routine in the navigation queue
 *
 * @return dequeue the routine that is in the front of the queue and return it
**/
uint8_t check_for_routine(void);

/**
 * @brief Handle a routine by changing the targets for the left and right motors
 *
 * @param routine The routine to be handled
**/
void handle_routine(uint8_t routine);

/**
 * @brief Sort of an emergency stop on the routines, stop the motors, clear the queue, reset targets
**/
void clear_routines(void);

/**
 * @brief Set the speed at which all motor funcitons operate
 *
 * @param f_newBase New base speed
**/
void set_base_speed(float f_newBase);

/**
 * @brief Block until all routines have been finished
**/
void block_until_all_routines_done();

#endif
