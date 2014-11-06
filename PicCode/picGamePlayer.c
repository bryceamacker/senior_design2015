/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: pic.c
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Controls all the game playing mechanisms for the
* SECON 2015 robot.
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        9/20/2014               SECON 2015
*********************************************************************/

#include "picGamePlayer.h"

char u8_c;
char sz_playSimonString[BUFFSIZE] = "Simon";
char sz_playRubiksString[BUFFSIZE] = "Rubik";
char sz_playCardsString[BUFFSIZE] = "Cards";
char sz_playEtchString[BUFFSIZE] = "Etch.";
char sz_idleString[BUFFSIZE] = "Idle.";
char sz_waitString[BUFFSIZE] = "Wait.";

int main(void) {
    u16_ledOnValue = 65535;
    u16_tempLedValue = 0;

    // Start off in wait state, waiting for the LED to turn off
    strncpy(sz_currentStateString, sz_waitString, BUFFSIZE);
    st_picState = IDLE;

    // Initialize pic and print out serial menu
    configBasic(HELLO_MSG);
    pic_game_player_init();

    // Sample a few values from the on LED
    for (i = 0; i < 100; ++i) {
        u16_tempLedValue = read_photo_cell(START_CELL);
        if (u16_tempLedValue < u16_ledOnValue) {
            u16_ledOnValue = u16_tempLedValue;
        }
        DELAY_MS(10);
    }

    // Wait until the start light turns off
    u16_tempLedValue = read_photo_cell(START_CELL);
    while(u16_tempLedValue >= (u16_ledOnValue - LED_MARGIN)) {
        u16_tempLedValue = read_photo_cell(START_CELL);
        doHeartbeat();
    }

    // Move to the idle string, letting the motor controller it's time to move
    strncpy(sz_currentStateString, sz_idleString, BUFFSIZE);
    st_picState = IDLE;

    // Print out the first serial menu
    serial_menu();

    // Game playing loop to check serial commands and I2C commands
    while(1) {
        if(isCharReady()) {
            // Handle serial command
            u8_c = inChar();
            serial_command(u8_c);
            serial_menu();
        } else if (st_picState == PLAY_ETCH) {
            play_etch();
            st_picState = IDLE;
            strncpy(sz_currentStateString, sz_idleString, BUFFSIZE);
        } else if (st_picState == PLAY_RUBIK) {
            play_rubiks();
            st_picState = IDLE;
            strncpy(sz_currentStateString, sz_idleString, BUFFSIZE);
        } else if (st_picState == PLAY_SIMON) {
            play_simon();
            st_picState = IDLE;
            strncpy(sz_currentStateString, sz_idleString, BUFFSIZE);
        } else if (st_picState == PLAY_CARDS) {
            st_picState = IDLE;
            strncpy(sz_currentStateString, sz_idleString, BUFFSIZE);
        }
        doHeartbeat();
    }
}

void I2C_check_command(volatile char *psz_s1) {
    if (strcmp((char*) psz_s1, "Etch.") == 0) {
        st_picState = PLAY_ETCH;
        strncpy(sz_currentStateString, sz_playEtchString, BUFFSIZE);
    } else if(strcmp((char*) psz_s1, "Rubik") == 0) {  
        st_picState = PLAY_RUBIK;
        strncpy(sz_currentStateString, sz_playRubiksString, BUFFSIZE);
    } else if(strcmp((char*) psz_s1, "Simon") == 0) {
        st_picState = PLAY_SIMON;
        strncpy(sz_currentStateString, sz_playSimonString, BUFFSIZE);
    } else if(strcmp((char*) psz_s1, "Cards") == 0) {
        st_picState = PLAY_CARDS;
        strncpy(sz_currentStateString, sz_playCardsString, BUFFSIZE);
    } else {
        st_picState = IDLE;
        strncpy(sz_currentStateString, sz_idleString, BUFFSIZE);
    }
    serial_menu();
}

void _ISRFAST _SI2C1Interrupt(void) {
    uint8_t u8_c;
    _SI2C1IF = 0;
    switch (e_mystate) {
    case STATE_WAIT_FOR_ADDR:
        u8_c = I2C1RCV; //clear RBF bit for address
        u16_index = 0;
        //check the R/W bit and see if read or write transaction
        if (I2C1STATbits.R_W) {
            I2C1TRN = sz_currentStateString[u16_index++];  //get first data byte
            I2C1CONbits.SCLREL = 1;     //release clock line so MASTER can drive it
            e_mystate = STATE_SEND_READ_DATA; //read transaction
        } else e_mystate = STATE_WAIT_FOR_WRITE_DATA;
        break;
    case STATE_WAIT_FOR_WRITE_DATA:
        //character arrived, place in buffer
        sz_i2cInString[u16_index++] = I2C1RCV;  //read the byte
        if (sz_i2cInString[u16_index-1] == 0) {
            //have a complete string, check the command
            I2C_check_command(sz_i2cInString);
            e_mystate = STATE_WAIT_FOR_ADDR; //wait for next transaction
        }
        break;
    case STATE_SEND_READ_DATA:
        I2C1TRN = sz_currentStateString[u16_index++];
        I2C1CONbits.SCLREL = 1;     //release clock line so MASTER can drive it
        if (sz_currentStateString[u16_index-1] == 0) e_mystate = STATE_SEND_READ_LAST;
        //this is the last character, after byte is shifted out, release the clock line again
        break;
    case STATE_SEND_READ_LAST:  //this is interrupt for last character finished shifting out
        e_mystate = STATE_WAIT_FOR_ADDR;
        break;
    default:
        e_mystate = STATE_WAIT_FOR_ADDR;
    }
}

