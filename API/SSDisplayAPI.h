/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: SSDisplayAPI.c
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Helps with the control of a sevent segmen display
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        1/26/2014               SECON 2015
*********************************************************************/

#include "pic24_all.h"
#include "game_player_port_mapping.h"

#ifdef DEBUG_BUILD
#include <stdio.h>
#endif

#ifndef SSD_DISPLAY_API_H_
#define SSD_DISPLAY_API_H_

#define START_BUTTON_NUMBER     1
#define START_LIGHT_NUMBER      2

#define SIMON_NUMBER            10

#define RUBIKS_NUMBER           20

#define ETCH_NUMBER             30
#define ETCH_I_NUMBER           31
#define ETCH_E1_NUMBER          32
#define ETCH_E2_NUMBER          33
#define ETCH_E3_NUMBER          34

#define CARD_NUMBER             40
#define NAVIGATING_NUMBER       99

#define ON_VALUE  1
#define OFF_VALUE 0

typedef enum {
  A = 0,
  B = 1,
  C = 2,
  D = 3,
  E = 4,
  F = 5,
  G = 6
} SSDsegments;

///////////////////////////////////////////////
//
// SSD config
//
///////////////////////////////////////////////
/**
 * @brief Initializes every thing for both SSDs
*/
void ssd_init(void);

/**
 * @brief Silly little light seqence showing of SSDs
*/
void init_sequence(void);

///////////////////////////////////////////////
//
// SSD primitives
//
///////////////////////////////////////////////
/**
 * @brief Stupid little helper to draw characters
 *
 * @param character Character to be checked
 * @return whether or not the character is alpha or not
**/
uint8_t check_if_character_alpha(char character);

/**
 * @brief Turn on the given segment for display1
 * @param segment Segment to be turned on
**/
void turn_on_display1_segment(SSDsegments segment);

/**
 * @brief Turn on the given segment for display2
 * @param segment Segment to be turned on
**/
void turn_on_display2_segment(SSDsegments segment);

/**
 * @brief Turn off the given segment for display1
 * @param segment Segment to be turned off
**/
void turn_off_display1_segment(SSDsegments segment);

/**
 * @brief Turn off the given segment for display2
 * @param segment Segment to be turned off
**/
void turn_off_display2_segment(SSDsegments segment);

/**
 * @brief Draw a zero on the display
 * @param u8_display Display to be drawn on
**/
void draw_zero(uint8_t u8_display);

/**
* @brief Draw a one on the display
* @param u8_display Display to be drawn on
**/
void draw_one(uint8_t u8_display);

/**
* @brief Draw a two on the display
* @param u8_display Display to be drawn on
**/
void draw_two(uint8_t u8_display);

/**
* @brief Draw a three on the display
* @param u8_display Display to be drawn on
**/
void draw_three(uint8_t u8_display);

/**
* @brief Draw a four on the display
* @param u8_display Display to be drawn on
**/
void draw_four(uint8_t u8_display);

/**
* @brief Draw a five on the display
* @param u8_display Display to be drawn on
**/
void draw_five(uint8_t u8_display);

/**
* @brief Draw a six on the display
* @param u8_display Display to be drawn on
**/
void draw_six(uint8_t u8_display);

/**
* @brief Draw a seven on the display
* @param u8_display Display to be drawn on
**/
void draw_seven(uint8_t u8_display);

/**
* @brief Draw a eight on the display
* @param u8_display Display to be drawn on
**/
void draw_eight(uint8_t u8_display);

/**
* @brief Draw a nine on the display
* @param u8_display Display to be drawn on
**/
void draw_nine(uint8_t u8_display);

/**
* @brief Draw various letters on the display
* @param u8_display Display to be drawn on
**/
void draw_C(uint8_t u8_display);
void draw_E(uint8_t u8_display);
void draw_F(uint8_t u8_display);
void draw_H(uint8_t u8_display);
void draw_J(uint8_t u8_display);
void draw_L(uint8_t u8_display);
void draw_P(uint8_t u8_display);
void draw_R(uint8_t u8_display);
void draw_U(uint8_t u8_display);

/**
* @brief Set a particular segment on display1
* @param segment Segment to be set
* @param u8_value Value to set the segment to
**/
void set_display1_segment(SSDsegments segment, uint8_t u8_value);

/**
* @brief Set a particular segment on display2
* @param segment Segment to be set
* @param u8_value Value to set the segment to
**/
void set_display2_segment(SSDsegments segment, uint8_t u8_value);

/**
 * @brief Turn everything on
**/
void display_all_on(void);

/**
 * @brief Turn everything off
**/
void display_all_off(void);

///////////////////////////////////////////////
//
// SSD usage
//
///////////////////////////////////////////////
/**
 * @brief Draw the give number on display1
 * @param u8_number Number to be drawn
**/
void display1_draw_number(uint8_t u8_number);

/**
 * @brief Draw the give number on display2
 * @param u8_number Number to be drawn
**/
void display2_draw_number(uint8_t u8_number);

/**
* @brief Draw a letter on the display
* @param character Character to draw
* @param u8_display Display to be drawn on
**/
void display_draw_char(char character, uint8_t u8_display);

/**
 * @brief Draw two digit number across the two displays
 * @param u8_number Number to be drawn
**/
void display_draw_number(uint8_t u8_number);

/**
 * @brief Draw a buffer of characters on the display
 *
 * @param buffer[2] Two characters to draw, 0 on 2 and 1 on 1
*/
void display_draw_buffer(char buffer[2]);

#ifdef DEBUG_BUILD
/**
 * @brief Helps test the ss displays
**/
void test_ss_displays(void);
#endif
#endif
