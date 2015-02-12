/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: sensor_array_5.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Functions to help control sensor array 5
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

#ifndef SENSOR_ARRAY_5_H_
#define SENSOR_ARRAY_5_H_

#define EMITTER_LINE5                   _RF6

#define SENSOR0_LINE5_IN                _RF5
#define SENSOR1_LINE5_IN                _RG9
#define SENSOR2_LINE5_IN                _RG8
#define SENSOR3_LINE5_IN                _RG7
#define SENSOR4_LINE5_IN                _RG6
#define SENSOR5_LINE5_IN                _RC2
#define SENSOR6_LINE5_IN                _RC1
#define SENSOR7_LINE5_IN                _RG15

#define SENSOR0_LINE5_OUT               _LATF5
#define SENSOR1_LINE5_OUT               _LATG9
#define SENSOR2_LINE5_OUT               _LATG8
#define SENSOR3_LINE5_OUT               _LATG7
#define SENSOR4_LINE5_OUT               _LATG6
#define SENSOR5_LINE5_OUT               _LATC2
#define SENSOR6_LINE5_OUT               _LATC1
#define SENSOR7_LINE5_OUT               _LATG15

///////////////////////////////////////////////
//
// Sensor config
//
///////////////////////////////////////////////
/**
* @brief Initializes the sensor array
 */
void sensor_array_init_line5(void);

/**
 * @brief Configures all the sensor arrays pins as outputs
 */
void config_outputs_line5(void);

/**
 * @brief Configures all the sensor arrays pins as inputs
 */
void config_inputs_line5(void);


///////////////////////////////////////////////
//
// Sensor primitives
//
///////////////////////////////////////////////

/**
 * @brief Sets all the sensors high and times as they discharge
 */
void set_sensors_high_line5();

/**
 * @brief Check each sensor on this array
 * @param pau16_sensorValues the current sensor values
 * @param u16_delta the time that has elapsed
**/
void check_sensor_values_line5(uint16_t* pau16_sensorValues, uint16_t u16_delta);

/**
 * @brief Turns on the sensor array emitters
 */
void emitters_off_line5(void);

/**
 * @brief Turns off the sensor array emitters
 */
void emitters_on_line5(void);

/**
 * @brief Get the max value from the calibration step
 * @return the max value
**/
uint16_t get_line_max_value_line5();

/**
 * Set the max value during calibration
 * @param u16_maxValue the new max value
**/
void set_line_max_value_line5(uint16_t u16_maxValue);

#endif
