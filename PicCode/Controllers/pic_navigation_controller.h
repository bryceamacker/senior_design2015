/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: pic_navigation_controller.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: PIC code that will give a user control over all 
* the navigation functions via a serial menu
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        12/5/2014              SECON 2015
*********************************************************************/

/**
 * @brief Handles serial commands
 * 
 * @param  u8_c command to be run
 */
void navigation_serial_command(uint8_t u8_motor);

/**
 * @brief Print out serial menu
 */
void navigation_serial_menu(void);

/**
 * @brief Print out list of functions that a single motor can perform
 */
void single_motor_function_menu(void);

/**
 * @brief Print out list of functions that a pair of motors can perform
 */
void double_motor_function_menu(void);
