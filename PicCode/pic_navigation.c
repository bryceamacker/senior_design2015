/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: pic_navigation.c
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: PIC that will use a sensor array and motors to
* follow a line to a game then communicate to another PIC via I2C
* so that it can play the game
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        10/01/2014              SECON 2015
*********************************************************************/

#include "pic24_all.h"
#include "line_follower_API.h"
#include "navigation_port_mapping.h"
#include "secon_robot_configuration.h"
#include <string.h>
#include <stdlib.h>

#ifdef DEBUG_BUILD
#include <stdio.h>
#warning "Navigation: DEBUG BUILD"
#endif

#define START_BUTTON_PUSHED     (_RF4 == 0)
#define START_BUTTON_RELEASED   (_RF4 == 1)

#define SIMON_BUTTON_PUSHED     (_RG6 == 0)
#define SIMON_BUTTON_RELEASED   (_RG6 == 1)

#define ETCH_BUTTON_PUSHED      (_RG7 == 0)
#define ETCH_BUTTON_RELEASED    (_RG7 == 1)

#define CARD_BUTTON_PUSHED      (_RG8 == 0)
#define CARD_BUTTON_RELEASED    (_RG8 == 1)

#define RUBIKS_BUTTON_PUSHED    (_RG9 == 0)
#define RUBIKS_BUTTON_RELEASED  (_RG9 == 1)

#define UP_BUTTON_PUSHED        (_RG9 == 0)
#define UP_BUTTON_RELEASED      (_RG9 == 1)

#define DOWN_BUTTON_PUSHED      (_RG7 == 0)
#define DOWN_BUTTON_RELEASED    (_RG7 == 1)

#define SET_BUTTON_PUSHED       (_RG6 == 0)
#define SET_BUTTON_RELEASED     (_RG6 == 1)

// I2C buffer
char sz_recieveString[BUFFSIZE];

// Navigation externs
extern uint8_t u8_routineBlock;
extern queue_t navigationRoutineQueue;
extern uint8_t u8_currentRoutine;

// Game stuff
uint8_t u8_gameBlock;
uint8_t pu8_gameOrder[4];

// Info for static courses
uint8_t u8_staticCourseNumber;

// Function declarations
void pic_navigation_init(void);
void run_static_course(uint8_t pu8_gameOrder[4]);
void navigate_course(uint8_t pu8_gameOrder[4]);
void play_game(gameID game);
void configure_robot(void);
void setup_start_button(void);
void wait_for_start_button_push(void);
void setup_game_buttons(void);
void get_game_order(uint8_t pu8_gameOrder[4]);
#ifdef DEBUG_BUILD
void print_order(uint8_t pu8_gameOrder[4]);
#endif

