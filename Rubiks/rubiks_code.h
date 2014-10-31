/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: rubiks_code.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Functions and declarations to control the 
* mechanisms that twist the rubiks cube
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        9/26/2014               SECON 2015
*********************************************************************/

#include "servosAPI.h"
#include "port_mapping.h"
#include "platform_control.h"

#ifndef RUBIKS_CODE_H_
#define RUBIKS_CODE_H_

#define TWIST_CLOCK_PULSE       950
#define TWIST_COUNTER_PULSE     2050
#define WAIT_TIME               1500

/////////////////////////////////////////////// 
//
// Rubiks Usage
//
///////////////////////////////////////////////

/**
 * @brief Intialize platform up and twist to untwisted
 */
void rubik_init(void);

/**
 * @brief Lower platform, twist cube, and then raise platform
 */
void play_rubiks(void);

/////////////////////////////////////////////// 
//
// Rubiks primitives
//
///////////////////////////////////////////////

/**
 * @brief Twist the rubiks cube
 */
void twist_rubiks_clock(void);

/**
 * @brief Twist the rubiks cube
 */
void twist_rubiks_counter(void);

#endif