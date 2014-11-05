/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: motorsAPI.c
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Functions to help control DC motors
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        9/20/2014               SECON 2015
*********************************************************************/

#include "motorsAPI.h"

/////////////////////////////////////////////// 
//
// Motor config
//
///////////////////////////////////////////////

void motors_init(void)
{
    // Configure timers
    config_motor_timer2();
    config_motor_timer3();
    
    // Configure output comparators
    motor_config_output_compare2();
    motor_config_output_compare3();
    motor_config_output_compare4();
    motor_config_output_compare5();

    // Turn on timers
    T2CONbits.TON = 1;
    T3CONbits.TON = 1;

    // Configure the pins for the motors
    CONFIG_RIGHT_MOTOR_IN1();
    CONFIG_RIGHT_MOTOR_IN2();
    CONFIG_LEFT_MOTOR_IN1();
    CONFIG_LEFT_MOTOR_IN2();
}

void config_motor_timer2(void) {
    T2CON = T2_OFF  
            | T2_IDLE_CON
            | T2_GATE_OFF
            | T2_32BIT_MODE_OFF
            | T2_SOURCE_INT
            | T2_PS_1_64;
    PR2 = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T2CONbits)) - 1;
    //PR2 = PULSEWIDTH1;
    TMR2  = 0;       //clear timer2 value
    _T2IF = 0;
    _T2IP = 1;
    _T2IE = 1;    //enable the Timer2 interrupt
}

void config_motor_timer3(void) {
    T3CON = T3_OFF 
            | T3_IDLE_CON 
            | T3_GATE_OFF
            | T2_32BIT_MODE_OFF
            | T3_SOURCE_INT
            | T3_PS_1_64;
    PR3 = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T3CONbits)) - 1;
    //PR3 = PULSEWIDTH1;
    TMR3  = 0;       //clear timer3 value
    _T3IF = 0;
    _T3IP = 1;
    _T3IE = 1;    //enable the Timer3 interrupt
}

void motor_config_output_compare2(void) {
    T2CONbits.TON = 0;          //disable Timer when configuring Output compare
    //CONFIG_OC2_TO_RP(RB4_RP);   //map OC2 to RB4
    OC2CONbits.OCM2 = 0b110;
    OC2RS = 0;  //clear both registers
    OC2R = 0;
    #ifdef OC2CON1
        //turn on the compare toggle mode using Timer2
        OC2CON1 = OC_TIMER2_SRC |     //Timer2 source
                    OC_PWM_CENTER_ALIGN;  //PWM
        OC2CON2 = OC_SYNCSEL_TIMER2;   //synchronize to timer2
    #else
        //older families, this PWM mode is compatible with center-aligned, OC2R=0
        //as writes to OC2RS sets the pulse widith.
        OC2CON = OC_TIMER2_SRC //Timer3 source
                    | OC_PWM_FAULT_PIN_DISABLE;  //PWM, no fault detection
    #endif
}

void motor_config_output_compare3(void) {
    T2CONbits.TON = 0;          //disable Timer when configuring Output compare
    //CONFIG_OC1_TO_RP(RB4_RP);   //map OC1 to RB4
    OC3CONbits.OCM2 = 0b110;
    OC3RS = 0;  //clear both registers
    OC3R = 0;
    #ifdef OC3CON2
        //turn on the compare toggle mode using Timer3
        OC3CON1 = OC_TIMER2_SRC |     //Timer3 source
                    OC_PWM_CENTER_ALIGN;  //PWM
        OC3CON2 = OC_SYNCSEL_TIMER2;   //synchronize to timer3
    #else
        //older families, this PWM mode is compatible with center-aligned, OC1R=0
        //as writes to OC1RS sets the pulse widith.
        OC3CON = OC_TIMER2_SRC      //Timer2 source
                | OC_PWM_FAULT_PIN_DISABLE;  //PWM, no fault detection
    #endif
}

