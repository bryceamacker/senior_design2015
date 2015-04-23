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
int16_t i16_lineCenter = 7000;
int16_t i16_lastLinePosition;


void line_follower_init() {
    sensor_array_init();
    motors_init();

    // Initialize the turn stack
    init_stack(&branchedTurnStack);
}

void load_turn_layout_to_line_follower(uint8_t u8_numberOfTurns, uint8_t* pua8_turnList) {
    u8_numberOfStaticTurns = u8_numberOfTurns;
    pua8_turnStaticList = pua8_turnList;
}

void follow_line_to_box(float f_maxSpeed, char u8_expectedTurn, uint8_t u8_recenterEnable) {
    uint8_t u8_lineContinuationDetected = 0;
    uint8_t u8_turnRan = 0;
    uint8_t u8_branchedFromMainLine = 0;
    uint8_t u8_lastTurn = 0;
    uint8_t u8_handlingTIntersection = 0;
    uint8_t u8_recenterCount = 0;

    uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM];
    uint16_t u16_lateReadingWindow = 0;

    #ifdef DEBUG_BUILD
    printf("Box...here I come\n");
    #endif

    while(1) {
        // Read all four sensor arrays
        read_sensor_triple_plus_hi_res(pau16_sensorValues, QTR_EMITTERS_ON);

        // So long as we're not performing a routine check for things
        if (u8_routineBlock == 0) {

            // We just got done with a turn
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

            // Reset a few variables
            u8_handlingTIntersection = 0;
            u8_turnRan = 0;
            u16_lateReadingWindow = 0;

            // Check for a box to stop at
            if (check_for_box(pau16_sensorValues) == 1) {
                // If our expected turn is a "1" then we're at the finish line
                if (u8_expectedTurn == 1) {
                    // Cross the finish line at we're done
                    enqueue(&navigationRoutineQueue, CROSS_FINISH_LINE);
                    check_for_routine();
                    block_until_all_routines_done();
                    return;
                }

                // If we're not branched from the mainline, check what turn we should do and flag that we have branched, flag that we hit a T (used for reverse)
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
                    }
                    // If the expected turn is right
                    else if (u8_expectedTurn == 'R') {
                        push(&branchedTurnStack, RIGHT_TURN);
                        handle_right_turn(0);
                        enqueue(&navigationRoutineQueue, FINISH_TURN);
                        enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
                        enqueue(&navigationMoveDistanceQueue, LINE_WIDTH*3);
                        block_until_all_routines_done();
                    }

                    // Set status flags appropriately
                    u8_branchedFromMainLine = 1;
                    u8_TIntersection = 1;
                    u8_handlingTIntersection = 1;
                }

                // If we are branched from the mainline, play a game
                else {
                    // Check to see if we were centered over the line when we came in
                    if ((i16_lastLinePosition == i16_lineCenter) || (u8_recenterCount >= NUM_RECENTER_ATTEMPTS) || (u8_recenterEnable == 0)) {
                        #ifdef DEBUG_BUILD
                        printf("Leeeet's play!\n");
                        #endif

                        // Leeeet's play
                        motors_stop();
                        return;
                    }

                    // If not then back up so that it can try and recenter over the line
                    else {
                        #ifdef DEBUG_BUILD
                        printf("Recentering\n");
                        #endif

                        u8_recenterCount++;

                        if (u8_recenterEnable == 1) {
                            enqueue(&navigationRoutineQueue, SIMON_BACKUP);
                            check_for_routine();
                            block_until_all_routines_done();
                        }
                        else if (u8_recenterEnable == 2) {
                            enqueue(&navigationRoutineQueue, CUBE_BACKUP);
                            check_for_routine();
                            block_until_all_routines_done();
                        }
                        else if (u8_recenterEnable == 3) {
                            enqueue(&navigationRoutineQueue, ETCH_BACKUP);
                            check_for_routine();
                            block_until_all_routines_done();
                        }
                        else if (u8_recenterEnable == 4) {
                            enqueue(&navigationRoutineQueue, CARD_BACKUP);
                            check_for_routine();
                            block_until_all_routines_done();
                        }
                    }
                }
            }
            // Check for a left turn
            else if (check_for_left_turn(pau16_sensorValues) == 1) {
                #ifdef DEBUG_BUILD
                printf("Left turn!\n");
                #endif

                // When running a static turn layout if we've exceeded the number of turns then we're at the finish line
                if ((u8_currentTurn > u8_numberOfStaticTurns) && (u8_expectedTurn == 1)) {
                    // Cross the finish line at we're done
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
                // Otherwise just turn and keep up with what turn we just did
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

                // When running a static turn layout if we've exceeded the number of turns then we're at the finish line
                if ((u8_currentTurn > u8_numberOfStaticTurns) && (u8_expectedTurn == 1)) {
                    // Cross the finish line at we're done
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
                // Otherwise just turn and keep up with what turn we just did
                else {
                    handle_right_turn(0);
                    u8_lastTurn = RIGHT_TURN;
                }
            }
            // Otherwise just continue along the line correcting movement as needed
            else {
                correct_line_error_pid(f_maxSpeed, FORWARD_MOVEMENT, pau16_sensorValues);
            }
            u8_lineContinuationDetected = 1;
        }
        // We're reparing for a turn, check to see if the line continues on
        else if ((u8_currentRoutine == PREPARE_TURN) || (u8_currentRoutine == PREPARE_TURN_CURVE)) {
            // Only allow a certain amount of time for late box detection
            u16_lateReadingWindow++;

            // If we lose the line in front of us then it's not a branch fromt the main line
            if (check_for_line(pau16_sensorValues) == 0) {
                u8_lineContinuationDetected = 0;
            }

            // At any time if we've hit a box while in a turn then we've mistaken a box as a turn
            if (check_for_box(pau16_sensorValues) == 1) {
                // If our expected turn is a "1" then we're at the finish line
                if (u8_expectedTurn == 1) {
                    // Cross the finish line at we're done
                    enqueue(&navigationRoutineQueue, CROSS_FINISH_LINE);
                    check_for_routine();
                    block_until_all_routines_done();
                }

                // If we're not branched from the mainline, check what turn we should do and flag that we have branched, flag that we hit a T (used for reverse)
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
                        enqueue(&navigationMoveDistanceQueue, LINE_WIDTH*2);
                        block_until_all_routines_done();
                    }
                    // If the expected turn is right
                    else if (u8_expectedTurn == 'R') {
                        push(&branchedTurnStack, RIGHT_TURN);
                        handle_right_turn(0);
                        enqueue(&navigationRoutineQueue, FINISH_TURN);
                        enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
                        enqueue(&navigationMoveDistanceQueue, LINE_WIDTH*2);
                        block_until_all_routines_done();
                    }

                    // Set status flags appropriately
                    u8_branchedFromMainLine = 1;
                    u8_TIntersection = 1;
                    u8_handlingTIntersection = 1;
                }

                // If we are branched from the mainline and not currently handling a T-intersection, play a game
                if ((u8_handlingTIntersection == 0) && (u8_branchedFromMainLine == 1)) {
                    // Forget the last turn since it was a mistake
                    pop(&branchedTurnStack);
                    u8_currentTurn--;

                    // Clear out the routine queue and stop
                    clear_routines();

                    // Check to see if we were centered over the line when we came in
                    if ((i16_lastLinePosition == i16_lineCenter) || (u8_recenterCount >= NUM_RECENTER_ATTEMPTS) || (u8_recenterEnable == 0)) {
                        #ifdef DEBUG_BUILD
                        printf("Wait no, leeeet's play!\n");
                        #endif

                        // Leeeet's play
                        return;
                    }

                    // If not then back up so that it can try and recenter over the line
                    else {
                        #ifdef DEBUG_BUILD
                        printf("Wait no, recentering\n");
                        #endif

                        u8_recenterCount++;

                        if (u8_recenterEnable == 1) {
                            enqueue(&navigationRoutineQueue, SIMON_BACKUP);
                            check_for_routine();
                            block_until_all_routines_done();
                        }
                        else if (u8_recenterEnable == 2) {
                            enqueue(&navigationRoutineQueue, CUBE_BACKUP);
                            check_for_routine();
                            block_until_all_routines_done();
                        }
                        else if (u8_recenterEnable == 3) {
                            enqueue(&navigationRoutineQueue, ETCH_BACKUP);
                            check_for_routine();
                            block_until_all_routines_done();
                        }
                        else if (u8_recenterEnable == 4) {
                            enqueue(&navigationRoutineQueue, CARD_BACKUP);
                            check_for_routine();
                            block_until_all_routines_done();
                        }
                    }
                }
            }

            // Reset some variables for safekeeping
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
        // Read all four arrays
        read_sensor_triple_plus_hi_res(pau16_sensorValues, QTR_EMITTERS_ON);

        // Move along the line if we aren't performing a routine
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

                // Wait until this turn has finished
                block_until_all_routines_done();
                return;
            }
            // We've encountered a turn
            else if ((check_for_left_turn(pau16_sensorValues) == 1) || (check_for_right_turn(pau16_sensorValues) == 1)) {
                // Pop off a past turn and handle it
                u8_nextTurn = pop(&branchedTurnStack);

                // Check to see if it's the last turn
                u8_lastTurn = (stack_is_empty(branchedTurnStack) == 1) ? 1 : 0;

                // Check which type of turn it is and handle it unless we saw a T-intersection to begin with
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

                // Get back to a line
                if (stack_is_empty(branchedTurnStack) == 0) {
                    reverse_until_line();
                }

                // Maybe the sensors missed the box, check to see if the stack is depleted
                if (stack_is_empty(branchedTurnStack) == 1) {
                    if (u8_TIntersection == 0) {
                        // Compensate for the last turn, don't want to miss anything on the main line
                        enqueue(&navigationRoutineQueue, FINISH_TURN);
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
                correct_line_error_pid(f_maxSpeed, FORWARD_MOVEMENT, pau16_sensorValues);
            }
        }
    } // while(1)
} // follow_line_back_to_main_line

