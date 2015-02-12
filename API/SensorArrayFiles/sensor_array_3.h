/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: sensor_array_3.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Functions to help control sensor array 3
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

#ifndef SENSOR_ARRAY_3_H_
#define SENSOR_ARRAY_3_H_

#define EMITTER_LINE3                   _RC15

#define SENSOR0_LINE3_IN                _RD7
#define SENSOR1_LINE3_IN                _RF0
#define SENSOR2_LINE3_IN                _RF1
#define SENSOR3_LINE3_IN                _RG1
#define SENSOR4_LINE3_IN                _RG0
#define SENSOR5_LINE3_IN                _RG14
#define SENSOR6_LINE3_IN                _RG12
#define SENSOR7_LINE3_IN                _RG13

#define SENSOR0_LINE3_OUT               _LATD7
#define SENSOR1_LINE3_OUT               _LATF0
#define SENSOR2_LINE3_OUT               _LATF1
#define SENSOR3_LINE3_OUT               _LATG1
#define SENSOR4_LINE3_OUT               _LATG0
#define SENSOR5_LINE3_OUT               _LATG14
#define SENSOR6_LINE3_OUT               _LATG12
#define SENSOR7_LINE3_OUT               _LATG13

///////////////////////////////////////////////
//
// Sensor config
//
///////////////////////////////////////////////
/**
* @brief Initializes the sensor array
 */
void sensor_array_init_line3(void);

/**
 * @brief Configures all the sensor arrays pins as outputs
 */
void config_outputs_line3(void);

/**
 * @brief Configures all the sensor arrays pins as inputs
 */
void config_inputs_line3(void);


///////////////////////////////////////////////
//
// Sensor primitives
//
///////////////////////////////////////////////

/**
 * @brief Sets all the sensors high and times as they discharge
 */
void set_sensors_high_line3();

/**
 * @brief Check each sensor on this array
 * @param pau16_sensorValues the current sensor values
 * @param u16_delta the time that has elapsed
**/
void check_sensor_values_line3(uint16_t* pau16_sensorValues, uint16_t u16_delta);

/**
 * @brief Turns on the sensor array emitters
 */
void emitters_off_line3(void);

/**
 * @brief Turns off the sensor array emitters
 */
void emitters_on_line3(void);

/**
 * @brief Get the max value from the calibration step
 * @return the max value
**/
uint16_t get_line_max_value_line3();

/**
 * Set the max value during calibration
 * @param u16_maxValue the new max value
**/
void set_line_max_value_line3(uint16_t u16_maxValue);

#endif
