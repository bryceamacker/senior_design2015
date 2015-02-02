/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: pic_game_player_controller.c
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: PIC code that will give a user control over all
* the game player functions via a serial menu
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        12/5/2014               SECON 2015
*********************************************************************/

#include "pic24_all.h"
#include "motors_API.h"
#include "SSDisplayAPI.h"
#include "etch_code.h"
#include "rubiks_code.h"
#include "simon_code.h"
#include "card_code.h"
#include "platform_control.h"
#include "game_arm_control.h"
#include <string.h>
#include <stdio.h>

// Varibles for input and to hold the current status of the robot
char u8_c;
uint8_t u8_platformPos;
uint8_t u8_twistPos;
uint8_t u8_platformFlipped;
uint16_t u16_pwm;

// Function Declarations
void pic_game_player_init(void);
void game_player_serial_command(uint8_t u8_c);
void game_player_serial_menu(void);
void game_player_servo_menu(void);
void simon_menu(void);
void game_player_set_servo(char u8_servo);
void start_light_print(void);

// Main loop for the game player pic using I2C command
int main(void) {
    // Initialize pic and print out serial menu
    configBasic(HELLO_MSG);
    pic_game_player_init();

    u8_twistPos = 0;
    u8_platformPos = 0;
    u8_platformFlipped = 0;

    game_player_serial_menu();

    // Game playing loop to check serial commands and I2C commands
    while(1) {
        if(isCharReady()) {
            // Handle serial command
            u8_c = inChar();
            game_player_serial_command(u8_c);
            game_player_serial_menu();
        }
        doHeartbeat();
    }
}

// Controller initialization
void pic_game_player_init() {
    // Initialize all the timers and comparators for the servos
    servo_init();

    // Delay to let the configurations to take place
    DELAY_MS(500);

    // Initialize the game arm
    game_arm_init();

    // Initialize all the servos to their starting position
    rubik_init();
    etch_init();
    simon_init();

    // Photo cell init
    photo_cell_init();

    // SSD init
    ssd_init();
}

///////////////////////////////////////////////
//
// Serial menus for the game player PIC
//
///////////////////////////////////////////////

