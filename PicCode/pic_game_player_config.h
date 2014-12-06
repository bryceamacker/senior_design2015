/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: pic_game_player_config.h
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
* Steven Calhoun        9/20/2014               SECON 2015
*********************************************************************/

#include "pic24_all.h"
#include "motors_API.h"
#include "etch_code.h"
#include "rubiks_code.h"
#include "simon_code.h"
#include "card_code.h"
#include "platform_control.h"
#include "game_arm_control.h"

#ifdef DEBUG_BUILD
#include <stdio.h>
#endif

/**
 * @brief Initialize the game player pic
 */
void pic_game_player_init(void);
