/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: sensor_array_4.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Functions to help control sensor array 4
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

#ifndef SENSOR_ARRAY_4_H_
#define SENSOR_ARRAY_4_H_

#define EMITTER_LINE4                   _RC12

#define SENSOR0_LINE4_IN                _RD9
#define SENSOR1_LINE4_IN                _RD10
#define SENSOR2_LINE4_IN                _RD11
#define SENSOR3_LINE4_IN                _RD0
#define SENSOR4_LINE4_IN                _RC13
#define SENSOR5_LINE4_IN                _RC14
#define SENSOR6_LINE4_IN                _RD5
#define SENSOR7_LINE4_IN                _RD6

#define SENSOR0_LINE4_OUT               _LATD9
#define SENSOR1_LINE4_OUT               _LATD10
#define SENSOR2_LINE4_OUT               _LATD11
#define SENSOR3_LINE4_OUT               _LATD0
#define SENSOR4_LINE4_OUT               _LATC13
#define SENSOR5_LINE4_OUT               _LATC14
#define SENSOR6_LINE4_OUT               _LATD5
#define SENSOR7_LINE4_OUT               _LATD6

///////////////////////////////////////////////
//
// Sensor config
//
///////////////////////////////////////////////
/**
* @brief Initializes the sensor array
 */
void sensor_array_init_line4(void);

/**
 * @brief Configures all the sensor arrays pins as outputs
 */
void config_outputs_line4(void);

/**
 * @brief Configures all the sensor arrays pins as inputs
 */
void config_inputs_line4(void);


///////////////////////////////////////////////
//
// Sensor primitives
//
///////////////////////////////////////////////

/**
 * @brief Sets all the sensors high and times as they discharge
 */
void set_sensors_high_line4();

/**
 * @brief Check each sensor on this array
 * @param pau16_sensorValues the current sensor values
 * @param u16_delta the time that has elapsed
**/
void check_sensor_values_line4(uint16_t* pau16_sensorValues, uint16_t u16_delta);

/**
 * @brief Turns on the sensor array emitters
 */
void emitters_off_line4(void);

/**
 * @brief Turns off the sensor array emitters
 */
void emitters_on_line4(void);

/**
 * @brief Get the max value from the calibration step
 * @return the max value
**/
uint16_t get_line_max_value_line4();

/**
 * Set the max value during calibration
 * @param u16_maxValue the new max value
**/
void set_line_max_value_line4(uint16_t u16_maxValue);

#endif
