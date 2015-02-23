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

uint8_t u8_leftTurnDetection = 0;
uint8_t u8_rightTurnDetection = 0;

stack_t branchedTurnStack;

extern queue_t navigationRoutineQueue;
extern uint8_t u8_routineBlock;
extern routineID u8_currentRoutine;


// Find the center of the line we are constantly trying to stay at
int16_t i16_lineCenter = ((1000 * (TRIPLE_SENSOR_NUM + 1)) / 2);

void line_follower_init() {
    sensor_array_init();
    motors_init();

    branchedTurnStack.top = 0;
    push(&branchedTurnStack, 0);
}

float get_line(uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]) {
    float f_line;
    float f_weight;
    uint16_t u16_i;
    uint16_t u16_sum;

    f_line = 0;
    f_weight = 0;

    // Add up all the sensors that see something
    // Old safe way
    /*
    for(u16_i = 0; u16_i < TRIPLE_HI_RES_SENSOR_NUM; u16_i++) {
        if ((u16_i >= 8) && (u16_i <= 23)) {
            f_line += (pau16_sensorValues[u16_i] * (u16_i+1)) / 2;
        }
        else {
            f_line += pau16_sensorValues[u16_i] * (u16_i+1);
        }
    }
    */

    // New more exciting way
    for (u16_i = 0; u16_i <= 7; u16_i++) {
        f_line += (pau16_sensorValues[u16_i] * (++f_weight));
    }
    for (u16_i = 8; u16_i <= 23; u16_i++) {
        f_line += (pau16_sensorValues[u16_i] * f_weight);
        f_weight += 0.5;
    }
    for (u16_i = 24; u16_i <= 31; u16_i++) {
        f_line += (pau16_sensorValues[u16_i] * (++f_weight));
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
    uint8_t u8_lineContinuationDetected = 0;
    uint8_t u8_turnRan = 0;
    uint8_t u8_branchedFromMainLine = 0;
    uint8_t u8_lastTurn = 0;

    uint16_t pau16_sensorValues[SENSOR_NUM];

    #ifdef DEBUG_BUILD
    printf("Box...here I come\n");
    #endif

    while(1) {
        read_sensor_triple_plus_hi_res(pau16_sensorValues, QTR_EMITTERS_ON);
        if (u8_routineBlock == 0) {
            // If we just got out of a turn and we saw a line during it, we've branched from main line
            if ((u8_turnRan == 1) && (u8_lineContinuationDetected == 1)) {
                #ifdef DEBUG_BUILD
                printf("Branched from the main line\n");
                #endif

                // Keep up with our first turn away from the main line
                push(&branchedTurnStack, u8_lastTurn);

                u8_branchedFromMainLine = 1;
            }
            u8_turnRan = 0;

            // Check for a box to stop at
            if (check_for_box(pau16_sensorValues) == 1) {
                #ifdef DEBUG_BUILD
                printf("Yay a box!\n");
                #endif

                // Found a box, we're done here
                motors_stop();
                return;
            }
            // Check for a left turn
            else if (check_for_left_turn(pau16_sensorValues) == 1) {
                #ifdef DEBUG_BUILD
                printf("Left turn!\n");
                #endif

                // If we've branched from the mainline, keep up with the turns we make
                if (u8_branchedFromMainLine == 1) {
                    push(&branchedTurnStack, LEFT_TURN);
                }
                else {
                    u8_lastTurn = LEFT_DIRECTION;
                }

                // Handle the left turn, flag that we are turning
                handle_left_turn();
            }
            // Check for a right turn
            else if (check_for_right_turn(pau16_sensorValues) == 1) {
                #ifdef DEBUG_BUILD
                printf("Right turn!\n");
                #endif

                // If we've branched from the mainline, keep up with the turns we make
                if (u8_branchedFromMainLine == 1) {
                    push(&branchedTurnStack, RIGHT_TURN);
                }
                else {
                    u8_lastTurn = RIGHT_DIRECTION;
                }

                // Handle the right turn, flag that we are turning
                handle_right_turn();
            }
            // Otherwise just continue along the line correcting movement as needed
            else {
                correct_line_error(f_maxSpeed, pau16_sensorValues);
            }
            u8_lineContinuationDetected = 1;
        }
        // We're reparing for a turn, check to see if the line continues on
        else if (u8_currentRoutine == PREPARE_TURN) {
            if (check_for_line(pau16_sensorValues) == 0) {
                u8_lineContinuationDetected = 0;
            }
            u8_turnRan = 1;
        }
    } // while(1)
} // follow_line_to_box

void follow_line_back_to_main_line(float f_maxSpeed) {
    uint8_t u8_lastTurn;
    uint16_t pau16_sensorValues[SENSOR_NUM];

    #ifdef DEBUG_BUILD
    printf("Headed back to the main line\n");
    #endif

    while(1) {
        read_sensor_triple_plus_hi_res(pau16_sensorValues, QTR_EMITTERS_ON);
        if (u8_routineBlock == 0) {
            // Finding a box while reversing means we've reached the main line
            if (check_for_box(pau16_sensorValues)) {
                #ifdef DEBUG_BUILD
                printf("Back at the main line\n");
                #endif

                // Handle our last turn
                u8_lastTurn = pop(&branchedTurnStack);
                if (u8_lastTurn == LEFT_DIRECTION) {
                    handle_reverse_left_turn();
                }
                if (u8_lastTurn == RIGHT_DIRECTION) {
                    handle_reverse_right_turn();
                }
                return;
            }
            // Check for a left turn
            else if (check_for_left_turn(pau16_sensorValues) == 1) {
                #ifdef DEBUG_BUILD
                printf("Reverse left turn!\n");
                #endif

                // Pop off a past turn and handle it
                pop(&branchedTurnStack);
                handle_reverse_left_turn();
            }
            // Check for a right turn
            else if (check_for_right_turn(pau16_sensorValues) == 1) {
                #ifdef DEBUG_BUILD
                printf("Reverse right turn!\n");
                #endif

                // Pop off a past turn and handle it
                pop(&branchedTurnStack);
                handle_reverse_right_turn();
            }
            // Otherwise just continue along the line correcting movement as needed
            else {
                correct_line_error_reverse(f_maxSpeed, pau16_sensorValues);
            }
        }
    } // while(1)
} // follow_line_back_to_main_line

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

    const int max = f_maxSpeed;
    i16_last_proportional = 0;
    i16_integral = 0;

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
            #ifdef DEBUG_BUILD
            printf("Power: %i Prop: %i Int: %i, Der: %i\n", power_difference, i16_proportional, i16_integral, i16_derivative);
            #endif
            power_difference = power_difference / 2;
            // Compute the actual motor settings.  We never set either motor
            // to a negative value.
            if(power_difference > max)
                power_difference = max;
            if(power_difference < -max)
                power_difference = -max;
            if(power_difference < 0){
                #ifdef DEBUG_BUILD
                printf("Max + power: %i", max+power_difference);
                #endif
                right_motor_fwd(max+power_difference);
                left_motor_fwd(max);
            }
            else{
                #ifdef DEBUG_BUILD
                printf("Max - power: %i", max-power_difference);
                #endif
                right_motor_fwd(max);
                left_motor_fwd(max-power_difference);
            }
        }
    }
}

