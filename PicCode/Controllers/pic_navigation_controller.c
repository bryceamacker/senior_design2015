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

extern queue_t navigationRoutineQueue;

// Function declarations
void pic_navigation_init();
void navigation_serial_command(uint8_t u8_motor);
void motor_control(uint8_t u8_motor, uint8_t u8_function);
void navigation_serial_menu(void);
void single_motor_function_menu(void);
void double_motor_function_menu(void);
void sensor_array_menu(void);
void sensor_array_print(uint8_t u8_sensorArray);
void print_get_line(void);
void navigate_course(void);

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

// Handle top level serial commands
void navigation_serial_command(uint8_t u8_command) {
    uint8_t u8_function;
    uint8_t u8_sensorArray;

    switch(u8_command) {
        case 'l':
            single_motor_function_menu();
            u8_function = inChar();
            motor_control(u8_command, u8_function);
            break;
        case 'r':
            single_motor_function_menu();
            u8_function = inChar();
            motor_control(u8_command, u8_function);
            break;
        case 'b':
            double_motor_function_menu();
            u8_function = inChar();
            motor_control(u8_command, u8_function);
            break;
        case 'a':
            sensor_array_menu();
            u8_sensorArray = inChar();
            sensor_array_print(u8_sensorArray);
            break;
        case 'c':
            calibrate(QTR_EMITTERS_ON, 1);
            calibrate(QTR_EMITTERS_ON, 2);
            calibrate(QTR_EMITTERS_ON, 3);
            calibrate(QTR_EMITTERS_ON, 4);
            break;
        case 'g':
            print_get_line();
            break;
        case 'n':
            follow_line_to_box(BASE_SPEED);
            break;
        case 'h':
            printf("\nChoose a direction to turn\n");
            printf("   Press 'l' turn left\n");
            printf("   Press 'r' turn right\n");
            u8_c = inChar();

            if (u8_c == 'r') {
                printf("Turning 90 degrees right\n");
                enqueue(&navigationRoutineQueue, PREPARE_TURN);
                enqueue(&navigationRoutineQueue, RIGHT_TURN);
                enqueue(&navigationRoutineQueue, FINISH_TURN);
                check_for_routine();
            }
            else if (u8_c == 'l') {
                printf("Turning 90 degrees left\n");
                enqueue(&navigationRoutineQueue, PREPARE_TURN);
                enqueue(&navigationRoutineQueue, LEFT_TURN);
                enqueue(&navigationRoutineQueue, FINISH_TURN);
                check_for_routine();
            }
            else {
                printf("Invalid Choice\n");
            }
            break;
        case 'w':
            navigate_course();
            break;
        default:
            printf("Invalid Choice\n");
            break;
    }
}

// Handle motor control functions
void motor_control(uint8_t u8_motor, uint8_t u8_function) {
    uint8_t u8_percentage;
    uint8_t u8_c2;

    uint16_t u16_revolutions;
    uint16_t u16_distance;

    int16_t i16_revolutions;
    int16_t i16_distance;

    char sz_buf[32];

    // Get the speed percentage and convert it to a duty cycle, unless it's a stop command
    if (u8_function != 's') {
        printf("\nEnter speed percentage: ");
        inStringEcho(sz_buf,31);
        sscanf(sz_buf,"%hhu",(uint8_t *) &u8_percentage);
        u8_c2 = inChar();
    }

    // Perform the given function on the given motor
    switch(u8_function) {
        case 'f':
            if (u8_motor == 'l') {
                left_motor_fwd(u8_percentage);
            } else if (u8_motor == 'r') {
                right_motor_fwd(u8_percentage);
            } else if (u8_motor == 'b') {
                motors_move_forward(u8_percentage);
            }
            break;
        case 'b':
            if (u8_motor == 'l') {
                left_motor_reverse(u8_percentage);
            } else if (u8_motor == 'r') {
                right_motor_reverse(u8_percentage);
            } else if (u8_motor == 'b') {
                motors_move_reverse(u8_percentage);
            }
            break;
        case 'r':
            motors_turn_right(u8_percentage);
            break;
        case 'l':
            motors_turn_left(u8_percentage);
            break;
        case 'o':
            printf("\nEnter number of tenths of revolutions\n");
            inStringEcho(sz_buf,31);
            sscanf(sz_buf,"%u", (uint16_t *) &u16_revolutions);
            u8_c2 = inChar();

            printf("\n(f)orward or (b)ack\n");
            u8_c = inChar();

            if (u8_c == 'b') {
                i16_revolutions = 0 - u16_revolutions;
            }
            else {
                i16_revolutions = u16_revolutions;
            }

            if (u8_motor == 'l') {
                printf("Turning left motor by %u tenths of revolutions\n", u16_revolutions);
                move_left_motor_by_revolutions(i16_revolutions/10.0, u8_percentage);
            } else if (u8_motor == 'r') {
                printf("Turning left motor by %u tenths of revolutions\n", u16_revolutions);
                move_right_motor_by_revolutions(i16_revolutions/10.0, u8_percentage);
            } else if (u8_motor == 'b') {
                printf("Turning motors by %u tenths of revolutions\n", u16_revolutions);
                move_by_revolutions(i16_revolutions/10.0, u8_percentage);
            }
            break;
        case 'd':
            printf("\nEnter distance in mm\n");
            inStringEcho(sz_buf,31);
            sscanf(sz_buf,"%u", (uint16_t *) &u16_distance);
            u8_c2 = inChar();

            printf("\n(f)orward or (b)ack\n");
            u8_c = inChar();

            if (u8_c == 'b') {
                i16_distance = 0 - u16_distance;
            }
            else {
                i16_distance = u16_distance;
            }

            if (u8_motor == 'l') {
                printf("Turning left motor by %u mm\n", u16_distance);
                move_left_motor_by_distance(i16_distance*1.0, u8_percentage);
            } else if (u8_motor == 'r') {
                printf("Turning right motor by %u mm\n", u16_distance);
                move_right_motor_by_distance(i16_distance*1.0, u8_percentage);
            } else if (u8_motor == 'b') {
                printf("Turning motors by %u mm\n", u16_distance);
                move_by_distance(i16_distance*1.0, u8_percentage);
            }
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
        default:
            break;
    }
}

