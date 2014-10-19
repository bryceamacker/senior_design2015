/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: simon_code.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION:Declarations of funcitons and constants for playing
* the simons
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        10/5/2014               SECON 2015
*********************************************************************/

#include "servosAPI.h"
#include "photoCellAPI.h"
#include "port_mapping.h"

#ifndef SIMON_CODE_H_
#define SIMON_CODE_H_

// Retract pulse width constants
#define YELLOW_RETRACT  2300
#define BLUE_RETRACT    2350
#define RED_RETRACT     2350
#define GREEN_RETRACT   600

// Hover pulse width constants
#define YELLOW_HOVER    1115
#define BLUE_HOVER      1025
#define RED_HOVER       990
#define GREEN_HOVER     600

// Push pulse width constants
#define YELLOW_PUSH     1075
#define BLUE_PUSH       990
#define RED_PUSH        950
#define GREEN_PUSH      1875

#define PUSH_WAIT       250

typedef enum {
    YELLOW_BUTTON = 0,
    BLUE_BUTTON =   1,
    RED_BUTTON =    2,
    GREEN_BUTTON =  3,
    START_BUTTON =  4
} buttonID;

/////////////////////////////////////////////// 
//
// Simon primitives
//
///////////////////////////////////////////////

void calibrate_sensors();
uint8_t find_color(uint8_t u8_numberOfButtons);
void play_buttons(uint8_t u8_num);
uint8_t confirmColor(uint8_t u8_color);
uint8_t confirmColorOff(uint8_t u8_color, uint16_t u16_onValue);

/**
 * Retract the arm for the given button
 * 
 * @param button arm to be retracted
 */
void simon_retract_button(buttonID button);

/**
 * Hover the arm for the given button
 * 
 * @param button arm to be hovered
 */
void simon_hover_button(buttonID button);

/**
 * Push the given Simon button
 * 
 * @param button to be pushed
 */
void simon_push_button(buttonID button);

/**
 * Push the given Simon button then return to hover position
 * 
 * @param button to be pushed then hovered
 */
void simon_push_and_hover_button(buttonID button);

/**
 * @brief Initialize simon arms to the retracted postion
 */
void simon_init(void);

/**
 * @brief Push all simon buttons
 */
void simon_push_buttons(void);

/**
 * @brief Position all servo arms to the hover position
 */
void simon_hover_buttons(void);

/**
 * @brief Position all servo arms in the retract position
 */
void simon_retract_buttons(void);

/**
 * @brief Push and hover all simon buttons
 */
void simon_push_and_hover_buttons();

/////////////////////////////////////////////// 
//
// Simon Usage
//
///////////////////////////////////////////////


#endif
