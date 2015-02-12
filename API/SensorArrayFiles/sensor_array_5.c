/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: sensor_array_5.c
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

#include "sensor_array_5.h"

uint16_t u16_maxValueLine5 = 0x7FFF;

///////////////////////////////////////////////
//
// Sensor config
//
///////////////////////////////////////////////

// Initializes sensor array
void sensor_array_init_line5() {
    CONFIG_RF6_AS_DIG_OUTPUT();
}

// Configures pins as dig outputs
void config_outputs_line5() {
    CONFIG_RF5_AS_DIG_OUTPUT();
    CONFIG_RG9_AS_DIG_OUTPUT();
    CONFIG_RG8_AS_DIG_OUTPUT();
    CONFIG_RG7_AS_DIG_OUTPUT();
    CONFIG_RG6_AS_DIG_OUTPUT();
    CONFIG_RC2_AS_DIG_OUTPUT();
    CONFIG_RC1_AS_DIG_OUTPUT();
    CONFIG_RG15_AS_DIG_OUTPUT();
}

// Configures pins as dig inputs
void config_inputs_line5() {
    CONFIG_RF5_AS_DIG_INPUT();
    DISABLE_RF5_PULLUP();

    CONFIG_RG9_AS_DIG_INPUT();
    DISABLE_RG9_PULLUP();

    CONFIG_RG8_AS_DIG_INPUT();
    DISABLE_RG8_PULLUP();

    CONFIG_RG7_AS_DIG_INPUT();
    DISABLE_RG7_PULLUP();

    CONFIG_RG6_AS_DIG_INPUT();
    DISABLE_RG6_PULLUP();

    CONFIG_RC2_AS_DIG_INPUT();
    DISABLE_RC2_PULLUP();

    CONFIG_RC1_AS_DIG_INPUT();
    DISABLE_RC1_PULLUP();

    CONFIG_RG15_AS_DIG_INPUT();
    DISABLE_RG15_PULLUP();
}

///////////////////////////////////////////////
//
// Sensor primitives
//
///////////////////////////////////////////////

// Sets all the sensors high and times as they discharge
void set_sensors_high_line5() {
    SENSOR0_LINE5_OUT = 1;
    SENSOR1_LINE5_OUT = 1;
    SENSOR2_LINE5_OUT = 1;
    SENSOR3_LINE5_OUT = 1;
    SENSOR4_LINE5_OUT = 1;
    SENSOR5_LINE5_OUT = 1;
    SENSOR6_LINE5_OUT = 1;
    SENSOR7_LINE5_OUT = 1;
}

// Check each sensor on this array
void check_sensor_values_line5(uint16_t* pau16_sensorValues, uint16_t u16_delta) {
    if((SENSOR0_LINE5_IN == 0) && (u16_delta < pau16_sensorValues[0])) {
        pau16_sensorValues[0] = u16_delta;
    }
    if((SENSOR1_LINE5_IN == 0) && (u16_delta < pau16_sensorValues[1])) {
        pau16_sensorValues[1] = u16_delta;
    }
    if((SENSOR2_LINE5_IN == 0) && (u16_delta < pau16_sensorValues[2])) {
        pau16_sensorValues[2] = u16_delta;
    }
    if((SENSOR3_LINE5_IN == 0) && (u16_delta < pau16_sensorValues[3])) {
        pau16_sensorValues[3] = u16_delta;
    }
    if((SENSOR4_LINE5_IN == 0) && (u16_delta < pau16_sensorValues[4])) {
        pau16_sensorValues[4] = u16_delta;
    }
    if((SENSOR5_LINE5_IN == 0) && (u16_delta < pau16_sensorValues[5])) {
        pau16_sensorValues[5] = u16_delta;
    }
    if((SENSOR6_LINE5_IN == 0) && (u16_delta < pau16_sensorValues[6])) {
        pau16_sensorValues[6] = u16_delta;
    }
    if((SENSOR7_LINE5_IN == 0) && (u16_delta < pau16_sensorValues[7])) {
        pau16_sensorValues[7] = u16_delta;
    }
}

// Disable LEDs
void emitters_off_line5() {
    EMITTER_LINE5 = 0;
    DELAY_US(EMITTER_DELAY);
}

// Enable LEDs
void emitters_on_line5() {
    EMITTER_LINE5 = 1;
    DELAY_US(EMITTER_DELAY);
}

// Get the max value from the calibration step
uint16_t get_line_max_value_line5() {
    return u16_maxValueLine5;
}

// Set the max value during calibration
void set_line_max_value_line5(uint16_t u16_maxValue) {
    u16_maxValueLine5 = u16_maxValue;
}