void correct_line_error_pid(float f_maxSpeed, uint8_t u8_direction, uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]) {
    int16_t i16_position;
    int16_t i16_error;
    int16_t i16_motorSpeed;
    int16_t i16_leftMotorSpeed;
    int16_t i16_rightMotorSpeed;

    static int16_t i16_lastError;

    // New motor speeds
    i16_leftMotorSpeed = 0;
    i16_rightMotorSpeed = 0;

    // Get the normalized position of the line from the center and hires arrays
    i16_position = read_line(QTR_EMITTERS_ON);
    if (i16_position == 0) {
        if (check_left_for_line(pau16_sensorValues) == 1) {
            i16_position = 0;
        }
        else if (check_right_for_line(pau16_sensorValues) == 1) {
            i16_position = 15000;
        }
        else {
            i16_position = 7000;
        }
    }

    i16_lastLinePosition = i16_position;

    // Calculate the error and the motorspeed offset using KP and KD
    i16_error = i16_position - i16_lineCenter;
    i16_motorSpeed = KP * i16_error + KD * (i16_error - i16_lastError);

    // Keep up with the previous error
    i16_lastError = i16_error;

    // Calculate the left and right mtor speeds
    if (u8_direction == FORWARD_MOVEMENT) {
        i16_leftMotorSpeed = ((f_maxSpeed/100)*MOTOR_PWM_PERIOD) + i16_motorSpeed;
        i16_rightMotorSpeed = ((f_maxSpeed/100)*MOTOR_PWM_PERIOD) - i16_motorSpeed;
    } else if (u8_direction == BACKWARD_MOVEMENT) {
        i16_leftMotorSpeed = ((f_maxSpeed/100)*MOTOR_PWM_PERIOD) - i16_motorSpeed;
        i16_rightMotorSpeed = ((f_maxSpeed/100)*MOTOR_PWM_PERIOD) + i16_motorSpeed;
    }

    // Set the new motor speed
    set_motors_pid(i16_leftMotorSpeed, i16_rightMotorSpeed, u8_direction);
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

    // If right, left, and center are detecting then there is a box
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

    // Check a few of the center sensors
    for (i = SENSOR_NUM + 1; i < (SENSOR_NUM*2)+1; i+=2) {
        if (pau16_sensorValues[i] == 1) {
            u8_detectingSensors++;
        }
    }

    // Also check for continuity along the sensor arrays by check the left most and center array
    for (i=0; i<=7;i++) {
        if (pau16_sensorValues[i] == 0) {
            if (u8_pastZero == 1) {
                u8_continuityCheck = 0;
            }
            u8_pastZero = 1;
        }
        else {
            u8_pastZero = 0;
        }
    }

    for (i=9; i<=13; i+=2) {
        if (pau16_sensorValues[i] == 0) {
            if (u8_pastZero == 1) {
                u8_continuityCheck = 0;
            }
            u8_pastZero = 1;
        }
        else {
            u8_pastZero = 0;
        }
    }

    // Check to see if we're detecting a turn and if there is continuity
    if ((u8_detectingSensors >= REQUIRED_DETECTING_SENSORS_FOR_TURNS) && (u8_continuityCheck == 1)) {
        return 1;
    } else {
        return 0;
    }
}