void motor_config_output_compare4(void) {
    T3CONbits.TON = 0;          //disable Timer when configuring Output compare
    //CONFIG_OC1_TO_RP(RB4_RP);   //map OC1 to RB4
    OC4CONbits.OCM2 = 0b110;
    OC4RS = 0;  //clear both registers
    OC4R = 0;
    #ifdef OC4CON2
        //turn on the compare toggle mode using Timer3
        OC4CON1 = OC_TIMER3_SRC     //Timer3 source
                   | OC_PWM_CENTER_ALIGN;  //PWM
        OC4CON2 = OC_SYNCSEL_TIMER3;   //synchronize to timer3
    #else
        //older families, this PWM mode is compatible with center-aligned, OC1R=0
        //as writes to OC1RS sets the pulse widith.
        OC4CON = OC_TIMER3_SRC     //Timer3 source
                | OC_PWM_FAULT_PIN_DISABLE;  //PWM, no fault detection
    #endif
}

void motor_config_output_compare5(void) {
    T3CONbits.TON = 0;          //disable Timer when configuring Output compare
    //CONFIG_OC1_TO_RP(RB4_RP);   //map OC1 to RB4
    OC5CONbits.OCM2 = 0b110;
    OC5RS = 0;  //clear both registers
    OC5R = 0;
    #ifdef OC5CON2
        //turn on the compare toggle mode using Timer3
        OC5CON1 = OC_TIMER3_SRC    //Timer3 source
                    | OC_PWM_CENTER_ALIGN;  //PWM
        OC5CON2 = OC_SYNCSEL_TIMER3;   //synchronize to timer3
    #else
        //older families, this PWM mode is compatible with center-aligned, OC1R=0
        //as writes to OC1RS sets the pulse widith.
        OC5CON = OC_TIMER3_SRC     //Timer3 source
               | OC_PWM_FAULT_PIN_DISABLE;  //PWM, no fault detection
    #endif
}

void _ISR _T2Interrupt(void) {
    _T2IF = 0;    //clear the timer interrupt bit
}

void _ISR _T3Interrupt(void) {
      _T3IF = 0;    //clear the timer interrupt bit
}

/////////////////////////////////////////////// 
//
// Motor primitives
//
///////////////////////////////////////////////

void left_motor_reverse (float f_duty) {
    LIN1_PULSE = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T2CONbits));
    LIN2_PULSE = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T2CONbits)) * (1-f_duty);
}

void left_motor_fwd (float f_duty) {
    LIN1_PULSE = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T2CONbits)) * (1-f_duty);
    LIN2_PULSE = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T2CONbits));
}

void left_motor_stop() {
    LIN1_PULSE = 0;
    LIN2_PULSE = 0;
}

// Right motor primitive movements
void right_motor_reverse (float f_duty) {
    RIN1_PULSE = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T3CONbits));
    RIN2_PULSE = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T3CONbits)) * (1-f_duty);
}

void right_motor_fwd (float f_duty) {
    RIN1_PULSE = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T3CONbits)) * (1-f_duty);
    RIN2_PULSE = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T3CONbits));
}

void right_motor_stop() {
    RIN1_PULSE = 0;
    RIN2_PULSE = 0;
}

/////////////////////////////////////////////// 
//
// Motor usage
//
///////////////////////////////////////////////

void motors_stop(void) {
    left_motor_stop();
    right_motor_stop();
}

void motors_turn_right(float f_duty) {
    right_motor_reverse(f_duty);
    left_motor_fwd(f_duty);
}

void motors_turn_left(float f_duty) {
    right_motor_fwd(f_duty);
    left_motor_reverse(f_duty);
}

void motors_move_forward(float f_duty) {
    right_motor_fwd(f_duty);
    left_motor_fwd(f_duty);
}

void motors_move_reverse(float f_duty) {
    right_motor_reverse(f_duty);
    left_motor_reverse(f_duty);
}