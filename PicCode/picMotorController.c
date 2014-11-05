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

int main (void) {
    configBasic(HELLO_MSG);
    pic_motor_controller_init();

    u16_position = 0;
    i16_error = 0;
    u8_detectingSensors = 0;
    u8_foundObjective = 0;

    // Flags while in sharp turns
    u8_leftTurn = 0;
    u8_rightTurn = 0;

    // Find the center of the line we are constantly trying to stay at
    i16_lineCenter = ((1000 * (SENSOR_NUM - 1)) / 2);

    // Start with the first game
    strncpy(sz_sendString, sz_playRubiksString, BUFFSIZE);

    while(1) {
        // Get the average position of the line
        u16_position = 1000 * get_line(pau16_sensorValues);
        i16_error = u16_position - i16_lineCenter;
        u8_detectingSensors = 0;

        strncpy(sz_recieveString, sz_sendString, BUFFSIZE);

        // Sum up the array
        for (i = 0; i < SENSOR_NUM; i++) {
            u8_detectingSensors += pau16_sensorValues[i];
        }

        // If enough sensors are detecting, what appears to be a wide line is most likely the edge of a box
        if (u8_detectingSensors >= SENSOR_NUM - 2) {
            u8_foundObjective = 1;
        }

        // Stop when we reach a box
        if (u8_foundObjective == 1) {
            printf("Found box\n");
            // Stop at the box and tell the game player to play a game
            motors_stop();
            writeNI2C1(PIC_GAME_PLAYER_ADDR, (uint8_t *)sz_sendString, getStringLength(sz_sendString));

            // Wait until the game is done being played
            while (strcmp((char*) sz_recieveString, "Idle.") != 0) {
                DELAY_MS(1000);
                readNI2C1(PIC_GAME_PLAYER_ADDR, (uint8_t *) sz_recieveString, 6);
            }

            // Print out success message and go to the next game
            if (strcmp((char*) sz_sendString, "Rubik") == 0) {
                // Go to the next game
                printf("Rubiks played\n");
                strncpy(sz_sendString, sz_playEtchString, BUFFSIZE);
            } else if (strcmp((char*) sz_sendString, "Etch.") == 0) {
                // Go to the next game
                printf("Etch played\n");
                strncpy(sz_sendString, sz_playSimonString, BUFFSIZE);
            } else if (strcmp((char*) sz_sendString, "Simon") == 0) {
                // Go to the next game
                printf("Simon played\n");
                strncpy(sz_sendString, sz_playRubiksString, BUFFSIZE);
            }

            u8_foundObjective = 0;
            motors_move_reverse(.15);
            DELAY_MS(2500);
            motors_stop();

        } else { 
            if (i16_error > 1000) {
                motors_turn_left(.15);
            }
            if (i16_error < -1000) {
                motors_turn_right(.15);
            }
            if ((i16_error >= -1000) && (i16_error <= 1000)) {
                motors_move_forward(.15);
            }
        }
    }
}

int16_t getStringLength(char* psz_1) {
    uint16_t u16_len;
    u16_len = 0;
    while (*psz_1) {
        psz_1++;
        u16_len++;
    }
    u16_len++;
    return u16_len;
}
