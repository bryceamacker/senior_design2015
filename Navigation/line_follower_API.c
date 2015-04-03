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

// Detection for turns and Ts
uint8_t u8_leftTurnDetection = 0;
uint8_t u8_rightTurnDetection = 0;
uint8_t u8_TIntersection = 0;
uint8_t u8_turnToFinish = 0;

// Variables to keep up with static turn layouts
uint8_t u8_numberOfStaticTurns = 0;
uint8_t u8_currentTurn = 0;
uint8_t* pua8_turnStaticList = {0};

// Stack to keep up with turns off of the main line
stack_t branchedTurnStack;

// Externs from the motors statck
extern queue_t navigationRoutineQueue;
extern uint8_t u8_routineBlock;
extern uint8_t u8_currentRoutine;

// Values for the PID controller
float KP = KP_DEFAULT;
float KD = KD_DEFAULT;

void line_follower_init() {
    sensor_array_init();
    motors_init();

    init_stack(&branchedTurnStack);
}

void load_turn_layout_to_line_follower(uint8_t u8_numberOfTurns, uint8_t* pua8_turnList) {
    u8_numberOfStaticTurns = u8_numberOfTurns;
    pua8_turnStaticList = pua8_turnList;
}

void follow_line_to_box(float f_maxSpeed, char u8_expectedTurn) {
    uint8_t u8_lineContinuationDetected = 0;
    uint8_t u8_turnRan = 0;
    uint8_t u8_branchedFromMainLine = 0;
    uint8_t u8_lastTurn = 0;
    uint8_t u8_handlingTIntersection = 0;

    uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM];
    uint16_t u16_lateReadingWindow = 0;

    #ifdef DEBUG_BUILD
    printf("Box...here I come\n");
    #endif

    while(1) {
        read_sensor_triple_plus_hi_res(pau16_sensorValues, QTR_EMITTERS_ON);
        if (u8_routineBlock == 0) {
            if (u8_turnRan == 1) {
                // Get back on a line after turning
                reverse_until_line();

                // If we just got out of a turn and we saw a line during it, we've branched from main line
                if (u8_lineContinuationDetected == 1) {
                    #ifdef DEBUG_BUILD
                    printf("Branched from the main line\n");
                    #endif

                    // Keep up with our first turn away from the main line
                    push(&branchedTurnStack, u8_lastTurn);

                    u8_branchedFromMainLine = 1;
                }
            }
            u8_handlingTIntersection = 0;
            u8_turnRan = 0;
            u16_lateReadingWindow = 0;

            // Check for a box to stop at
            if (check_for_box(pau16_sensorValues) == 1) {
                // If we're not branched from the mainline, check what turn we should do and flag that we have branched, flag that we hit a T (used for reverse)
                if (u8_expectedTurn == 1) {
                    enqueue(&navigationRoutineQueue, CROSS_FINISH_LINE);
                    check_for_routine();
                    block_until_all_routines_done();
                    return;
                }
                if (u8_branchedFromMainLine == 0) {
                    #ifdef DEBUG_BUILD
                    printf("T-intersection\n");
                    #endif

                    // If the expected turn is left
                    if (u8_expectedTurn == 'L') {
                        push(&branchedTurnStack, LEFT_TURN);
                        handle_left_turn(0);
                        enqueue(&navigationRoutineQueue, FINISH_TURN);
                        enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
                        enqueue(&navigationMoveDistanceQueue, LINE_WIDTH*3);
                        block_until_all_routines_done();
                    } else {
                        push(&branchedTurnStack, RIGHT_TURN);
                        handle_right_turn(0);
                        enqueue(&navigationRoutineQueue, FINISH_TURN);
                        enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
                        enqueue(&navigationMoveDistanceQueue, LINE_WIDTH*3);
                        block_until_all_routines_done();
                    }
                    u8_branchedFromMainLine = 1;
                    u8_TIntersection = 1;
                    u8_handlingTIntersection = 1;
                }
                // If we are branched from the mainline, play a game
                else {
                    #ifdef DEBUG_BUILD
                    printf("Leeeet's play!\n");
                    #endif

                    // Leeeet's play
                    motors_stop();
                    return;
                }
            }
            // Check for a left turn
            else if (check_for_left_turn(pau16_sensorValues) == 1) {
                #ifdef DEBUG_BUILD
                printf("Left turn!\n");
                #endif

                if ((u8_currentTurn > u8_numberOfStaticTurns) && (u8_expectedTurn == 1)) {
                    enqueue(&navigationRoutineQueue, CROSS_FINISH_LINE);
                    check_for_routine();
                    block_until_all_routines_done();
                    return;
                }

                // If we've branched from the mainline, keep up with the turns we make
                if (u8_branchedFromMainLine == 1) {
                    handle_left_turn(1);
                    push(&branchedTurnStack, LEFT_TURN);
                }
                else {
                    handle_left_turn(0);
                    u8_lastTurn = LEFT_TURN;
                }
            }
            // Check for a right turn
            else if (check_for_right_turn(pau16_sensorValues) == 1) {
                #ifdef DEBUG_BUILD
                printf("Right turn!\n");
                #endif

                if ((u8_currentTurn > u8_numberOfStaticTurns) && (u8_expectedTurn == 1)) {
                    enqueue(&navigationRoutineQueue, CROSS_FINISH_LINE);
                    check_for_routine();
                    block_until_all_routines_done();
                    return;
                }

                // If we've branched from the mainline, keep up with the turns we make
                if (u8_branchedFromMainLine == 1) {
                    push(&branchedTurnStack, RIGHT_TURN);
                    handle_right_turn(1);
                }
                else {
                    handle_right_turn(0);
                    u8_lastTurn = RIGHT_TURN;
                }
            }
            // Otherwise just continue along the line correcting movement as needed
            else {
                correct_line_error_pid(f_maxSpeed, FORWARD_MOVEMENT);
            }
            u8_lineContinuationDetected = 1;
        }
        // We're reparing for a turn, check to see if the line continues on
        else if ((u8_currentRoutine == PREPARE_TURN) || (u8_currentRoutine == PREPARE_TURN_CURVE)) {
            u16_lateReadingWindow++;
            if (check_for_line(pau16_sensorValues) == 0) {
                u8_lineContinuationDetected = 0;
            }
            // At any time if we've hit a box while in a turn then we've mistaken a box as a turn
            if (check_for_box(pau16_sensorValues) == 1){
                // If we're not branched from the mainline, check what turn we should do and flag that we have branched, flag that we hit a T (used for reverse)
                if (u8_expectedTurn == 1) {
                    enqueue(&navigationRoutineQueue, CROSS_FINISH_LINE);
                    check_for_routine();
                    block_until_all_routines_done();
                }
                if ((u8_branchedFromMainLine == 0)  && (u8_handlingTIntersection == 0) && (u16_lateReadingWindow < 100)) {
                    #ifdef DEBUG_BUILD
                    printf("Wait no it's a T-intersection\n");
                    #endif

                    // Clear out the routine queue and stop
                    clear_routines();
                    u8_currentTurn--;

                    // If the expected turn is left
                    if (u8_expectedTurn == 'L') {
                        push(&branchedTurnStack, LEFT_TURN);
                        handle_left_turn(0);
                        enqueue(&navigationRoutineQueue, FINISH_TURN);
                        enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
                        enqueue(&navigationMoveDistanceQueue, LINE_WIDTH*3);
                        block_until_all_routines_done();
                    } else {
                        push(&branchedTurnStack, RIGHT_TURN);
                        handle_right_turn(0);
                        enqueue(&navigationRoutineQueue, FINISH_TURN);
                        enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
                        enqueue(&navigationMoveDistanceQueue, LINE_WIDTH*3);
                        block_until_all_routines_done();
                    }
                    u8_branchedFromMainLine = 1;
                    u8_TIntersection = 1;
                    u8_handlingTIntersection = 1;
                }
                // If we are branched from the mainline, play a game
                if ((u8_handlingTIntersection == 0) && (u8_branchedFromMainLine == 1)) {
                    #ifdef DEBUG_BUILD
                    printf("Wait no, leeeet's play!\n");
                    #endif

                    u8_currentTurn--;
                    // Forget the last turn since it was a mistake
                    pop(&branchedTurnStack);

                    // Clear out the routine queue and stop
                    clear_routines();

                    // Leeeet's play
                    return;
                }
            }
            if (u8_branchedFromMainLine == 1) {
                u8_lineContinuationDetected = 0;
            }
            u8_turnRan = 1;
        }
    } // while(1)
} // follow_line_to_box

