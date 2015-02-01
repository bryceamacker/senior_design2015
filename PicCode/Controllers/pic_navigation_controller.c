/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: pic_navigation_controller.c
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

#include "pic24_all.h"
#include "line_follower_API.h"
#include <string.h>
#include <stdio.h>

// Variable to hold user input
char u8_c;

// Function declarations
void pic_navigation_init();
void navigation_serial_command(uint8_t u8_motor);
void navigation_serial_menu(void);
void single_motor_function_menu(void);
void double_motor_function_menu(void);

// Main loop for the navigation PIC controller using serial commands
int main (void) {
    // Initialize pic and print out serial menu
    configBasic(HELLO_MSG);
    pic_navigation_init();

    navigation_serial_menu();

    // Game playing loop to check serial commands and I2C commands
    while(1) {
        if(isCharReady()) {
            // Handle serial command
            u8_c = inChar();
            navigation_serial_command(u8_c);
            navigation_serial_menu();
        }
        doHeartbeat();
    }
}

// Initialization for the navigation PIC
void pic_navigation_init() {
    // Allow the game player to boot up first
    DELAY_MS(5000);

    // Initialize everything to follow a line
    line_follower_init();

    // I2C Config
    configI2C1(400);
}

///////////////////////////////////////////////
//
// Serial menus for the navigation PIC
//
///////////////////////////////////////////////

void navigation_serial_command(uint8_t u8_motor) {
    uint8_t u8_function;
    uint8_t u8_percentage;
    char sz_buf[32];

    // Get which funtion to perform
    if (u8_motor == 'l') {
        single_motor_function_menu();
    } else if (u8_motor == 'r') {
        single_motor_function_menu();
    } else if (u8_motor == 'b') {
        double_motor_function_menu();
    } else {
        printf("Invalid motor choice\n");
    }
    u8_function = inChar();

    // Get the speed percentage and convert it to a duty cycle, unless it's a stop command
    if (u8_function != 's') {
        printf("\nEnter speed percentage: ");
        inStringEcho(sz_buf,31);
        sscanf(sz_buf,"%hhu",(uint8_t *) &u8_percentage);
    }

    // Perform the given function on the given motor
    switch(u8_function) {
        case 'f':
            if (u8_motor == 'l') {
                left_motor_fwd(u8_percentage/100.0);
            } else if (u8_motor == 'r') {
                right_motor_fwd(u8_percentage/100.0);
            } else if (u8_motor == 'b') {
                motors_move_forward(u8_percentage/100.0);
            }
            break;
        case 'b':
            if (u8_motor == 'l') {
                left_motor_reverse(u8_percentage/100.0);
            } else if (u8_motor == 'r') {
                right_motor_reverse(u8_percentage/100.0);
            } else if (u8_motor == 'b') {
                motors_move_reverse(u8_percentage/100.0);
            }
            break;
        case 'r':
            motors_turn_right(u8_percentage/100.0);
            break;
        case 'l':
            motors_turn_left(u8_percentage/100.0);
            break;
        case 's':
            if (u8_motor == 'l') {
                left_motor_stop();
            } else if (u8_motor == 'r') {
                right_motor_stop();
            } else if (u8_motor == 'b') {
                motors_stop();
            }
            break;
        case 'a':
            print_sensor_array();
            break;
        default:
            break;
    }
}

void navigation_serial_menu() {
    printf("\nChoose a command\n");
    printf("   Press 'l' to control the left motor\n");
    printf("   Press 'r' to control the right motor\n");
    printf("   Press 'b' to control both motors\n");
    printf("   Press 'a' to print out the sensor array values\n");
    printf("   Press 'q' to quit menu and wait for start light\n");
}

void single_motor_function_menu() {
    printf("\nChoose a motor function\n");
    printf("   Press 'f' for forward\n");
    printf("   Press 'b' for back\n");
    printf("   Press 's' for stop\n");
}

void double_motor_function_menu() {
    printf("\nChoose a motor function\n");
    printf("   Press 'f' for forward\n");
    printf("   Press 'b' for back\n");
    printf("   Press 'r' for right turn\n");
    printf("   Press 'l' for left turn\n");
    printf("   Press 's' for stop\n");
}
