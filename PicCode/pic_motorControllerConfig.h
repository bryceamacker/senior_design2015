/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: pic_motorControllerConfig.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Configuration for the PIC that controls navigation
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        10/01/2014              SECON 2015
*********************************************************************/

#include "pic24_all.h"
#include "lineFollowerAPI.h"

#ifdef DEBUG_BUILD
#include <stdio.h>
#warning "Motor Controlller: DEBUG BUILD"
#endif

/**
 * @brief Initialize the motor controller pic
 */
void pic_motor_controller_init();

/**
 * @brief Handles serial commands
 * 
 * @param  u8_c command to be run
 */
void motor_controller_serial_command(uint8_t u8_motor);

/**
 * @brief Print out serial menu
 */
void motor_controller_serial_menu(void);

/**
 * @brief Print out list of functions that a single motor can perform
 */
void single_motor_function_menu(void);

/**
 * @brief Print out list of functions that a pair of motors can perform
 */
void double_motor_function_menu(void);