// Handle left turn
void handle_left_turn(uint8_t u8_curve) {
    motors_stop();

    // Check to see if we're using a static turn layout
    if (u8_numberOfStaticTurns != 0) {
        handle_turn(pua8_turnStaticList[u8_currentTurn++]);
        #ifdef DEBUG_BUILD
        printf("On static turn %i\n", u8_currentTurn);
        #endif
    } else {
        // Queue all the routines for a left turn
        if ((u8_curve == 0) || (SKIP_ALL_CURVE_TURNS == 1)) {
            handle_turn(NORMAL_LEFT_DEFAULT);
        } else {
            handle_turn(CURVE_LEFT_DEFAULT);
        }
    }
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

    // Check a few of the center sensors
    for (i = SENSOR_NUM + 1; i < (SENSOR_NUM*2)+1; i+=2) {
        if (pau16_sensorValues[i] == 1) {
            u8_detectingSensors++;
        }
    }

    // Also check for continuity along the sensor arrays by check the right most and center array
    for (i=31; i>=24;i--) {
        if (pau16_sensorValues[i] == 0) {
            if (u8_pastZero == 1) {
                u8_continuityCheck = 0;
            }
            u8_pastZero = 1;
        }
        else {
            u8_pastZero = 0;
        }
    }

    for (i=19; i<=23; i+=2) {
        if (pau16_sensorValues[i] == 0) {
            if (u8_pastZero == 1) {
                u8_continuityCheck = 0;
            }
            u8_pastZero = 1;
        }
        else {
            u8_pastZero = 0;
        }
    }

    // Check to see if we're detecting a turn and if there is continuity
    if ((u8_detectingSensors >= REQUIRED_DETECTING_SENSORS_FOR_TURNS) && (u8_continuityCheck == 1)) {
        return 1;
    } else {
        return 0;
    }
}

