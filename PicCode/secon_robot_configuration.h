/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: secon_robot_configuration.c
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: All the configuration options for the robot are
* in this file
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        3/5/2014                SECON 2015
*********************************************************************/

// I2C Constants
#define PIC_GAME_PLAYER_ADDR            0x20
#define BUFFSIZE                        64

// Configurations
#define STATIC_ORDER                    1
#define SKIP_START_LIGHT                0
#define SKIP_START_BUTTON               1
#define SKIP_STATIC_COURSE_SELECTION    1

#define SKIP_SIMON                      0
#define SKIP_ETCH                       0
#define SKIP_RUBIKS                     0
#define SKIP_CARD                       0

// I2C Messages
char sz_playSimonString[BUFFSIZE] =     "Simon";
char sz_playRubiksString[BUFFSIZE] =    "Rubik";
char sz_playCardsString[BUFFSIZE] =     "Cards";
char sz_playEtchString[BUFFSIZE] =      "Etch.";
char sz_idleString[BUFFSIZE] =          "Idle.";
char sz_waitString[BUFFSIZE] =          "Wait.";
char sz_dispString[BUFFSIZE] =          "Dis";
