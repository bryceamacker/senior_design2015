/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: pic_game_player_config.c
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Setup for the SECON 2015 game player PIC 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        9/20/204                SECON 2015
*********************************************************************/

#include "pic_game_player_config.h"

#define PIC_I2C_ADDR 0x20

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
    
    // I2C init
    configI2C1(400);
    I2C1ADD = PIC_I2C_ADDR >> 1;
    _SI2C1IF = 0;
    _SI2C1IP = 1;
    _SI2C1IE = 1;
}