void follow_line_back_to_main_line(float f_maxSpeed) {
    uint8_t u8_lastTurn;
    uint8_t u8_lastTurnType;
    uint8_t u8_nextTurn;
    uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM];

    #ifdef DEBUG_BUILD
    printf("Headed back to the main line\n");
    #endif

    while(1) {
        read_sensor_triple_plus_hi_res(pau16_sensorValues, QTR_EMITTERS_ON);
        if (u8_routineBlock == 0) {
            // Finding a box means we've reached the main line
            if (check_for_box(pau16_sensorValues)){
                #ifdef DEBUG_BUILD
                printf("Back at the main line\n");
                #endif

                // Handle our last turn
                u8_lastTurnType = pop(&branchedTurnStack);
                if (u8_lastTurnType == LEFT_TURN) {
                    handle_left_turn(1);
                }
                if (u8_lastTurnType == RIGHT_TURN) {
                    handle_right_turn(1);
                }
                block_until_all_routines_done();
                return;
            }
            // We've encountered a turn
            else if ((check_for_left_turn(pau16_sensorValues) == 1) || (check_for_right_turn(pau16_sensorValues) == 1)) {
                // Pop off a past turn and handle it
                u8_nextTurn = pop(&branchedTurnStack);

                // Check to see if it's the last turn
                u8_lastTurn = (stack_is_empty(branchedTurnStack) == 1) ? 1 : 0;

                // Check which type of turn it is and handle it
                if ((u8_lastTurn == 1) && (u8_TIntersection == 1)) {
                    #ifdef DEBUG_BUILD
                    printf("Passing back over T\n");
                    #endif

                    enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
                    enqueue(&navigationMoveDistanceQueue, LINE_WIDTH*2);
                    check_for_routine();
                }
                else if (u8_nextTurn == RIGHT_TURN) {
                    #ifdef DEBUG_BUILD
                    printf("Right turn!\n");
                    #endif

                    if (u8_lastTurn == 1) {
                        handle_right_turn(0);
                    } else {
                        handle_left_turn(1);
                    }
                }
                else if (u8_nextTurn == LEFT_TURN) {
                    #ifdef DEBUG_BUILD
                    printf("Left turn!\n");
                    #endif

                    if (u8_lastTurn == 1) {
                        handle_left_turn(0);
                    } else {
                        handle_right_turn(1);
                    }
                }
                else {
                    #ifdef DEBUG_BUILD
                    printf("Something went wrong, we shouldn't be here\n");
                    #endif
                }

                // Wait until we turn, then get back on the line
                block_until_all_routines_done();

                if (stack_is_empty(branchedTurnStack) == 0) {
                    reverse_until_line();
                }

                // Maybe the sensors missed the box, check to see if the stack is depleted
                if (stack_is_empty(branchedTurnStack) == 1) {
                    if (u8_TIntersection == 0) {
                        // Compensate for the last turn, don't want to miss anything on the main line
                        enqueue(&navigationRoutineQueue, FINISH_TURN);
                        // reverse_until_branch();
                        enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
                        enqueue(&navigationMoveDistanceQueue, LINE_WIDTH*2);
                        check_for_routine();
                        block_until_all_routines_done();
                    }
                    u8_TIntersection = 0;

                    // We're back at the main line
                    return;
                }
            }
            // Otherwise just continue along the line correcting movement as needed
            else {
                correct_line_error_pid(f_maxSpeed, FORWARD_MOVEMENT);
            }
        }
    } // while(1)
} // follow_line_back_to_main_line