// Main loop for the navigation PIC using I2C commands
int main (void) {
    u8_gameBlock = 0;
    u8_staticCourseNumber = 0;

    // Configure the motor controller PIC
    configBasic(HELLO_MSG);
    pic_navigation_init();
    setup_start_button();
    setup_game_buttons();

    configure_robot();

    if (SKIP_START_BUTTON == 0) {
        #ifdef DEBUG_BUILD
        printf("Waiting for start button\n");
        #endif
        wait_for_start_button_push();
    }

    calibrateAllSensorArrays();

    // Wait for the game player PIC to detect the start light to turn off
    #ifdef DEBUG_BUILD
    printf("Waiting for start signal\n");
    #endif
    while (strcmp((char*) sz_recieveString, "Idle.") != 0) {
        DELAY_MS(1000);
        readNI2C1(PIC_GAME_PLAYER_ADDR, (uint8_t *) sz_recieveString, 6);
        doHeartbeat();
    }

    if (prepare_course_routines(u8_staticCourseNumber) == 1) {
        // Navigate the whole course
        run_static_course(pu8_gameOrder);
    }
    else {
        calibrateAllSensorArrays();
        navigate_course(pu8_gameOrder);
    }

    // After the finish line has been reached just sit and relax
    while(1) doHeartbeat();
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

// Navigate the whole course
void navigate_course(uint8_t pu8_gameOrder[4]) {
    // Game counter
    uint8_t u8_currentGame;

    u8_currentGame = 0;

    enqueue(&navigationRoutineQueue, MOVE_PAST_START_BOX);
    check_for_routine();

    // Play Rubiks, Etch, and Simon then stop
    while(u8_currentGame <= 3) {
        // Find a box
        follow_line_to_box(BASE_SPEED);

        // Make our final preperations
        final_game_preparations(pu8_gameOrder[u8_currentGame]);

        // Tell the game player to play a game
        play_game(pu8_gameOrder[u8_currentGame]);
        #ifdef DEBUG_BUILD
        printf("Reached game %u\n", u8_currentGame);
        #endif

        // Leave the game box in preperation to follow the line back
        prepare_to_leave_game(pu8_gameOrder[u8_currentGame]);

        // Get back to the main line
        follow_line_back_to_main_line(BASE_SPEED);

        // Increment to the next game
        u8_currentGame++;
    }

    // Get to the finish line
    follow_line_to_box(BASE_SPEED);
}

void run_static_course(uint8_t pu8_gameOrder[4]) {
    // Game counter
    uint8_t u8_currentGame;
    u8_currentGame = 0;

    // Start the routines
    check_for_routine();

    #ifdef DEBUG_BUILD
    printf("Running a static course\n");
    #endif

    while(u8_currentGame <= 3) {
        if (u8_currentRoutine == PLAY_GAME_PAUSE) {
            play_game(pu8_gameOrder[u8_currentGame]);
            u8_gameBlock = 0;
            check_for_routine();
        } else {
            doHeartbeat();
        }
    }
}

// Function to send I2C commands to play games
void play_game(gameID game) {
    char sz_sendString[BUFFSIZE];
    char sz_recieveString[BUFFSIZE];

    // Copy the correct string to send
    if (game == RUBIKS) {
        #ifdef DEBUG_BUILD
        printf("Playing Rubiks\n");
        #endif
        strncpy(sz_sendString, sz_playRubiksString, BUFFSIZE);
    } else if (game == ETCH) {
        #ifdef DEBUG_BUILD
        printf("Playing Etch\n");
        #endif
        strncpy(sz_sendString, sz_playEtchString, BUFFSIZE);
    } else if (game == SIMON) {
        #ifdef DEBUG_BUILD
        printf("Playing Simon\n");
        #endif
        strncpy(sz_sendString, sz_playSimonString, BUFFSIZE);
    } else if (game == CARD) {
        #ifdef DEBUG_BUILD
        printf("Playing Cards\n");
        #endif
        strncpy(sz_sendString, sz_playCardsString, BUFFSIZE);
    }

    // Send the game player the game to play
    writeNI2C1(PIC_GAME_PLAYER_ADDR, (uint8_t *)sz_sendString, 6);

    // Wait until the game is done being played
    do {
        DELAY_MS(1000);
        readNI2C1(PIC_GAME_PLAYER_ADDR, (uint8_t *) sz_recieveString, 6);
        doHeartbeat();
    } while (strcmp((char*) sz_recieveString, "Idle.") != 0);

    // Print out a success message
    #ifdef DEBUG_BUILD
    if (game == RUBIKS) {
        printf("Rubiks played\n");
    } else if (game == ETCH) {
        printf("Etch played\n");
    } else if (game == SIMON) {
        printf("Simon played\n");
    } else if (game == CARD) {
        printf("Cards played\n");
    }
    #endif
}

// Set up the start button
void setup_start_button() {
    CONFIG_RF4_AS_DIG_INPUT();
    ENABLE_RF4_PULLUP();
    DELAY_US(1);
}

// Wait until the start button is pushed
void wait_for_start_button_push() {
    while (START_BUTTON_RELEASED) {
        doHeartbeat();
    }
}

// Configure the game selection buttons
void setup_game_buttons() {
    CONFIG_RG6_AS_DIG_INPUT();
    ENABLE_RG6_PULLUP();
    DELAY_US(1);

    CONFIG_RG7_AS_DIG_INPUT();
    ENABLE_RG7_PULLUP();
    DELAY_US(1);

    CONFIG_RG8_AS_DIG_INPUT();
    ENABLE_RG8_PULLUP();
    DELAY_US(1);

    CONFIG_RG9_AS_DIG_INPUT();
    ENABLE_RG9_PULLUP();
    DELAY_US(1);
}

void configure_robot(void) {
    char tempBuffer[4];
    char numBuffer[2];
    char sz_sendString[BUFFSIZE];

    if (STATIC_ORDER == 0) {
        #ifdef DEBUG_BUILD
        printf("Waiting for game order\n");
        #endif
        get_game_order(pu8_gameOrder);

        #ifdef DEBUG_BUILD
        print_order(pu8_gameOrder);
        #endif
    } else {
        pu8_gameOrder[0] = SIMON;
        pu8_gameOrder[1] = RUBIKS;
        pu8_gameOrder[2] = ETCH;
        pu8_gameOrder[3] = CARD;
    }

    if (SKIP_STATIC_COURSE_SELECTION == 0) {
        while (SET_BUTTON_RELEASED) {
            if (DOWN_BUTTON_PUSHED) {
                if (u8_staticCourseNumber == 0) {
                    u8_staticCourseNumber = 99;
                } else {
                    u8_staticCourseNumber--;
                }
                strncpy(tempBuffer, sz_dispString, 4);
                itoa (numBuffer, u8_staticCourseNumber, 10);

                if (u8_staticCourseNumber < 10) {
                    numBuffer[0] = '0';
                }

                strcat(tempBuffer, numBuffer);
                strncpy(sz_sendString, tempBuffer, BUFFSIZE);
                writeNI2C1(PIC_GAME_PLAYER_ADDR, (uint8_t *)sz_sendString, 6);

                #ifdef DEBUG_BUILD
                printf("Static course %u\n", u8_staticCourseNumber);
                printf("Created string %s\n", sz_sendString);
                #endif
            }
            if (UP_BUTTON_PUSHED) {
                if (u8_staticCourseNumber == 99) {
                    u8_staticCourseNumber = 0;
                } else {
                    u8_staticCourseNumber++;
                }
                strncpy(tempBuffer, sz_dispString, 4);
                itoa (numBuffer, u8_staticCourseNumber, 10);

                if (u8_staticCourseNumber < 10) {
                    numBuffer[0] = '0';
                }

                strcat(tempBuffer, numBuffer);
                strncpy(sz_sendString, tempBuffer, BUFFSIZE);
                writeNI2C1(PIC_GAME_PLAYER_ADDR, (uint8_t *)sz_sendString, 6);

                #ifdef DEBUG_BUILD
                printf("Static course %u\n", u8_staticCourseNumber);
                printf("Created string %s\n", sz_sendString);
                #endif
            }

            // Simple debounce
            DELAY_MS(10);
        }
    }
}

// Wait until buttons for every game have been pressed
void get_game_order(uint8_t pu8_gameOrder[4]) {
    uint8_t u8_simonSet;
    uint8_t u8_cardSet;
    uint8_t u8_etchSet;
    uint8_t u8_rubiksSet;
    uint8_t u8_position;

    u8_simonSet = 0;
    u8_cardSet = 0;
    u8_etchSet = 0;
    u8_rubiksSet = 0;

    u8_position = 0;

    while (!(u8_simonSet && u8_cardSet && u8_etchSet && u8_rubiksSet)) {
        if (SIMON_BUTTON_PUSHED && (!u8_simonSet)) {
            #ifdef DEBUG_BUILD
            printf("Qeued Simon\n");
            #endif

            pu8_gameOrder[u8_position] = SIMON;
            u8_simonSet = 1;
            u8_position++;
        }
        else if (CARD_BUTTON_PUSHED && (!u8_cardSet)) {
            #ifdef DEBUG_BUILD
            printf("Qeued Cards\n");
            #endif

            pu8_gameOrder[u8_position] = CARD;
            u8_cardSet = 1;
            u8_position++;
        }
        else if (ETCH_BUTTON_PUSHED && (!u8_etchSet)) {
            #ifdef DEBUG_BUILD
            printf("Qeued Etch\n");
            #endif

            pu8_gameOrder[u8_position] = ETCH;
            u8_etchSet = 1;
            u8_position++;
        }
        else if (RUBIKS_BUTTON_PUSHED && (!u8_rubiksSet)) {
            #ifdef DEBUG_BUILD
            printf("Qeued Rubiks\n");
            #endif

            pu8_gameOrder[u8_position] = RUBIKS;
            u8_rubiksSet = 1;
            u8_position++;
        }
        doHeartbeat();
    }
}

#ifdef DEBUG_BUILD
// Helper to print out the inputted game order
void print_order(uint8_t pu8_order[4]) {
    uint8_t u8_i;
    uint8_t game;

    for (u8_i=0;u8_i<=3;u8_i++) {
        game = pu8_order[u8_i];

        if (game == RUBIKS) {
            printf("%u) Rubiks\n", u8_i+1);
        } else if (game == ETCH) {
            printf("%u) Etch\n", u8_i+1);
        } else if (game == SIMON) {
            printf("%u) Simon\n", u8_i+1);
        } else if (game == CARD) {
            printf("%u) Cards\n", u8_i+1);
        }
    }
}
#endif
