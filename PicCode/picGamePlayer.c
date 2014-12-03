/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: picGamePlayer.c
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
char sz_playSimonString[BUFFSIZE] =     "Simon";
char sz_playRubiksString[BUFFSIZE] =    "Rubik";
char sz_playCardsString[BUFFSIZE] =     "Cards";
char sz_playEtchString[BUFFSIZE] =      "Etch.";
char sz_idleString[BUFFSIZE] =          "Idle.";
char sz_waitString[BUFFSIZE] =          "Wait.";

int main(void) {
    // Start off in wait state, waiting for the LED to turn off
    strncpy(sz_currentStateString, sz_waitString, BUFFSIZE);
    e_picState = IDLE;

    // Initialize pic and print out serial menu
    configBasic(HELLO_MSG);
    pic_game_player_init();

    // Wait for the start signal
    // wait_for_start_signal();

    // Move to the idle string, letting the motor controller it's time to move
    strncpy(sz_currentStateString, sz_idleString, BUFFSIZE);
    e_picState = IDLE;

    // Print out the first serial menu
    #ifdef DEBUG_BUILD
    serial_menu();
    #endif

    // Game playing loop to check serial commands and I2C commands
    while(1) {
        if (e_picState != IDLE) {
            if (e_picState == PLAY_ETCH) {
                play_etch();
            } else if (e_picState == PLAY_RUBIK) {
                play_rubiks();
            } else if (e_picState == PLAY_SIMON) {
                play_simon();
            } else if (e_picState == PLAY_CARDS) {
                play_card();
            }
            e_picState = IDLE;
            strncpy(sz_currentStateString, sz_idleString, BUFFSIZE);
            #ifdef DEBUG_BUILD
            printf("Waiting for a new game command\n");
            #endif
        } 
        #ifdef DEBUG_BUILD
        else if(isCharReady()) {
            // Handle serial command
            u8_c = inChar();
            serial_command(u8_c);
            serial_menu();
        } 
        #endif
        doHeartbeat();
    }
}

void I2C_check_command(volatile char *psz_s1) {
    if (strcmp((char*) psz_s1, sz_playEtchString) == 0) {
        e_picState = PLAY_ETCH;
    } else if(strcmp((char*) psz_s1, sz_playRubiksString) == 0) {  
        e_picState = PLAY_RUBIK;
    } else if(strcmp((char*) psz_s1, sz_playSimonString) == 0) {
        e_picState = PLAY_SIMON;
    } else if(strcmp((char*) psz_s1, sz_playCardsString) == 0) {
        e_picState = PLAY_CARDS;
    } else {
        e_picState = IDLE;
    }
    strncpy(sz_currentStateString, psz_s1, BUFFSIZE);

    #ifdef DEBUG_BUILD
    serial_menu();
    #endif
}

void wait_for_start_signal(void) {
    uint16_t i16_ledMaxOnValue;
    uint16_t i16_ledMinOnValue;
    uint16_t i16_ledThreshold;
    uint16_t u16_tempLedValue;
    uint8_t i;

    i16_ledMaxOnValue = 0;
    i16_ledMinOnValue = 65535;
    i16_ledThreshold = 0;
    u16_tempLedValue = 0;

    #ifdef DEBUG_BUILD
    printf("Waiting for start signal\n");
    #endif

    // Sample a few values from the on LED
    for (i = 0; i < 100; ++i) {
        u16_tempLedValue = read_photo_cell(START_CELL);
        if (u16_tempLedValue < i16_ledMinOnValue) {
            i16_ledMinOnValue = u16_tempLedValue;
        }
        if (u16_tempLedValue > i16_ledMaxOnValue) {
            i16_ledMaxOnValue = u16_tempLedValue;
        }
        DELAY_MS(10);
    }

    // Calculate the threshold
    i16_ledThreshold = i16_ledMaxOnValue - i16_ledMinOnValue;

    // Wait until the start light turns off
    u16_tempLedValue = read_photo_cell(START_CELL);
    while(u16_tempLedValue >= (i16_ledMinOnValue - i16_ledThreshold)) {
        u16_tempLedValue = read_photo_cell(START_CELL);
        doHeartbeat();
    }

    #ifdef DEBUG_BUILD
    printf("Start signal detected\n");
    #endif
}

// I2C intterrupt handler
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

