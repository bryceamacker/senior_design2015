/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName:
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

#include "picMotorController.h"

char sz_playRubiksString[BUFFSIZE] = "Rubik";
char sz_playEtchString[BUFFSIZE] = "Etch.";
char sz_playSimonString[BUFFSIZE] = "Simon";
char sz_playCardsString[BUFFSIZE] = "Cards";
char sz_idleString[BUFFSIZE] = "Idle.";
uint8_t u8_c;

int main (void) {
    // Configure the motor controller PIC
    configBasic(HELLO_MSG);
    pic_motor_controller_init();

    // Start with the first game
    u8_currentGame = 0;

    // Wait for the game player PIC to detect the start light to turn off
    printf("Waiting for start signal\n");
    while (strcmp((char*) sz_recieveString, "Idle.") != 0) {
        DELAY_MS(1000);
        readNI2C1(PIC_GAME_PLAYER_ADDR, (uint8_t *) sz_recieveString, 6);
        doHeartbeat();
    }

    // Play Rubiks, Etch, and Simon then stop
    while(u8_currentGame < 3) {
        printf("Following line to box\n");
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

void play_game(gameID game) {
    char sz_sendString[BUFFSIZE];
    char sz_recieveString[BUFFSIZE];
    
    // Copy the correct string to send
    if (game == RUBIKS) {
        printf("Playing Rubiks\n");
        strncpy(sz_sendString, sz_playRubiksString, BUFFSIZE);
    } else if (game == ETCH) {
        printf("Playing Etch\n");
        strncpy(sz_sendString, sz_playEtchString, BUFFSIZE);
    } else if (game == SIMON) {
        printf("Playing Simon\n");
        strncpy(sz_sendString, sz_playSimonString, BUFFSIZE);
    } else if (game == CARD) {
        printf("Playing Cards\n");
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
    if (game == RUBIKS) {
        printf("Rubiks played\n");
    } else if (game == ETCH) {
        printf("Etch played\n");
    } else if (game == SIMON) {
        printf("Simon played\n");
    } else if (game == CARD) {
        printf("Cards played\n");
    }
}