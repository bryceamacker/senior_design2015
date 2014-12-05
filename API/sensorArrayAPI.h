/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: sensorArrayAPI.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Functions to help control sensor arrays
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        9/20/2014              SECON 2015
*********************************************************************/

#include "pic24_all.h"

#ifndef SENSOR_ARRAY_API_H_
#define SENSOR_ARRAY_API_H_

#define SENSOR_NUM              8
#define QTR_EMITTERS_OFF        0
#define QTR_EMITTERS_ON         1
#define QTR_EMITTERS_ON_AND_OFF 2

#define EMITTER_DELAY           200

/////////////////////////////////////////////// 
//
// Sensor config
//
///////////////////////////////////////////////
/**
 * @brief Initializes and calibrates the sensor array
 */
void snesor_array_init(void);

/**
 * @brief Configures all the sensor arrays pins as outputs
 */
void config_outputs(void);

/**
 * @brief Configures all the sensor arrays pins as inputs
 */
void config_inputs(void);

/**
 * @brief Configures timer 4 for sensor usage
 */
void config_timer4(void);

/**
 * @brief Turns on the sensor array emitters
 */
void emitters_off(void);

/**
 * @brief Turns off the sensor array emitters
 */
void emitters_on(void);

/////////////////////////////////////////////// 
//
// Sensor primitives
//
///////////////////////////////////////////////
/**
 * @brief Calibrates the sensor array for the current lighting
 * 
 * @param u8_readMode whether the emitters are on or not
 */
void calibrate(char u8_readMode);

/**
 * @brief Read the sensor values using a specific read mode
 * 
 * @param pau16_sensorValues the values from the sensor array
 * @param u8_readMode whether the emitters are on or not
 */
void read(uint16_t* pau16_sensorValues, char u8_readMode);

/**
 * @brief Read each individual capacitor value of each sensor on the array
 * 
 * @param pau16_sensorValues the values from the sensor array
 */
void read_values(uint16_t* pau16_sensorValues);

/////////////////////////////////////////////// 
//
// Sensor usage
//
///////////////////////////////////////////////
/**
 * @brief Read the entire sensor array
 * 
 * @param pau16_sensorValues the values from the sensor array
 * @param u8_readMode whether the emitters are on or not
 */
void read_sensor_array(uint16_t* pau16_sensorValues, char u8_readMode);

#endif