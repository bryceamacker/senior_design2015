#include "pic24_all.h"

#ifndef SERVOS_API_H_
#define SERVOS_API_H_

typedef enum {
    SIMON_RED =     0,
    SIMON_GREEN =   1,
    SIMON_YELLOW =  2,
    SIMON_BLUE =    3,
    RUBIKS =        4,
    ETCH_LEFT =     5,
    ETCH_RIGHT =    6
} servoIDs;

typedef enum {
    RED_BUTTON =    0,
    GREEN_BUTTON =  1,
    YELLOW_BUTTON = 2,
    BLUE_BUTTON =   3
} buttonIDs;

// Servo config
void servo_init(void);

// Servo primitives
void turn_servo(servoIDs id, float degrees);

// Servo usage
void push_simon_button (buttonIDs id);
void turn_servo_clockwise(servoIDs id, float degrees);
void turn_servo_counterwise(servoIDs id, float degrees);

#endif