// Recenter the robot over the line while moving forwards
void correct_line_error(float f_maxSpeed, uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]) {
    uint8_t u8_detectingSensors;
    uint8_t i;

    uint16_t u16_position;

    int16_t i16_error;

    // Get the average position of the line
    u16_position = 1000 * get_line(pau16_sensorValues);
    i16_error = u16_position - i16_lineCenter;
    u8_detectingSensors = 0;

    // Sum up the array
    for (i = 0; i < TRIPLE_HI_RES_SENSOR_NUM; i++) {
        u8_detectingSensors += pau16_sensorValues[i];
    }

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

// Recenter the robot over the line while moving backwards
void correct_line_error_reverse(float f_maxSpeed, uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]) {
    uint8_t u8_detectingSensors;
    uint8_t i;

    uint16_t u16_position;

    int16_t i16_error;

    // Get the average position of the line
    u16_position = 1000 * get_line(pau16_sensorValues);
    i16_error = u16_position - i16_lineCenter;
    u8_detectingSensors = 0;

    // Sum up the array
    for (i = 0; i < TRIPLE_HI_RES_SENSOR_NUM; i++) {
        u8_detectingSensors += pau16_sensorValues[i];
    }

    if (i16_error > 1000) {
        motors_turn_right(f_maxSpeed);
    }
    if (i16_error < -1000) {
        motors_turn_left(f_maxSpeed);
    }
    if ((i16_error >= -1000) && (i16_error <= 1000)) {
        motors_move_reverse(f_maxSpeed);
    }
}