void follow_line_back_to_main_line_reverse(float f_maxSpeed) {
    uint8_t u8_lastTurn;
    uint8_t u8_lastTurnType;
    uint8_t u8_nextTurn;
    uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM];

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
                u8_lastTurnType = pop(&branchedTurnStack);
                if (u8_lastTurnType == LEFT_TURN) {
                    handle_reverse_left_turn(1);
                }
                if (u8_lastTurnType == RIGHT_TURN) {
                    handle_reverse_right_turn(1);
                }
                return;
            }
            // Check for a left turn
            else if ((check_for_left_turn(pau16_sensorValues) == 1) || (check_for_right_turn(pau16_sensorValues) == 1)) {
                // Pop off a past turn and handle it
                u8_nextTurn = pop(&branchedTurnStack);

                // Check to see if it's the last turn
                u8_lastTurn = (stack_is_empty(branchedTurnStack) == 1) ? 1 : 0;

                // Check which type of turn it is and handle it
                if (u8_nextTurn == RIGHT_TURN) {
                    #ifdef DEBUG_BUILD
                    printf("Reverse right turn!\n");
                    #endif

                    handle_reverse_right_turn(u8_lastTurn);
                }
                else if (u8_nextTurn == LEFT_TURN) {
                    #ifdef DEBUG_BUILD
                    printf("Reverse left turn!\n");
                    #endif

                    handle_reverse_left_turn(u8_lastTurn);
                }

                // Maybe the sensors missed the box, check to see if the stack is depleted
                if (stack_is_empty(branchedTurnStack) == 1) {
                    // Wait until this last turn has finished
                    block_until_all_routines_done();
                    return;
                }
            }
            // Otherwise just continue along the line correcting movement as needed
            else {
                correct_line_error_reverse(f_maxSpeed, pau16_sensorValues);
            }
        }
    } // while(1)
} // follow_line_back_to_main_line_reverse

