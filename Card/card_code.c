/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: card_code.c
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Functions to pick up a card using the arm
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        11/19/2014              SECON 2015
*********************************************************************/

#include "card_code.h"

///////////////////////////////////////////////
//
// Card Usage
//
///////////////////////////////////////////////
void play_card() {
    int8_t i;
    display_draw_number(CARD_NUMBER);
    #ifdef DEBUG_BUILD
    printf("\n*** Grabbing Card ***\n");
    #endif

    // Slide arm forward
    game_arm_slide_forward();

    // Put arm down
    game_arm_pivot(25);
    DELAY_MS(500);
    for (i = 25;i >= 0;i-=4) {
        game_arm_pivot((uint8_t)i);
        DELAY_US(100);
    }
    game_arm_pivot(0);

    // Pull arm up
    for (i = 0;i <= 25; i+=4) {
        game_arm_pivot((uint8_t)i);
        DELAY_US(100);
    }
    game_arm_pivot(100);
    DELAY_MS(500);

    // Slide arm back
    game_arm_slide_back();
}
