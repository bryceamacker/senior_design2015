/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: sensor_array_API.c
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
* Steven Calhoun        9/20/2014               SECON 2015
*********************************************************************/

#include "sensor_array_API.h"

uint16_t u16_maxValue = 0x7FFF;

/////////////////////////////////////////////// 
//
// Sensor config
//
///////////////////////////////////////////////

// Initializes and callibrates the sensor array
void snesor_array_init() {
    uint8_t i;
    
    // Calibrate a bit for better array results
    for (i = 0; i < 100; i++) {
        calibrate(QTR_EMITTERS_ON);
    }
}

// Configures pins as dig outputs
void config_outputs() {
    CONFIG_RB0_AS_DIG_OUTPUT();
    CONFIG_RB1_AS_DIG_OUTPUT();
    CONFIG_RB2_AS_DIG_OUTPUT();
    CONFIG_RB3_AS_DIG_OUTPUT();
    CONFIG_RB4_AS_DIG_OUTPUT();
    CONFIG_RB5_AS_DIG_OUTPUT();
    CONFIG_RB6_AS_DIG_OUTPUT();
    CONFIG_RB7_AS_DIG_OUTPUT();
}

// Configures pins as dig inpts
void config_inputs() {
    CONFIG_RB0_AS_DIG_INPUT();
    DISABLE_RB0_PULLUP();
    CONFIG_RB1_AS_DIG_INPUT();
    DISABLE_RB1_PULLUP();
    CONFIG_RB2_AS_DIG_INPUT();
    DISABLE_RB2_PULLUP();
    CONFIG_RB3_AS_DIG_INPUT();
    DISABLE_RB3_PULLUP();
    CONFIG_RB4_AS_DIG_INPUT();
    DISABLE_RB4_PULLUP();
    CONFIG_RB5_AS_DIG_INPUT();
    DISABLE_RB5_PULLUP();
    CONFIG_RB6_AS_DIG_INPUT();
    DISABLE_RB6_PULLUP();
    CONFIG_RB7_AS_DIG_INPUT();
    DISABLE_RB7_PULLUP();
}

// Configures timer to measure discharge time of the capacitor
void  configTimer4() {
    T4CON = T4_OFF 
            | T4_IDLE_CON 
            | T4_GATE_OFF
            | T4_32BIT_MODE_OFF
            | T4_SOURCE_INT
            | T4_PS_1_8;  //1 tick = 0.2 us at FCY=40 MHz
    PR4 = 0xFFFF;                    //maximum period
    TMR4  = 0;                       //clear timer2 value
    _T4IF = 0;                       //clear interrupt flag
    T4CONbits.TON = 1;               //turn on the timer
} 

// Disables LED
void emittersOff() {
    _RB8 = 0;
    DELAY_US(EMITTER_DELAY);
}

// Enables LED
void emittersOn() {
    _RB8 = 1;
    DELAY_US(EMITTER_DELAY);
}

/////////////////////////////////////////////// 
//
// Sensor primitives
//
///////////////////////////////////////////////

// Calibrates to set the max value
void calibrate(char u8_readMode) {
    uint16_t pau16_sensorValues[SENSOR_NUM]; // This might need to be uint16_t*, hasn't been tested
    uint16_t u16_i;
    uint16_t u16_minValue;

    read(pau16_sensorValues, u8_readMode);
    u16_minValue = pau16_sensorValues[0];
    
    for(u16_i = 1; u16_i < SENSOR_NUM; u16_i++) {
        if(pau16_sensorValues[u16_i] < u16_minValue) {
            u16_minValue = pau16_sensorValues[u16_i];
        }
    }
    u16_maxValue = u16_minValue*2.5; 
} 

// Reads the sensor values using a specific read mode
void read(uint16_t* pau16_sensor_values, char u8_readMode) {
    uint16_t pau16_off_values[SENSOR_NUM];
    uint16_t u16_i;
    //emitter pin = RB8
    CONFIG_RB8_AS_DIG_OUTPUT();
    if(u8_readMode == QTR_EMITTERS_ON || u8_readMode == QTR_EMITTERS_ON_AND_OFF) {
        emittersOn();
    }
    read_values(pau16_sensor_values);

    emittersOff();
    if(u8_readMode == QTR_EMITTERS_ON_AND_OFF) {
        read_values(pau16_off_values);
    }

    if(u8_readMode == QTR_EMITTERS_ON_AND_OFF) {
        for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
            pau16_sensor_values[u16_i] += u16_maxValue - pau16_off_values[u16_i];
        }
    }
}

// Measures the discharge time for each capacitor associated with a sensor
void read_values(uint16_t* pau16_sensor_values) {
    uint16_t u16_start, u16_delta, u16_i, u16_pin;
    uint8_t pau8_sensorCheck[SENSOR_NUM];
    config_outputs();
    DELAY_US(1);
    for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
        pau16_sensor_values[u16_i] = u16_maxValue;
        pau8_sensorCheck[u16_i] = 0;
    }
    //drive outputs RB0 - RB7 and emitter high
    PORTB = 0x01FF;
    DELAY_US(10);
    
    //change outputs to inputs
    //dissable internal pullups
    config_inputs();
    DELAY_US(1);
    //mesure time for each input

    configTimer4();
    u16_start = TMR4;
    while(TMR4 - u16_start < u16_maxValue) {
        u16_delta = TMR4 - u16_start;
        for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
            u16_pin = 0x0001;
            u16_pin = u16_pin << u16_i;
            if(((PORTB & u16_pin) == 0) && (u16_delta < pau16_sensor_values[u16_i])) {
                pau16_sensor_values[u16_i] = u16_delta;
            }
        }
    }
    T4CONbits.TON = 0;
}

/////////////////////////////////////////////// 
//
// Sensor usage
//
///////////////////////////////////////////////

// Binary encodes the raw values of each sensor
void read_sensor_array(uint16_t* pau16_sensorValues, char u8_readMode) {
    uint16_t u16_i;
    read(pau16_sensorValues,u8_readMode);

    for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
        if(pau16_sensorValues[u16_i] < u16_maxValue - 1) {
            pau16_sensorValues[u16_i] = 1;
        }
        else {
            pau16_sensorValues[u16_i] = 0;
        }
    }
} 
