/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: platform_control.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Declarations and constants for controlling the
* platform which holds the rubiks twisting servo and the etch drawing
* servos.
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        10/13/2014              SECON 2015
*********************************************************************/

#include "servosAPI.h"
#include "port_mapping.h"

#ifndef PLATFORM_CONTROL_H_
#define PLATFORM_CONTROL_H_

#define PLATFORM_UP_PULSE       2050
#define PLATFORM_RUBIK_PULSE    1550
#define PLATFORM_ETCH_PULSE     900

#define TWIST_COUNTER_PULSE     2050
#define TWIST_DIAG_PULSE        1350

/////////////////////////////////////////////// 
//
// Platform config
//
///////////////////////////////////////////////
/**
 * @brief Initialize the platform
 */
void platform_init(void);

/////////////////////////////////////////////// 
//
// Platform usage
//
///////////////////////////////////////////////
/** * @brief Raise the platform to it's intial position
 */
void platform_up(void);

/**
 * @brief Lower the platform to twist the cube
 */
void platform_rubiks(void);

/**
 * @brief Move twist and platform to position etch servos
 */
void platform_etch(void);

#endif