// Recenter the robot over the line while moving forwards
void correct_line_error(float f_maxSpeed, uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]) {
    uint8_t u8_moveForward;
    uint8_t u8_veeredLeft;
    uint8_t u8_veeredRight;
    uint8_t i;

    u8_moveForward = 0;
    u8_veeredLeft = 0;
    u8_veeredRight = 0;

    // 8   10    12    14    16    18    20    22
    //   9    11    13    15    17    19    21    23

    // If our center ones are seeing a line, just move forward
    if ((pau16_sensorValues[14] == 1) && (pau16_sensorValues[15] == 1) && (pau16_sensorValues[16] == 1) && (pau16_sensorValues[17] == 1)) {
        u8_moveForward = 1;
    }

    // Check left of the center
    for (i = 8; i <= 14; i++) {
        if (pau16_sensorValues[i] == 1) {
            u8_veeredLeft++;
        }
    }
    // Check right of the center
    for (i = 17; i <= 23; i++) {
        if (pau16_sensorValues[i] == 1) {
            u8_veeredRight++;
        }
    }

    // Handle what to do
    if (u8_moveForward == 1) {
        motors_move_forward(f_maxSpeed);
    }
    else if (u8_veeredLeft >= 2) {
        motors_turn_left(f_maxSpeed);
    }
    else if (u8_veeredRight >= 2) {
        motors_turn_right(f_maxSpeed);
    }
}

// Recenter the robot over the line while moving backwards
void correct_line_error_reverse(float f_maxSpeed, uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]) {
    uint8_t u8_moveBack;

    uint8_t u8_veeredLeft;
    static uint8_t u8_correctedLeft;

    uint8_t u8_veeredRight;
    static uint8_t u8_correctedRight;

    uint8_t i;

    u8_moveBack = 0;

    u8_veeredLeft = 0;
    u8_correctedLeft = 1;

    u8_veeredRight = 0;
    u8_correctedRight = 1;

    // 8   10    12    14    16    18    20    22
    //   9    11    13    15    17    19    21    23

    // If our center ones are seeing a line, just move forward
    if ((pau16_sensorValues[14] == 1) && (pau16_sensorValues[15] == 1) && (pau16_sensorValues[16] == 1) && (pau16_sensorValues[17] == 1)) {
        u8_moveBack = 1;
    }

    // Check left of the center
    for (i = 8; i <= 13; i++) {
        if (pau16_sensorValues[i] == 1) {
            u8_veeredLeft++;
        }
    }
    // Check right of the center
    for (i = 18; i <= 23; i++) {
        if (pau16_sensorValues[i] == 1) {
            u8_veeredRight++;
        }
    }

    // Once we've no longer veered, we've corrected our path
    if (u8_veeredLeft == 0) {
        u8_correctedLeft = 1;
    }
    if (u8_veeredRight == 0) {
        u8_correctedRight = 1;
    }

    // Handle what to do
    if (u8_moveBack == 1) {
        motors_move_reverse(f_maxSpeed);
    }
    else if ((u8_veeredLeft >= 2) && (u8_correctedLeft == 1)) {
        // Turn right a bit
        motors_turn_right(f_maxSpeed);
        DELAY_MS(50);
        motors_stop();

        // Set as uncrorrected
        u8_correctedLeft = 0;
    }
    else if ((u8_veeredRight >= 2) && (u8_correctedRight == 1)) {
        // Turn left a bit
        motors_turn_left(f_maxSpeed);
        DELAY_MS(50);
        motors_stop();

        // Set as uncrorrected
        u8_correctedRight = 0;
    }
    else {
        motors_move_reverse(f_maxSpeed);
    }
}

