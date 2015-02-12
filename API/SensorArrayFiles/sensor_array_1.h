/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: sensor_array_1.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Functions to help control sensor array 1
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        2/11/2015               SECON 2015
*********************************************************************/

#include "pic24_all.h"
#include "sensor_array_common.h"
#ifdef DEBUG_BUILD
#include <stdio.h>
#endif

#ifndef SENSOR_ARRAY_1_H_
#define SENSOR_ARRAY_1_H_

#define EMITTER_LINE1                   _RB8

#define SENSOR0_LINE1_IN                _RB0
#define SENSOR1_LINE1_IN                _RB1
#define SENSOR2_LINE1_IN                _RB2
#define SENSOR3_LINE1_IN                _RB3
#define SENSOR4_LINE1_IN                _RB4
#define SENSOR5_LINE1_IN                _RB5
#define SENSOR6_LINE1_IN                _RB6
#define SENSOR7_LINE1_IN                _RB7

#define SENSOR0_LINE1_OUT               _LATB0
#define SENSOR1_LINE1_OUT               _LATB1
#define SENSOR2_LINE1_OUT               _LATB2
#define SENSOR3_LINE1_OUT               _LATB3
#define SENSOR4_LINE1_OUT               _LATB4
#define SENSOR5_LINE1_OUT               _LATB5
#define SENSOR6_LINE1_OUT               _LATB6
#define SENSOR7_LINE1_OUT               _LATB7

///////////////////////////////////////////////
//
// Sensor config
//
///////////////////////////////////////////////
/**
* @brief Initializes the sensor array
 */
void sensor_array_init_line1(void);

/**
 * @brief Configures all the sensor arrays pins as outputs
 */
void config_outputs_line1(void);

/**
 * @brief Configures all the sensor arrays pins as inputs
 */
void config_inputs_line1(void);


///////////////////////////////////////////////
//
// Sensor primitives
//
///////////////////////////////////////////////

/**
 * @brief Sets all the sensors high and times as they discharge
 */
void set_sensors_high_line1();

/**
 * @brief Check each sensor on this array
 * @param pau16_sensorValues the current sensor values
 * @param u16_delta the time that has elapsed
**/
void check_sensor_values_line1(uint16_t* pau16_sensorValues, uint16_t u16_delta);

/**
 * @brief Turns on the sensor array emitters
 */
void emitters_off_line1(void);

/**
 * @brief Turns off the sensor array emitters
 */
void emitters_on_line1(void);

/**
 * @brief Get the max value from the calibration step
 * @return the max value
**/
uint16_t get_line_max_value_line1();

/**
 * Set the max value during calibration
 * @param u16_maxValue the new max value
**/
void set_line_max_value_line1(uint16_t u16_maxValue);

#endif
