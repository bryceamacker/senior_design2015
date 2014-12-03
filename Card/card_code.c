\/*********************************************************************
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
    // Slide arm forward
    game_arm_slide_forward();
    DELAY(1000);

    // Put arm down
    game_arm_lower();
    DELAY(1000);

    // Pull arm up
    game_arm_raise();
    DELAY(1000);

    // Slide arm back
    game_arm_slide_back();
    DELAY(1000);
}