void correct_line_error_pid(float f_maxSpeed, uint8_t u8_direction) {
    uint16_t u16_lineCenter;

    int16_t i_position;
    int16_t i_error;
    int16_t i_motorSpeed;
    int16_t i_leftMotorSpeed;
    int16_t i_rightMotorSpeed;

    static int16_t i_lastError;

    i_leftMotorSpeed = 0;
    i_rightMotorSpeed = 0;
    u16_lineCenter = 7000;

    i_position = read_line(QTR_EMITTERS_ON);
    i_error = i_position - u16_lineCenter;

    i_motorSpeed = KP * i_error + KD * (i_error - i_lastError);
    i_lastError = i_error;

    if (u8_direction == FORWARD_MOVEMENT) {
        i_leftMotorSpeed = ((f_maxSpeed/100)*MOTOR_PWM_PERIOD) + i_motorSpeed;
        i_rightMotorSpeed = ((f_maxSpeed/100)*MOTOR_PWM_PERIOD) - i_motorSpeed;
    } else if (u8_direction == BACKWARD_MOVEMENT) {
        i_leftMotorSpeed = ((f_maxSpeed/100)*MOTOR_PWM_PERIOD) - i_motorSpeed;
        i_rightMotorSpeed = ((f_maxSpeed/100)*MOTOR_PWM_PERIOD) + i_motorSpeed;
    }

    set_motors_pid(i_leftMotorSpeed, i_rightMotorSpeed, u8_direction);
}

