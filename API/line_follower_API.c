/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: line_follower_API.c
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

#include "line_follower_API.h"

void line_follower_init() {
    sensor_array_init();
    motors_init();
}

float get_line(uint16_t* pau16_sensorValues) {
    float f_line;
    uint16_t u16_i;
    uint16_t u16_sum;

    f_line = 0;
    // read_sensor_array(pau16_sensorValues, QTR_EMITTERS_ON, 1);
    read_sensor_triple_plus_hi_res(pau16_sensorValues, QTR_EMITTERS_ON);

    for(u16_i = 0; u16_i < TRIPLE_HI_RES_SENSOR_NUM; u16_i++) {
        f_line += pau16_sensorValues[u16_i] * (u16_i+1);
    }
    if(f_line == 0.0) {
        return 0.0;
    }
    u16_sum = 0;
    for(u16_i = 0; u16_i < TRIPLE_HI_RES_SENSOR_NUM; u16_i++) {
        u16_sum += pau16_sensorValues[u16_i];
    }
    f_line = f_line/u16_sum;
    return f_line;
}

void follow_line_to_box(float f_maxSpeed) {
    uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM];
    uint16_t u16_position;

    int16_t i16_error;
    int16_t i16_lineCenter;

    uint8_t u8_detectingSensors;
    uint8_t i;

    #ifdef DEBUG_BUILD
    printf("Box...here I come\n");
    #endif

    // Find the center of the line we are constantly trying to stay at
    i16_lineCenter = ((1000 * (TRIPLE_HI_RES_SENSOR_NUM - 1)) / 2);

    while(1) {
        // Get the average position of the line
        u16_position = 1000 * get_line(pau16_sensorValues);
        i16_error = u16_position - i16_lineCenter;
        u8_detectingSensors = 0;

        // Sum up the array
        for (i = 0; i < TRIPLE_HI_RES_SENSOR_NUM; i++) {
            u8_detectingSensors += pau16_sensorValues[i];
        }

        // Check for a box to stop at
        if (check_for_box() == 1) {
            motors_stop();

            #ifdef DEBUG_BUILD
            printf("Yay a box!\n");
            #endif

            return;
        }
        // Check for a left turn
        else if (check_for_left_turn() == 1) {
            #ifdef DEBUG_BUILD
            printf("Left turn!\n");
            #endif

            motors_stop();
            turn_90_degrees(f_maxSpeed, LEFT_DIRECTION);
        }
        // Check for a right turn
        else if (check_for_right_turn() == 1) {
            #ifdef DEBUG_BUILD
            printf("Right turn!\n");
            #endif

            motors_stop();
            turn_90_degrees(f_maxSpeed, RIGHT_DIRECTION);
        }
        // Otherwise just continue along the line correcting movement as needed
        else {
            if (i16_error > 1000) {
                motors_turn_right(f_maxSpeed);
            }
            if (i16_error < -1000) {
                motors_turn_left(f_maxSpeed);
            }
            if ((i16_error >= -1000) && (i16_error <= 1000)) {
                motors_move_forward(f_maxSpeed);
            }
        }
    }
}

void follow_line_to_box_pid(float f_maxSpeed) {
    uint16_t pau16_sensorValues[SENSOR_NUM];
    uint16_t u16_position;

    int16_t i16_proportional;
    int16_t i16_derivative;
    int16_t i16_last_proportional;
    int16_t i16_integral;
    int16_t i16_error;
    int16_t i16_lineCenter;

    uint8_t u8_detectingSensors;
    uint8_t i;

    const int max = 1;

    // Find the center of the line we are constantly trying to stay at
    i16_lineCenter = ((1000 * (SENSOR_NUM - 1)) / 2);

    while(1) {
        // Get the average position of the line
        u16_position = 1000 * get_line(pau16_sensorValues);
        i16_proportional = ((int)u16_position) - 2000;

        // Compute the derivative (change) and integral (sum) of the
        // position.
        i16_derivative = i16_proportional - i16_last_proportional;
        i16_integral += i16_proportional;

        i16_last_proportional = i16_proportional;

        i16_error = u16_position - i16_lineCenter;
        u8_detectingSensors = 0;

        // Sum up the array
        for (i = 0; i < SENSOR_NUM; i++) {
            u8_detectingSensors += pau16_sensorValues[i];
        }

        if (u8_detectingSensors >= SENSOR_NUM - 2) {
            motors_stop();
            return;
        } else {
            // Compute the difference between the two motor power settings,
            // m1 - m2.  If this is a positive number the robot will turn
            // to the right.  If it is a negative number, the robot will
            // turn to the left, and the magnitude of the number determines
            // the sharpness of the turn.
            int power_difference = i16_proportional/20 + i16_integral/10000 + i16_derivative*3/2;
            printf("Power: %i Prop: %i Int: %i, Der: %i\n", power_difference, i16_proportional, i16_integral, i16_derivative);
             power_difference = power_difference / 2;
            // Compute the actual motor settings.  We never set either motor
            // to a negative value.
            if(power_difference > max)
                power_difference = max;
            if(power_difference < -max)
                power_difference = -max;
            if(power_difference < 0){
                printf("Max + power: %i", max+power_difference);
                right_motor_fwd(max+power_difference);
                left_motor_fwd(max);
            }
            else{
                printf("Max - power: %i", max-power_difference);
                right_motor_fwd(max);
                left_motor_fwd(max-power_difference);
            }
        }
    }
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

// Check for a box
uint8_t check_for_box(void) {
    uint16_t pau16_sensorValues[TRIPLE_SENSOR_NUM];
    uint8_t u8_detectingSensors;
    uint8_t i;

    u8_detectingSensors = 0;

    read_sensor_triple(pau16_sensorValues, QTR_EMITTERS_ON);

    for (i = 0; i < TRIPLE_SENSOR_NUM; i++) {
        if (pau16_sensorValues[i] == 1) {
            u8_detectingSensors++;
        }
    }

    if (u8_detectingSensors >= TRIPLE_SENSOR_NUM - 8) {
        return 1;
    } else {
        return 0;
    }
}

// Check for a left turn
uint8_t check_for_left_turn(void) {
    uint16_t pau16_sensorValues[SENSOR_NUM];
    uint8_t u8_detectingSensors;
    uint8_t i;

    u8_detectingSensors = 0;

    read_sensor_array(pau16_sensorValues, QTR_EMITTERS_ON, TRIPLE_LEFT_LINE);

    for (i = 0; i < SENSOR_NUM; i++) {
        if (pau16_sensorValues[i] == 1) {
            u8_detectingSensors++;
        }
    }

    if (u8_detectingSensors >= SENSOR_NUM - 1) {
        return 1;
    } else {
        return 0;
    }
}

// Check for a right turn
uint8_t check_for_right_turn(void) {
    uint16_t pau16_sensorValues[SENSOR_NUM];
    uint8_t u8_detectingSensors;
    uint8_t i;

    u8_detectingSensors = 0;

    read_sensor_array(pau16_sensorValues, QTR_EMITTERS_ON, TRIPLE_RIGHT_LINE);

    for (i = 0; i < SENSOR_NUM; i++) {
        if (pau16_sensorValues[i] == 1) {
            u8_detectingSensors++;
        }
    }

    if (u8_detectingSensors >= SENSOR_NUM - 1) {
        return 1;
    } else {
        return 0;
    }
}

#ifdef DEBUG_BUILD
// Print line follower data
void print_line_data(uint16_t* pau16_sensorValues) {
    int i;
    for (i = 0 ; i < SENSOR_NUM ; ++i)
    {
        printf("%d-", pau16_sensorValues[i]);
    }
}
#endif