// Handle right turn
void handle_right_turn(uint8_t u8_curve) {
    motors_stop();

    // Check to see if we're using a static turn layout
    if (u8_numberOfStaticTurns != 0) {
        handle_turn(pua8_turnStaticList[u8_currentTurn++]);
        #ifdef DEBUG_BUILD
        printf("On static turn %i\n", u8_currentTurn);
        #endif
    } else {
        // Queue all the routines for a right turn
        if ((u8_curve == 0) || (SKIP_ALL_CURVE_TURNS == 1)) {
            handle_turn(NORMAL_RIGHT_DEFAULT);
        } else {
            handle_turn(CURVE_RIGHT_DEFAULT);
        }
    }
}

// Reverse until the robot gets back on a line
void reverse_until_line() {
    uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM];

    #ifdef DEBUG_BUILD
    printf("Getting back to a line\n");
    #endif

    // Keep backing up until we see a line and nothing else
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

    // Keep backing up until either a left turn or right turn is detected
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

    // Wait until the center 4 sensors are detecting
    while (u8_centered == 0) {
        read_sensor_hi_res(pau16_sensorValues, QTR_EMITTERS_ON);

        if ((pau16_sensorValues[7] == 1) && (pau16_sensorValues[8] == 1) && (pau16_sensorValues[9] == 1) && (pau16_sensorValues[10] == 1)) {
            u8_centered = 1;
        }
    }
    motors_stop();
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

uint8_t check_left_for_line(uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]) {
    uint8_t u8_detectingSensors;
    uint8_t i;

    u8_detectingSensors = 0;

    for (i = 0; i <= 7; i++) {
        if (pau16_sensorValues[i] == 1) {
            u8_detectingSensors++;
        }
    }

    if (u8_detectingSensors >= 1) {
        return 1;
    } else {
        return 0;
    }
}

