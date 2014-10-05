#include "servosAPI.h"
#include "port_mapping.h"

#ifndef ETCH_CODE_H_
#define ETCH_CODE_H_

#define FULL_ROTATION_HORIZ_DIST    18.5039     // in mm
#define FULL_ROTATION_VERT_DIST     19.3421     // in mm
#define ETCH_UNIT                   4.4         // 4.4 mm unit for simplicity of code

/////////////////////////////////////////////// 
//
// Etch primitives
//
///////////////////////////////////////////////
/**
 * @brief Turn the vertical nob of the etch-a-sketch
 * @details This function is custom tuned to the etch-a-sketch nobs
 * 
 * @param distance Distance in millimeters to move the cursor in the vertical direction
 */
void turn_servo_vertical_etch_distance(float distance);

/**
 * @brief Turn the horizontal nob of the etch-a-sketch
 * @details This function is custom tuned to the etch-a-sketch nobs
 * 
 * @param distance Distance in millimeters to move the cursor in the horizontal direction
 */
void turn_servo_horizontal_etch_distance(float distance);

/////////////////////////////////////////////// 
//
// Etch Characters/words
//
///////////////////////////////////////////////
/**
 * @brief I-Character on etch
 */
void draw_I_character(void);

/**
 * @brief E-Character on etch starting from bottom
 */
void draw_E_character_from_bottom(void);

/**
 * @brief E-Character on etch from top
 */
void drawE_character_from_top();

/**
 * @brief Full IEEE word
 */
void draw_IEEE(void);

/**
 * @brief Underlines IEEE and goes back to beginning
 */
void underline_to_reset(void);

#endif