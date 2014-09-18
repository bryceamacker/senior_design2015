#include "pic24_all.h"
#ifndef _MOTORS_PWM_H_
#define _MOTORS_PWM_H_

void motors_init(void);
void configTimer2(void);
void configTimer3(void);
void configOutputCompare2(void);
void configOutputCompare3(void);
void configOutputCompare4(void);
void configOutputCompare5(void);

void _ISR _T2Interrupt(void);
void _ISR _T3Interrupt(void);

void getMotorValue(void);
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