void game_player_serial_command(uint8_t u8_c) {
    char u8_servo;
    if (u8_c == 'e') {
        play_etch();
    } else if (u8_c == 'c') {
        play_rubiks();
    } else if (u8_c == 's') {
        play_simon();
    } else if (u8_c == 'd') {
        play_card();
    } else if (u8_c == 'p') {
        if (u8_platformPos == 0) {
            u8_platformPos = 1;
            printf("\n*** Lowering platform ***\n");
            platform_rubiks();
        } else {
            u8_platformPos = 0;
            printf("\n*** Raising platform ***\n");
            platform_up();
        }
    } else if (u8_c == 't') {
        printf("\n*** Twisting rubiks twist ***\n");
        if (u8_twistPos == 0) {
            u8_twistPos = 1;
            twist_rubiks_clock();
        } else {
            u8_twistPos = 0;
            twist_rubiks_counter();
        }
    } else if (u8_c == 'f') {
        if (u8_platformFlipped == 0) {
            u8_platformFlipped = 1;
            printf("\n*** Flipping back platform for Etch ***\n");
            platform_etch();
        } else {
            u8_platformFlipped = 0;
            printf("\n*** Reseting platform to original position ***\n");
            platform_etch_undo();
        }
    } else if (u8_c == 'r') {
        simon_menu();
        u8_servo = inChar();

        if (u8_servo == 'a') {
            printf("\n*** Retracting all buttons ***\n");
            simon_retract_buttons();
        } else if (u8_servo == 'y') {
            printf("\n*** Retracting yellow button ***\n");
            simon_retract_button(YELLOW_BUTTON);
        } else if (u8_servo == 'b') {
            printf("\n*** Retracting blue button ***\n");
            simon_retract_button(BLUE_BUTTON);
        } else if (u8_servo == 'r') {
            printf("\n*** Retracting red button ***\n");
            simon_retract_button(RED_BUTTON);
        } else if (u8_servo == 'g') {
            printf("\n*** Retracting green button ***\n");
            simon_retract_button(GREEN_BUTTON);
        } else if (u8_servo == 's') {
            printf("\n*** Retracting start button ***\n");
            simon_retract_button(START_BUTTON);
        } else {
            printf("Invalid command");
        }
    } else if (u8_c == 'h') {
        simon_menu();
        u8_servo = inChar();

        if (u8_servo == 'a') {
            printf("\n*** Hovering all buttons ***\n");
            simon_hover_buttons();
        } else if (u8_servo == 'y') {
            printf("\n*** Hovering yellow button ***\n");
            simon_hover_button(YELLOW_BUTTON);
        } else if (u8_servo == 'b') {
            printf("\n*** Hovering blue button ***\n");
            simon_hover_button(BLUE_BUTTON);
        } else if (u8_servo == 'r') {
            printf("\n*** Hovering red button ***\n");
            simon_hover_button(RED_BUTTON);
        } else if (u8_servo == 'g') {
            printf("\n*** Hovering green button ***\n");
            simon_hover_button(GREEN_BUTTON);
        } else if (u8_servo == 's') {
            printf("\n*** Hovering start button ***\n");
            simon_hover_button(START_BUTTON);
        } else {
            printf("Invalid command");
        }
    } else if (u8_c == 'b') {
        simon_menu();
        u8_servo = inChar();

        if (u8_servo == 'a') {
            printf("\n*** Pushing all buttons ***\n");
            simon_push_buttons();
        } else if (u8_servo == 'y') {
            printf("\n*** Pushing yellow button ***\n");
            simon_push_button(YELLOW_BUTTON);
        } else if (u8_servo == 'b') {
            printf("\n*** Pushing blue button ***\n");
            simon_push_button(BLUE_BUTTON);
        } else if (u8_servo == 'r') {
            printf("\n*** Pushing red button ***\n");
            simon_push_button(RED_BUTTON);
        } else if (u8_servo == 'g') {
            printf("\n*** Pushing green button ***\n");
            simon_push_button(GREEN_BUTTON);
        } else if (u8_servo == 's') {
            printf("\n*** Pushing start button ***\n");
            simon_push_button(START_BUTTON);
        } else {
            printf("Invalid command");
        }
    } else if (u8_c == 'n') {
        simon_menu();
        u8_servo = inChar();

        if (u8_servo == 'a') {
            printf("\n*** Pushing and hover all buttons ***\n");
            simon_push_and_hover_buttons();
        } else if (u8_servo == 'y') {
            printf("\n*** Pushing and hover yellow button ***\n");
            simon_push_and_hover_button(YELLOW_BUTTON);
        } else if (u8_servo == 'b') {
            printf("\n*** Pushing and hover blue button ***\n");
            simon_push_and_hover_button(BLUE_BUTTON);
        } else if (u8_servo == 'r') {
            printf("\n*** Pushing and hover red button ***\n");
            simon_push_and_hover_button(RED_BUTTON);
        } else if (u8_servo == 'g') {
            printf("\n*** Pushing and hover green button ***\n");
            simon_push_and_hover_button(GREEN_BUTTON);
        } else if (u8_servo == 's') {
            printf("\n*** Pushing and hover start button ***\n");
            simon_push_and_hover_button(START_BUTTON);
        } else {
            printf("Invalid command");
        }
    } else if (u8_c == 'z') {
        photo_trans_print();
    } else if (u8_c == 'l') {
        start_light_print();
    } else if (u8_c == 'v') {
        test_ss_displays();
    } else if (u8_c == 'x') {
        // Set a sepcific servo
        game_player_servo_menu();
        u8_servo = inChar();
        game_player_set_servo(u8_servo);
    } else {
        printf("Invalid command");
    }
}

void game_player_serial_menu(void) {
    printf("\nChoose a command\n");
    printf("   Press 'e' to play Etch-a-Sketch\n");
    printf("   Press 'c' to spin Rubiks\n");
    printf("   Press 's' to play Simon\n");
    printf("   Press 'd' to pick up a card from the deck\n");
    if (u8_platformPos == 0) {
        printf("   Press 'p' to raise platform\n");
    } else {
        printf("   Press 'p' to lower platform\n");
    }
    printf("   Press 't' to spin rubiks twist servo\n");
    if (u8_platformFlipped == 0) {
        printf("   Press 'f' to prepare for etch\n");
    } else {
        printf("   Press 'f' to reset platform to init\n");
    }
    printf("   Press 'r' to retract Simon arms\n");
    printf("   Press 'h' to hover Simon arms\n");
    printf("   Press 'b' to push Simon buttons\n");
    printf("   Press 'n' to push and hover Simon buttons\n");
    printf("   Press 'm' motor control\n");
    printf("   Press 'z' to read photo transistors\n");
    printf("   Press 'l' to read start light resistor\n");
    printf("   Press 'v' to test the displays\n");
    printf("   Press 'x' to set a servo\n");
}

