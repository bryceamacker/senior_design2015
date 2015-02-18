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
#define SIMON_BUTTON            _RG6
#define ETCH_BUTTON             _RG7
#define CARD_BUTTON             _RG8
#define RUBIKS_BUTTON           _RG9

#define STATIC_ORDER            1
#define SKIP_START_LIGHT        1
#define SKIP_START_BUTTON       1

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
void setup_game_buttons(void);
void get_game_order(uint8_t pu8_gameOrder[4]);
#ifdef DEBUG_BUILD
void print_order(uint8_t pu8_gameOrder[4]);
#endif

// Main loop for the navigation PIC using I2C commands
int main (void) {
    uint8_t pu8_gameOrder[4];

    // Configure the motor controller PIC
    configBasic(HELLO_MSG);
    pic_navigation_init();
    setup_start_button();
    setup_game_buttons();

    // Start with the first game
    u8_currentGame = 0;

    if (STATIC_ORDER == 0) {
        #ifdef DEBUG_BUILD
        printf("Waiting for game order\n");
        #endif
        get_game_order(pu8_gameOrder);

        #ifdef DEBUG_BUILD
        print_order(pu8_gameOrder);
        #endif
    } else {
        pu8_gameOrder[0] = RUBIKS;
        pu8_gameOrder[1] = ETCH;
        pu8_gameOrder[2] = SIMON;
    }

    if (SKIP_START_BUTTON == 0) {
        #ifdef DEBUG_BUILD
        printf("Waiting for start button\n");
        #endif
        wait_for_start_button_push();
    }

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
    while(u8_currentGame <= 2) {
        #ifdef DEBUG_BUILD
        printf("Following line to box\n");
        #endif

        // Find a box
        follow_line_to_box(0.15);

        // Tell the game player to play a game
        play_game(pu8_gameOrder[u8_currentGame]);

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
