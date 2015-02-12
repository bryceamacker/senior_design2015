/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: sensor_array_2.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Functions to help control sensor array 2
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

#ifndef SENSOR_ARRAY_2_H_
#define SENSOR_ARRAY_2_H_

#define EMITTER_LINE2                   _RD8

#define SENSOR0_LINE2_IN                _RF4
#define SENSOR1_LINE2_IN                _RB15
#define SENSOR2_LINE2_IN                _RB14
#define SENSOR3_LINE2_IN                _RB13
#define SENSOR4_LINE2_IN                _RB12
#define SENSOR5_LINE2_IN                _RB11
#define SENSOR6_LINE2_IN                _RB10
#define SENSOR7_LINE2_IN                _RB9

#define SENSOR0_LINE2_OUT               _LATF4
#define SENSOR1_LINE2_OUT               _LATB15
#define SENSOR2_LINE2_OUT               _LATB14
#define SENSOR3_LINE2_OUT               _LATB13
#define SENSOR4_LINE2_OUT               _LATB12
#define SENSOR5_LINE2_OUT               _LATB11
#define SENSOR6_LINE2_OUT               _LATB10
#define SENSOR7_LINE2_OUT               _LATB9

///////////////////////////////////////////////
//
// Sensor config
//
///////////////////////////////////////////////
/**
 * @brief Initializes the sensor array
 */
void sensor_array_init_line2(void);

/**
 * @brief Configures all the sensor arrays pins as outputs
 */
void config_outputs_line2(void);

/**
 * @brief Configures all the sensor arrays pins as inputs
 */
void config_inputs_line2(void);


///////////////////////////////////////////////
//
// Sensor primitives
//
///////////////////////////////////////////////

/**
 * @brief Sets all the sensors high and times as they discharge
 */
void set_sensors_high_line2();

/**
 * @brief Check each sensor on this array
 * @param pau16_sensorValues the current sensor values
 * @param u16_delta the time that has elapsed
**/
void check_sensor_values_line2(uint16_t* pau16_sensorValues, uint16_t u16_delta);

/**
 * @brief Turns on the sensor array emitters
 */
void emitters_off_line2(void);

/**
 * @brief Turns off the sensor array emitters
 */
void emitters_on_line2(void);

/**
 * @brief Get the max value from the calibration step
 * @return the max value
**/
uint16_t get_line_max_value_line2();

/**
 * Set the max value during calibration
 * @param u16_maxValue the new max value
**/
void set_line_max_value_line2(uint16_t u16_maxValue);

#endif
