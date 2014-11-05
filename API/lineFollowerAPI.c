/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: lineFollowerAPI.c
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Uses a sensor array and two DC motors to follow
* a line
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Bryce Amacker         10/01/2014              SECON 2015
*********************************************************************/

#include "lineFollowerAPI.h"

void line_follower_init() {
    snesor_array_init();
    motors_init();
}

float get_line(uint16_t* pau16_sensorValues) {
    float f_line;
    uint16_t u16_i;
    uint16_t u16_sum;

    f_line = 0;
    read_sensor_array(pau16_sensorValues, QTR_EMITTERS_ON);
    for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
        f_line += pau16_sensorValues[u16_i] * (u16_i+1);
    }
    if(f_line == 0.0) {
        return 0.0;
    }
    u16_sum = 0;
    for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
        u16_sum += pau16_sensorValues[u16_i];
    }
    f_line = f_line/u16_sum;
    return f_line;
}

void follow_line(uint16_t* pau16_sensorValues, float f_maxSpeed) {
    float f_line, f_lineToSpeedFactor, f_rightDuty, f_leftDuty;

    f_lineToSpeedFactor = f_maxSpeed/ 4.5;
    f_line = get_line(pau16_sensorValues);
    //if f_line = 0 relie on previous readings
    if(f_line == 0) {
        return;
    }
    f_rightDuty = f_line*f_lineToSpeedFactor;
    f_leftDuty = (9-f_line)*f_lineToSpeedFactor;

    if(f_rightDuty > f_maxSpeed) {
        f_rightDuty = f_maxSpeed;
    }
    if(f_leftDuty > f_maxSpeed) {
        f_leftDuty = f_maxSpeed;
    }
    right_motor_fwd(f_rightDuty);
    left_motor_fwd(f_leftDuty);
    printf("line: %f left: %f right: %f \n",f_line, f_leftDuty, f_rightDuty);
}

void follow_line_back(uint16_t* pau16_sensorValues, float f_maxSpeed) {
    float f_lineToSpeedFactor, f_line, f_rightDuty, f_leftDuty;

    f_lineToSpeedFactor = f_maxSpeed/ 4.5;
    f_line = get_line(pau16_sensorValues);
    //if f_line = 0 relie on previous readings
    if(f_line == 0) {
        return;
    }
    f_rightDuty = f_line*f_lineToSpeedFactor;
    f_leftDuty = (9-f_line)*f_lineToSpeedFactor;
    if(f_rightDuty > f_maxSpeed) {
        f_rightDuty = f_maxSpeed;
    }
    if(f_leftDuty > f_maxSpeed) {
        f_leftDuty = f_maxSpeed;
    }
    left_motor_reverse(f_rightDuty);
    right_motor_reverse(f_leftDuty);
}

// Print line follower data
void print_line_data(uint16_t* pau16_sensorValues) { 
    int i;
    for (i = 0 ; i < SENSOR_NUM ; ++i)
    {
        printf("%d-", pau16_sensorValues[i]);
    }
}