// Check for a box
uint8_t check_for_box(uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]) {
    uint8_t u8_detectingSensors;
    uint8_t i;

    u8_detectingSensors = 0;

    for (i = 0; i < SENSOR_NUM; i++) {
        if (pau16_sensorValues[i] == 1) {
            u8_detectingSensors++;
        }
    }

    if (u8_detectingSensors >= SENSOR_NUM - 1) {
        u8_rightTurnDetection = 0;
        u8_leftTurnDetection = 0;
        return 1;
    } else {
        return 0;
    }

}

// Check for a left turn
uint8_t check_for_left_turn(uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]) {
    uint8_t u8_detectingSensors;
    uint8_t i;

    u8_detectingSensors = 0;

    // The left line is the leftmost 8 sensors
    for (i = 0; i < SENSOR_NUM; i++) {
        if (pau16_sensorValues[i] == 1) {
            u8_detectingSensors++;
        }
    }

    // Check to see if we're detecting a turn
    if (u8_detectingSensors >= SENSOR_NUM - 1) {
        u8_leftTurnDetection++;
    } else {
        u8_leftTurnDetection = 0;
    }

    // If we've detected it enough times then turn
    if (u8_leftTurnDetection >= NUM_OF_REQUIRED_DETECTIONS) {
        u8_leftTurnDetection = 0;
        return 1;
    } else {
        return 0;
    }

}

// Handle left turn
void handle_left_turn(void) {
    motors_stop();

    // Queue all the routines for a left turn
    enqueue(&navigationRoutineQueue, PREPARE_TURN);
    enqueue(&navigationRoutineQueue, LEFT_TURN);
    enqueue(&navigationRoutineQueue, FINISH_TURN);

    // Initiate these routines
    check_for_routine();
}

// Handle a reverse left turn
void handle_reverse_left_turn(void) {
    motors_stop();

    // Detecting a left turn while moving backwards means turning right
    enqueue(&navigationRoutineQueue, PREPARE_REVERSE_TURN);
    enqueue(&navigationRoutineQueue, RIGHT_TURN);
    enqueue(&navigationRoutineQueue, FINISH_REVERSE_TURN);

    // Initiate these routines
    check_for_routine();
}

// Check for a right turn
uint8_t check_for_right_turn(uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]) {
    uint8_t u8_detectingSensors;
    uint8_t i;

    u8_detectingSensors = 0;

    // The right line is the rightmost 8 sensors
    for (i = 24; i <= 31; i++) {
        if (pau16_sensorValues[i] == 1) {
            u8_detectingSensors++;
        }
    }

    // Check to see if we're detecting a turn
    if (u8_detectingSensors >= SENSOR_NUM - 1) {
        u8_rightTurnDetection++;
    } else {
        u8_rightTurnDetection = 0;
    }

    // If we've detected it enough times then turn
    if (u8_rightTurnDetection >= NUM_OF_REQUIRED_DETECTIONS) {
        u8_rightTurnDetection = 0;
        return 1;
    } else {
        return 0;
    }
}

// Handle right turn
void handle_right_turn(void) {
    motors_stop();

    // Queue all the routines for a right turn
    enqueue(&navigationRoutineQueue, PREPARE_TURN);
    enqueue(&navigationRoutineQueue, RIGHT_TURN);
    enqueue(&navigationRoutineQueue, FINISH_TURN);

    // Initiate these routines
    check_for_routine();
}

// Handle a reverse right turn
void handle_reverse_right_turn(void) {
    motors_stop();

    // Detecting a right turn while moving backwards means turning left
    enqueue(&navigationRoutineQueue, PREPARE_REVERSE_TURN);
    enqueue(&navigationRoutineQueue, LEFT_TURN);
    enqueue(&navigationRoutineQueue, FINISH_REVERSE_TURN);

    // Initiate these routines
    check_for_routine();
}

// Check for a line in the center
uint8_t check_for_line(uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]) {
    uint8_t u8_detectingSensors;
    uint8_t i;

    u8_detectingSensors = 0;

    // The center array goes from 8 to 22, but only every other one since the hires is intermingled
    for (i = 9; i <= 23; i+=2) {
        if (pau16_sensorValues[i] == 1) {
            u8_detectingSensors++;
        }
    }

    if (u8_detectingSensors >= SENSOR_NUM - 6) {
        return 1;
    } else {
        return 0;
    }
}
