/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: pic_navigation_constant_test.c
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Simple program to test the constants for the DC
* motors. Simply moves one meter forward. This helps test the motor
* speed adjustments as well as the encoder adjustments
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        4/2/2015                SECON 2015
*********************************************************************/

#include "pic24_all.h"
#include "line_follower_API.h"
#include "navigation_port_mapping.h"
#include "secon_robot_configuration.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SET_BUTTON_PUSHED     (_RG9 == 0)
#define SET_BUTTON_RELEASED   (_RG9 == 1)

#define DEBOUNCE_DELAY          10

// Function declarations
void pic_navigation_init(void);
void setup_start_button(void);

int main (void) {
    // Configure the motor controller PIC
    configBasic(HELLO_MSG);
    pic_navigation_init();
    setup_start_button();

    // Wait for the start button to be pushed then move a meter
    while(1) {
        if (SET_BUTTON_PUSHED == 1) {
            move_by_distance(1000, BASE_SPEED);

            DELAY_MS(DEBOUNCE_DELAY);
            while(SET_BUTTON_PUSHED);
            DELAY_MS(DEBOUNCE_DELAY);
        }
        doHeartbeat();
    }
}

// Initialization for the navigation PIC
void pic_navigation_init() {
    // Initialize everything to follow a line
    line_follower_init();
}

// Set up the start button
void setup_start_button() {
    CONFIG_RG9_AS_DIG_INPUT();
    ENABLE_RG9_PULLUP();
    DELAY_US(1);
}
