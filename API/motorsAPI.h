#include "pic24_all.h"
#include <stdio.h>
#include <dataXfer.h>
#include <stdbool.h>

#ifndef _MOTORS_API_H_
#define _MOTORS_API_H_

// Motor config
void motors_init();
void config_timer2(void);
void config_timer3(void);
void config_output_compare2(void);
void config_output_compare3(void);
void config_output_compare4(void);
void config_output_compare5(void);
void _ISR _T2Interrupt(void);
void _ISR _T3Interrupt(void);

// Serial UI for robot movement
void get_motor_value(void);

// Left motor primitives
void left_motor_fwd(float duty);
void left_motor_reverse(float duty);
void left_motor_stop(void);

// Right motor primitives
void right_motor_fwd(float duty);
void right_motor_reverse(float duty);
void right_motor_stop(void);

// Motor usage
void stop(void);
void turn_right(float duty);
void turn_left(float duty);
void move_forward(float duty);
void move_reverse(float duty);

#endif