void game_player_servo_menu(void) {
    printf("\nChoose a servo\n");
    printf("   v) Etch vertical\n");
    printf("   h) Etch horizontal\n");
    printf("   p) Platform\n");
    printf("   t) Rubiks twist\n");
    printf("   y) Simon yellow\n");
    printf("   b) Simon blue\n");
    printf("   r) Simon red\n");
    printf("   g) Simon green\n");
    printf("   s) Arm slide\n");
    printf("   a) Arm positions\n");
}

void simon_menu(void) {
    printf("\nChoose Simon arm\n");
    printf("   a) All arms\n");
    printf("   y) Yellow arm\n");
    printf("   b) Blue arm\n");
    printf("   r) Red arm\n");
    printf("   g) Green arm\n");
    printf("   s) Start arm\n");
}

void game_player_set_servo(char u8_servo) {
    uint16_t u16_pwm;
    char sz_buf[32];
    char u8_c;
    u8_c = 0;

    // Get pulse width for non-continuous servos
    if (u8_servo != '0' && u8_servo != '1') {
        printf("\nEnter pulse width: ");
        inStringEcho(sz_buf,31);
        sscanf(sz_buf,"%d",(uint16_t *) &u16_pwm);
    }

    // Set servo appropriately
    if (u8_servo == 'v') {
        printf("\nUse 'a' and 'd' to move servo clockwise and counter clockwise and 'q' to quit");
        while (u8_c != 'q') {
            u8_c = inChar();
            if (u8_c == 'a') {
                step_servo(0, ETCH_VERTICAL);
            } else if (u8_c == 'd') {
                step_servo(1, ETCH_VERTICAL);
            } else {
                stop_servo(ETCH_VERTICAL);
            }
        }
    } else if (u8_servo == 'h') {
        printf("\nUse 'a' and 'd' to move servo and 'q' to quit");
        while (u8_c != 'q') {
            u8_c = inChar();
            if (u8_c == 'a') {
                step_servo(0, ETCH_HORIZ);
            } else if (u8_c == 'd') {
                step_servo(1, ETCH_HORIZ);
            } else {
                stop_servo(ETCH_HORIZ);
            }
        }
    } else if (u8_servo == 'p') {
        turn_servo_by_pulse(RUBIKS_PLATFORM, u16_pwm);
        printf("\n*** Setting platform to %u ***\n", u16_pwm);
    } else if (u8_servo == 't') {
        turn_servo_by_pulse(RUBIKS_TWIST, u16_pwm);
        printf("\n*** Setting rubiks twist to %u ***\n", u16_pwm);
    } else if (u8_servo == 'y') {
        turn_servo_by_pulse(SIMON_YELLOW, u16_pwm);
        printf("\n*** Setting simon yellow to %u ***\n", u16_pwm);
    } else if (u8_servo == 'b') {
        turn_servo_by_pulse(SIMON_BLUE, u16_pwm);
        printf("\n*** Setting simon blue to %u ***\n", u16_pwm);
    } else if (u8_servo == 'r') {
        turn_servo_by_pulse(SIMON_RED, u16_pwm);
        printf("\n*** Setting simon red to %u ***\n", u16_pwm);
    } else if (u8_servo == 'g') {
        turn_servo_by_pulse(SIMON_GREEN, u16_pwm);
        printf("\n*** Setting simon green to %u ***\n", u16_pwm);
    } else if (u8_servo == 's') {
        turn_servo_by_pulse(ARM_SLIDE, u16_pwm);
        printf("\n*** Setting arm extend to %u ***\n", u16_pwm);
    } else if (u8_servo == 'a') {
        turn_servo_by_pulse(ARM_POSITION, u16_pwm);
        printf("\n*** Setting arm pivot to %u ***\n", u16_pwm);
    } else {
        printf("Invalid choice\n");
    }
}

void start_light_print() {
    uint16_t u16_startLightvalue;

    while (1) {
        u16_startLightvalue = read_photo_cell(START_CELL);
        printf("Start light: %d\n", u16_startLightvalue);

        doHeartbeat();
    }
}