uint8_t check_right_for_line(uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM]) {
    uint8_t u8_detectingSensors;
    uint8_t i;

    u8_detectingSensors = 0;

    for (i = 24; i <= 31; i++) {
        if (pau16_sensorValues[i] == 1) {
            u8_detectingSensors++;
        }
    }

    if (u8_detectingSensors >= 1) {
        return 1;
    } else {
        return 0;
    }
}

void handle_turn(uint8_t u8_turn) {
    // Curve left with back up
    if ((u8_turn == CURVE_LEFT_DEFAULT) || (u8_turn == CURVE_LEFT_BACK_UP)) {
        enqueue(&navigationRoutineQueue, PREPARE_TURN_CURVE);
        enqueue(&navigationRoutineQueue, PREPARE_TURN_CURVE);

        enqueue(&navigationRoutineQueue, MOVE_REVERSE_DISTANCE);
        enqueue(&navigationMoveDistanceQueue, PREPARE_CURVE_TURN_DISTANCE);

        enqueue(&navigationRoutineQueue, LEFT_CURVE_TURN);
        enqueue(&navigationRoutineQueue, FINISH_TURN);
    }

    // Curve left with no back up
    else if (u8_turn == CURVE_LEFT_NO_BACK) {
        enqueue(&navigationRoutineQueue, PREPARE_TURN_CURVE);
        enqueue(&navigationRoutineQueue, PREPARE_TURN_CURVE);

        enqueue(&navigationRoutineQueue, MOVE_REVERSE_DISTANCE);
        enqueue(&navigationMoveDistanceQueue, PREPARE_CURVE_TURN_DISTANCE);

        enqueue(&navigationRoutineQueue, LEFT_CURVE_TURN);
    }

    // Normal left with no back up
    else if ((u8_turn == NORMAL_LEFT_DEFAULT) || (u8_turn == NORMAL_LEFT_NO_BACK)) {
        enqueue(&navigationRoutineQueue, PREPARE_TURN);
        enqueue(&navigationRoutineQueue, LEFT_TURN);
    }

    // Normal left with back up
    else if (u8_turn == NORMAL_LEFT_BACK_UP) {
        enqueue(&navigationRoutineQueue, PREPARE_TURN);
        enqueue(&navigationRoutineQueue, LEFT_TURN);
        enqueue(&navigationRoutineQueue, FINISH_TURN);
    }

    // Curve right with back up
    else if ((u8_turn == CURVE_RIGHT_DEFAULT) || (u8_turn == CURVE_RIGHT_BACK_UP)) {
        enqueue(&navigationRoutineQueue, PREPARE_TURN_CURVE);
        enqueue(&navigationRoutineQueue, PREPARE_TURN_CURVE);

        enqueue(&navigationRoutineQueue, MOVE_REVERSE_DISTANCE);
        enqueue(&navigationMoveDistanceQueue, PREPARE_CURVE_TURN_DISTANCE);

        enqueue(&navigationRoutineQueue, RIGHT_CURVE_TURN);
        enqueue(&navigationRoutineQueue, FINISH_TURN);
    }

    // Curve right with no back up
    else if (u8_turn == CURVE_RIGHT_NO_BACK) {
        enqueue(&navigationRoutineQueue, PREPARE_TURN_CURVE);
        enqueue(&navigationRoutineQueue, PREPARE_TURN_CURVE);

        enqueue(&navigationRoutineQueue, MOVE_REVERSE_DISTANCE);
        enqueue(&navigationMoveDistanceQueue, PREPARE_CURVE_TURN_DISTANCE);

        enqueue(&navigationRoutineQueue, RIGHT_CURVE_TURN);
    }

    // Normal right with no back up
    else if ((u8_turn == NORMAL_RIGHT_DEFAULT) || (u8_turn == NORMAL_RIGHT_NO_BACK)) {
        enqueue(&navigationRoutineQueue, PREPARE_TURN);
        enqueue(&navigationRoutineQueue, RIGHT_TURN);
    }

    // Normal right with back up
    else if (u8_turn == NORMAL_RIGHT_BACK_UP) {
        enqueue(&navigationRoutineQueue, PREPARE_TURN);
        enqueue(&navigationRoutineQueue, RIGHT_TURN);
        enqueue(&navigationRoutineQueue, FINISH_TURN);
    }

    check_for_routine();
}

void set_KP(float f_newValue) {
    KP = f_newValue;
}

void set_KD(float f_newValue) {
    KD = f_newValue;
}
