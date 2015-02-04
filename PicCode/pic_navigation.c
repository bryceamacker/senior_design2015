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
#include <string.h>

#ifdef DEBUG_BUILD
#include <stdio.h>
#warning "Navigation: DEBUG BUILD"
#endif

#define PIC_GAME_PLAYER_ADDR    0x20
#define BUFFSIZE                64
#define START_BUTTON            _RF4

// Game enumeration
typedef enum {
    SIMON =     0,
    RUBIKS =    1,
    ETCH =      2,
    CARD =      3
} gameID;

// I2C Messages
char sz_playSimonString[BUFFSIZE] =     "Simon";
char sz_playRubiksString[BUFFSIZE] =    "Rubik";
char sz_playCardsString[BUFFSIZE] =     "Cards";
char sz_playEtchString[BUFFSIZE] =      "Etch.";
char sz_idleString[BUFFSIZE] =          "Idle.";
char sz_waitString[BUFFSIZE] =          "Wait.";
char sz_recieveString[BUFFSIZE];

// Game counter
uint8_t u8_currentGame;

// Function declarations
void pic_navigation_init(void);
void play_game(gameID game);
void setup_start_button(void);
void wait_for_start_button_push(void);

// Main loop for the navigation PIC using I2C commands
int main (void) {
    // Configure the motor controller PIC
    configBasic(HELLO_MSG);
    pic_navigation_init();
    setup_start_button();

    // Start with the first game
    u8_currentGame = 0;

    #ifdef DEBUG_BUILD
    printf("Waiting for start button\n");
    #endif
    wait_for_start_button_push();

    // Wait for the game player PIC to detect the start light to turn off
    #ifdef DEBUG_BUILD
    printf("Waiting for start signal\n");
    #endif
    while (strcmp((char*) sz_recieveString, "Idle.") != 0) {
        DELAY_MS(1000);
        readNI2C1(PIC_GAME_PLAYER_ADDR, (uint8_t *) sz_recieveString, 6);
        doHeartbeat();
    }

    // Get out of the starting box
    motors_move_forward(0.15);
    DELAY_MS(3000);
    motors_stop();

    // Play Rubiks, Etch, and Simon then stop
    while(u8_currentGame <= 3) {
        #ifdef DEBUG_BUILD
        printf("Following line to box\n");
        #endif

        // Find a box
        follow_line_to_box(0.15);

        // Tell the game player to play a game
        play_game(u8_currentGame);

        // Back up for a few seconds, increment the current game, and start over
        motors_move_reverse(0.15);
        DELAY_MS(2500);
        motors_stop();
        u8_currentGame++;
    }

    // After all games have been played just sit
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

void setup_start_button() {
    CONFIG_RF4_AS_DIG_INPUT();
    ENABLE_RF4_PULLUP();
    DELAY_US(1);
}

void wait_for_start_button_push() {
    while (START_BUTTON) {
        doHeartbeat();
    }
}
