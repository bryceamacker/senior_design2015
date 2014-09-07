#include "pic24_all.h"
#ifndef _MOTORS_H_
#define _MOTORS_H_

void motors_init(void);
void PWM1_duty(float duty);
void PWM2_duty(float duty);
void _ISR _T2Interrupt(void);
void left_motor_fwd(float duty);
void left_motor_reverse(float duty);
void right_motor_fwd(float duty);
void right_motor_reverse(float duty);
void stop(void);
void turn_left(float duty);
void turn_right(float duty);
void forward(float duty);
void reverse(float duty);

#endif

