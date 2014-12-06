/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: pic_navigation_config.c
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

#include "pic_navigation_config.h"

void pic_navigation_init() {
    // Allow the game player to boot up first
    DELAY_MS(5000);

    // Initialize everything to follow a line
    line_follower_init();

    // I2C Config
    configI2C1(400);
}
