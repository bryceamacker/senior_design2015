/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: platform_control.c
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Functions to control the platform that holds the
* rubiks and etch playing mechanisms.
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        10/13/2014              SECON 2015
*********************************************************************/

#include "platform_control.h"

void platform_init() {
    platform_up();
    DELAY_MS(500);
    turn_servo_by_pulse(RUBIKS_TWIST, TWIST_COUNTER_PULSE);
}

void platform_up() {
    turn_servo_by_pulse(RUBIKS_PLATFORM, PLATFORM_UP_PULSE);
}

void platform_rubiks() {
    turn_servo_by_pulse(RUBIKS_PLATFORM, PLATFORM_RUBIK_PULSE);
}

void platform_etch() {
    turn_servo_by_pulse(RUBIKS_TWIST,TWIST_DIAG_PULSE);
    turn_servo_by_pulse(RUBIKS_PLATFORM, PLATFORM_ETCH_PULSE);
}