// Main serial menu for navigation pic
void navigation_serial_menu() {
    printf("\nChoose a command\n");
    printf("   Press 'l' to control the left motor\n");
    printf("   Press 'r' to control the right motor\n");
    printf("   Press 'b' to control both motors\n");
    printf("   Press 'a' to print out the sensor array values\n");
    printf("   Press 'c' to recalibrate all the sensor arrays\n");
    printf("   Press 'g' to get line continuously and print line value\n");
    printf("   Press 'n' to navigate to a box\n");
    printf("   Press 'h' to turn 90 degrees\n");
    printf("   Press 'w' to navigate the whole course (skips the game stuff)\n");
}

// Menu for contorlling a single motor
void single_motor_function_menu() {
    printf("\nChoose a motor function\n");
    printf("   Press 'f' for forward\n");
    printf("   Press 'b' for back\n");
    printf("   Press 'o' to move by revolution\n");
    printf("   Press 'd' to move by distance\n");
    printf("   Press 's' for stop\n");
}

// Menu for contorlling both motors
void double_motor_function_menu() {
    printf("\nChoose a motor function\n");
    printf("   Press 'f' for forward\n");
    printf("   Press 'b' for back\n");
    printf("   Press 'r' for right turn\n");
    printf("   Press 'l' for left turn\n");
    printf("   Press 'o' to move by revolution\n");
    printf("   Press 'd' to move by distance\n");
    printf("   Press 's' for stop\n");
}

// Menu for all the sensor arrays
void sensor_array_menu() {
    printf("\nChoose a sensor array to print\n");
    printf("   Press 'a' for forward\n");
    printf("   Press '1' for line 1\n");
    printf("   Press '2' for line 2\n");
    printf("   Press '3' for line 3\n");
    printf("   Press '4' for line 4\n");
    printf("   Press '5' for line 5\n");
    printf("   Press 't' for the triple\n");
    printf("   Press 'h' for the hi-res\n");
    printf("   Press 'p' for the triple plus the hi-res\n");
    printf("   Press 'b' for back line\n");
}

// Print a certain line sensor array over and over
void sensor_array_print(uint8_t u8_sensorArray) {
    while(isCharReady() == 0) {
        switch(u8_sensorArray) {
            case 'a':
                print_all_sensor_arrays();
                break;
            case '1':
                print_sensor_array(1);
                break;
            case '2':
                print_sensor_array(2);
                break;
            case '3':
                print_sensor_array(3);
                break;
            case '4':
                print_sensor_array(4);
                break;
            case '5':
                print_sensor_array(5);
                break;
            case 't':
                print_sensor_triple();
                break;
            case 'h':
                print_sensor_hi_res();
                break;
            case 'p':
                print_sensor_triple_plus_hi_res();
                break;
            case 'b':
                print_sensor_back();
                break;
        }
        doHeartbeat();
    }
}

// Print the calculated line position
void print_get_line() {
    uint16_t pau16_sensorValues[TRIPLE_HI_RES_SENSOR_NUM];
    uint16_t u16_position;

    while(isCharReady() == 0) {
        read_sensor_triple_plus_hi_res(pau16_sensorValues, QTR_EMITTERS_ON);
        u16_position = 1000 * get_line(pau16_sensorValues);
        printf("Line position: %u\n", u16_position);
    }
}

// Navigate an entire course to the finish line
void navigate_course() {
    // Game counter
    uint8_t u8_currentGame;

    u8_currentGame = 0;

    enqueue(&navigationRoutineQueue, MOVE_PAST_START_BOX);
    check_for_routine();

    // Play Rubiks, Etch, and Simon then stop
    while(u8_currentGame <= 3) {
        // Find a box
        follow_line_to_box(BASE_SPEED);

        // Just a delay to make it obvious that we've reached a box
        DELAY_MS(2500);

        // Get back to the main line
        follow_line_back_to_main_line(BASE_SPEED);
        motors_stop();
        u8_currentGame++;
    }

    // Get to the finish line
    follow_line_to_box(BASE_SPEED);
}
