/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: sensorArrayAPI.c
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

#include "sensorArrayAPI.h"

uint16_t u16_maxValue = 0x7FFF;

// Configures pins as dig outputs
void config_outputs(void) {
    CONFIG_RB0_AS_DIG_OUTPUT();
    CONFIG_RB1_AS_DIG_OUTPUT();
    CONFIG_RB2_AS_DIG_OUTPUT();
    CONFIG_RB3_AS_DIG_OUTPUT();
    CONFIG_RB4_AS_DIG_OUTPUT();
    CONFIG_RB5_AS_DIG_OUTPUT();
    CONFIG_RB6_AS_DIG_OUTPUT();
    CONFIG_RB7_AS_DIG_OUTPUT();
    CONFIG_RB8_AS_DIG_OUTPUT();
    CONFIG_RB9_AS_DIG_OUTPUT();
    CONFIG_RB10_AS_DIG_OUTPUT();
    CONFIG_RB11_AS_DIG_OUTPUT();
    CONFIG_RB12_AS_DIG_OUTPUT();
    CONFIG_RB13_AS_DIG_OUTPUT();
    CONFIG_RB14_AS_DIG_OUTPUT();
}

// Configures pins as dig inpts
void config_inputs(void) {
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
    CONFIG_RB8_AS_DIG_INPUT();
    DISABLE_RB8_PULLUP();
    CONFIG_RB9_AS_DIG_INPUT();
    DISABLE_RB9_PULLUP();
    CONFIG_RB10_AS_DIG_INPUT();
    DISABLE_RB10_PULLUP();
    CONFIG_RB11_AS_DIG_INPUT();
    DISABLE_RB11_PULLUP();
    CONFIG_RB12_AS_DIG_INPUT();
    DISABLE_RB12_PULLUP();
    CONFIG_RB13_AS_DIG_INPUT();
    DISABLE_RB13_PULLUP();
    CONFIG_RB14_AS_DIG_INPUT();
    DISABLE_RB14_PULLUP();
}

// Configures timer to measure discharge time of the capacitor
void config_timer4(void) {
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
void emitters_off(void) {
    _RF4 = 0;
    DELAY_US(200);
}

// Enables LED
void emitters_on(void) {
    _RF4 = 1;
    DELAY_US(200);
}

// Calibrates to set the max value
void calibrate(char u8_readMode) {
    uint16_t* pau16_sensorValues[SENSOR_NUM];
    read(pau16_sensorValues, u8_readMode);
    uint16_t u16_i;
    uint16_t u16_minValue = pau16_sensorValues[0];
    for(u16_i = 1; u16_i < SENSOR_NUM; u16_i++) {
        printf(" %u \t", pau16_sensorValues[u16_i]);
        
        if(pau16_sensorValues[u16_i] < u16_minValue) {
            u16_minValue = pau16_sensorValues[u16_i];
        }
    }
    u16_maxValue = u16_minValue*2.5; 
} 

// Reads the sensor values using a specific read mode
void read(uint16_t* pau16_sensorValues, char u8_readMode) {
    uint16_t pau16_offValues[SENSOR_NUM];
    uint16_t u16_i;
    //emitter pin = RF4
    CONFIG_RF4_AS_DIG_OUTPUT();
    if(u8_readMode == QTR_EMITTERS_ON || u8_readMode == QTR_EMITTERS_ON_AND_OFF) {
        emitters_on();
    }
    read_values(pau16_sensorValues);

    emitters_off();
    if(u8_readMode == QTR_EMITTERS_ON_AND_OFF) {
        read_values(pau16_offValues);
    }

    if(u8_readMode == QTR_EMITTERS_ON_AND_OFF) {
        for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
            pau16_sensorValues[u16_i] += u16_maxValue - pau16_offValues[u16_i];
        }
    }
}

// Measures the discharge time for each capacitor associated with a sensor
void read_values(uint16_t* pau16_sensorValues) {
    uint16_t u16_start, u16_delta, u16_i, u16_pin;
    uint8_t pau8_sensorCheck[SENSOR_NUM];
    config_outputs();
    DELAY_US(1);
    for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
        pau16_sensorValues[u16_i] = u16_maxValue;
        pau8_sensorCheck[u16_i] = 0;
    }
    //drive outputs RB0 - RB7 and emitter high
    PORTB = 0x01FF;
        // 0000 0001 1111 1111
        // RB0 - RB8

        //drive outputs RB0 - RB14
        //PORTB = 0x7FFF;
        PORTF = 0x0010;
    DELAY_US(10);
        // 0111 1111 1111 1111
        // RB0 - RB14
    
    //change outputs to inputs
    //dissable internal pullups
    config_inputs();
    DELAY_US(1);
    //mesure time for each input

    config_timer4();
    u16_start = TMR4;
    while(TMR4 - u16_start < u16_maxValue) {
        u16_delta = TMR4 - u16_start;
        for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
            u16_pin = 0x0001;
            u16_pin = u16_pin << u16_i;
            if(((PORTB & u16_pin) == 0) && (u16_delta < pau16_sensorValues[u16_i])) {
                pau16_sensorValues[u16_i] = u16_delta;
            }
        }
    }
    T4CONbits.TON = 0;
}

// Binary encodes the raw values of each sensor
void read_line(uint16_t* pau16_sensorValues, char u8_readMode) {
    read(pau16_sensorValues,u8_readMode);
    uint16_t u16_i;
    /*
    for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
        printf(" %u \t",pau16_sensorValues[u16_i]);
        DELAY_MS(1);
    }
    */
    for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
        if(pau16_sensorValues[u16_i] < u16_maxValue - 1) {
            pau16_sensorValues[u16_i] = 1;
        }
        else {
            pau16_sensorValues[u16_i] = 0;
        }
    }
} 