// Check for a box
uint8_t check_for_box(uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]) {
    uint8_t u8_leftDetectingSensors;
    uint8_t u8_centerDetectingSensors;
    uint8_t u8_rightDetectingSensors;
    uint8_t i;

    u8_leftDetectingSensors = 0;
    u8_centerDetectingSensors = 0;
    u8_rightDetectingSensors = 0;

    // Left side
    for (i = 0; i <= 7; i++) {
        if (pau16_sensorValues[i] == 1) {
            u8_leftDetectingSensors++;
        }
    }

    // The center array goes from 8 to 22, but only every other one since the hires is intermingled
    for (i = 8; i <= 22; i+=2) {
        if (pau16_sensorValues[i] == 1) {
            u8_centerDetectingSensors++;
        }
    }

    // Right side
    for (i = 23; i <= 31; i++) {
        if (pau16_sensorValues[i] == 1) {
            u8_rightDetectingSensors++;
        }
    }

    if ((u8_leftDetectingSensors >= 4) && (u8_centerDetectingSensors >= SENSOR_NUM - 3) && (u8_rightDetectingSensors >= 4)) {
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
    uint8_t u8_continuityCheck;
    uint8_t u8_pastZero;

    u8_continuityCheck = 1;
    u8_pastZero = 0;
    u8_detectingSensors = 0;

    // The left line is the leftmost 8 sensors
    for (i = 0; i < SENSOR_NUM; i++) {
        if (pau16_sensorValues[i] == 1) {
            u8_detectingSensors++;
        }
    }

    for (i = SENSOR_NUM + 1; i < (SENSOR_NUM*2)+1; i+=2) {
        if (pau16_sensorValues[i] == 1) {
            u8_detectingSensors++;
        }
    }

    for (i=0; i<=7;i++) {
        if (pau16_sensorValues[i] == 0) {
            if (u8_pastZero == 1) {
                u8_continuityCheck = 0;
            }
            u8_pastZero = 1;
        }
        u8_pastZero = 0;
    }

    for (i=9; i<=13; i+=2) {
        if (pau16_sensorValues[i] == 0) {
            if (u8_pastZero == 1) {
                u8_continuityCheck = 0;
            }
            u8_pastZero = 1;
        }
        u8_pastZero = 0;
    }

    // Check to see if we're detecting a turn
    if ((u8_detectingSensors >= SENSOR_NUM + 2) && (u8_continuityCheck == 1)) {
        return 1;
    } else {
        return 0;
    }
}

// Handle left turn
void handle_left_turn(uint8_t u8_curve) {
    motors_stop();
    u8_turnToFinish = LEFT_TURN;

    if (u8_numberOfStaticTurns != 0) {
        handle_turn(pua8_turnStaticList[u8_currentTurn++]);
        #ifdef DEBUG_BUILD
        printf("On static turn %i\n", u8_currentTurn);
        #endif
    } else {
        // Queue all the routines for a left turn
        if (u8_curve == 0) {
            handle_turn(NORMAL_LEFT);
        } else {
            handle_turn(CURVE_LEFT);
        }
    }
}

// Handle a reverse left turn
void handle_reverse_left_turn(uint8_t u8_curve) {
    motors_stop();

    // Detecting a left turn while moving backwards means turning right
    if (u8_curve == 0) {
        enqueue(&navigationRoutineQueue, PREPARE_REVERSE_TURN);
        enqueue(&navigationRoutineQueue, RIGHT_TURN);
    } else {
        enqueue(&navigationRoutineQueue, PREPARE_TURN_CURVE);
        enqueue(&navigationRoutineQueue, RIGHT_CURVE_TURN);
    }

    // Initiate these routines
    check_for_routine();
}

// Check for a right turn
uint8_t check_for_right_turn(uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]) {
    uint8_t u8_detectingSensors;
    uint8_t u8_continuityCheck;
    uint8_t u8_pastZero;
    uint8_t i;

    u8_pastZero = 1;
    u8_continuityCheck = 1;
    u8_detectingSensors = 0;

    // The right line is the rightmost 8 sensors
    for (i = 24; i <= 31; i++) {
        if (pau16_sensorValues[i] == 1) {
            u8_detectingSensors++;
        }
    }

    for (i = SENSOR_NUM + 1; i < (SENSOR_NUM*2)+1; i+=2) {
        if (pau16_sensorValues[i] == 1) {
            u8_detectingSensors++;
        }
    }

    for (i=31; i>=24;i--) {
        if (pau16_sensorValues[i] == 0) {
            if (u8_pastZero == 1) {
                u8_continuityCheck = 0;
            }
            u8_pastZero = 1;
        }
        u8_pastZero = 0;
    }

    for (i=19; i<=23; i+=2) {
        if (pau16_sensorValues[i] == 0) {
            if (u8_pastZero == 1) {
                u8_continuityCheck = 0;
            }
            u8_pastZero = 1;
        }
        u8_pastZero = 0;
    }

    // Check to see if we're detecting a turn
    if (u8_detectingSensors >= SENSOR_NUM + 2) {
        return 1;
    } else {
        return 0;
    }
}

// Handle right turn
void handle_right_turn(uint8_t u8_curve) {
    motors_stop();
    u8_turnToFinish = RIGHT_TURN;

    if (u8_numberOfStaticTurns != 0) {
        handle_turn(pua8_turnStaticList[u8_currentTurn++]);
        #ifdef DEBUG_BUILD
        printf("On static turn %i\n", u8_currentTurn);
        #endif
    } else {
        // Queue all the routines for a right turn
        if (u8_curve == 0) {
            handle_turn(NORMAL_RIGHT);
        } else {
            handle_turn(CURVE_RIGHT);
        }
    }
}

// Handle a reverse right turn
void handle_reverse_right_turn(uint8_t u8_curve) {
    motors_stop();

    // Detecting a right turn while moving backwards means turning left
    if (u8_curve == 0) {
        enqueue(&navigationRoutineQueue, PREPARE_REVERSE_TURN);
        enqueue(&navigationRoutineQueue, LEFT_TURN);
    } else {
        enqueue(&navigationRoutineQueue, PREPARE_TURN_CURVE);
        enqueue(&navigationRoutineQueue, LEFT_CURVE_TURN);
    }

    // Initiate these routines
    check_for_routine();
}

// Reverse until the robot gets back on a line
void reverse_until_line() {
    uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM];

    #ifdef DEBUG_BUILD
    printf("Getting back to a line\n");
    #endif


    while(1) {
        motors_move_reverse(BASE_SPEED);
        read_sensor_triple_plus_hi_res(pau16_sensorValues, QTR_EMITTERS_ON);

        // If we see a line, no box, and no turns, then we are back on a line
        if ((check_for_line(pau16_sensorValues) == 1)
        && (check_for_left_turn(pau16_sensorValues) == 0)
        && (check_for_right_turn(pau16_sensorValues) == 0)
        && (check_for_box(pau16_sensorValues) == 0)) {
            return;
        }
    }
}

void reverse_until_branch() {
    uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM];

    #ifdef DEBUG_BUILD
    printf("Getting back to a branch\n");
    #endif

    while(1) {
        motors_move_reverse(BASE_SPEED);
        read_sensor_triple_plus_hi_res(pau16_sensorValues, QTR_EMITTERS_ON);

        // If we see a line, no box, and no turns, then we are back on a line
        if ((check_for_line(pau16_sensorValues) == 1)
        && ((check_for_left_turn(pau16_sensorValues) == 1) || (check_for_right_turn(pau16_sensorValues) == 1))) {
            return;
        }
    }
}

// Check for a line in the center
uint8_t check_for_line(uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]) {
    uint8_t u8_detectingSensors;
    uint8_t i;

    u8_detectingSensors = 0;

    // The center array goes from 8 to 23
    for (i = 9; i <= 23; i++) {
        if (pau16_sensorValues[i] == 1) {
            u8_detectingSensors++;
        }
    }

    if (u8_detectingSensors >= SENSOR_NUM - 7) {
        return 1;
    } else {
        return 0;
    }
}

void turn_until_line_centered(float f_speed, uint8_t u8_direction) {
    uint16_t pau16_sensorValues[SENSOR_NUM*2];
    uint8_t u8_centered = 0;

    if (u8_direction == LEFT_TURN) {
        motors_turn_left(f_speed);

        #ifdef DEBUG_BUILD
        printf("Centering after left turn\n");
        #endif
    } else if (u8_direction == RIGHT_TURN) {
        motors_turn_right(f_speed);

        #ifdef DEBUG_BUILD
        printf("Centering after right turn\n");
        #endif
    }

    while (u8_centered == 0) {
        read_sensor_hi_res(pau16_sensorValues, QTR_EMITTERS_ON);

        if ((pau16_sensorValues[7] == 1) && (pau16_sensorValues[8] == 1) && (pau16_sensorValues[9] == 1) && (pau16_sensorValues[10] == 1)) {
            u8_centered = 1;
        }
    }
    motors_stop();
}

void handle_turn(uint8_t u8_turn) {
    if (u8_turn == CURVE_LEFT) {
        enqueue(&navigationRoutineQueue, PREPARE_TURN_CURVE);
        enqueue(&navigationRoutineQueue, PREPARE_TURN_CURVE);

        enqueue(&navigationRoutineQueue, MOVE_REVERSE_DISTANCE);
        enqueue(&navigationMoveDistanceQueue, PREPARE_CURVE_TURN_DISTANCE);

        enqueue(&navigationRoutineQueue, LEFT_CURVE_TURN);
        enqueue(&navigationRoutineQueue, FINISH_TURN);

    } else if (u8_turn == NORMAL_LEFT) {
        enqueue(&navigationRoutineQueue, PREPARE_TURN);
        enqueue(&navigationRoutineQueue, LEFT_TURN);
        // enqueue(&navigationRoutineQueue, FINISH_TURN);

    } else if (u8_turn == CURVE_RIGHT) {
        enqueue(&navigationRoutineQueue, PREPARE_TURN_CURVE);
        enqueue(&navigationRoutineQueue, PREPARE_TURN_CURVE);

        enqueue(&navigationRoutineQueue, MOVE_REVERSE_DISTANCE);
        enqueue(&navigationMoveDistanceQueue, PREPARE_CURVE_TURN_DISTANCE);

        enqueue(&navigationRoutineQueue, RIGHT_CURVE_TURN);
        enqueue(&navigationRoutineQueue, FINISH_TURN);
    } else if (u8_turn == NORMAL_RIGHT) {
        enqueue(&navigationRoutineQueue, PREPARE_TURN);
        enqueue(&navigationRoutineQueue, RIGHT_TURN);
        // enqueue(&navigationRoutineQueue, FINISH_TURN);
    }
    check_for_routine();
}

void set_KP(float f_newValue) {
    KP = f_newValue;
}

void set_KD(float f_newValue) {
    KD = f_newValue;
}
