#include "servosAPI.h"
#include "port_mapping.h"

#ifndef SIMON_CODE_H_
#define SIMON_CODE_H_

#define YELLOW_PUSH     1050
#define BLUE_PUSH       1025
#define RED_PUSH        895
#define GREEN_PUSH      1825

#define YELLOW_HOVER    1125
#define BLUE_HOVER      1085
#define RED_HOVER       925
#define GREEN_HOVER     1750

#define YELLOW_RETRACT  2300
#define BLUE_RETRACT    2350
#define RED_RETRACT     2350
#define GREEN_RETRACT   600

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

/**
 * Functions for ALL of the different simon push positions
 */
void simon_push_yellow_button(void);
void simon_push_blue_button(void);
void simon_push_red_button(void);
void simon_push_green_button(void);

/**
 * Functions for ALL of the different simon hover positions
 */
void simon_hover_yellow_button(void);
void simon_hover_blue_button(void);
void simon_hover_red_button(void);
void simon_hover_green_button(void);

/**
 * Functions for ALL of the different simon retract positions
 */
void simon_retract_yellow(void);
void simon_retract_blue(void);
void simon_retract_red(void);
void simon_retract_green(void);

/////////////////////////////////////////////// 
//
// Simon Usage
//
///////////////////////////////////////////////

/**
 * @brief Initialize simon arms to the retracted postion
 */
void simon_init(void);

/**
 * @brief Push a simon button
 * 
 * @param button buttonID of the button to be pushed
 */
void simon_push_button(buttonID button);

/**
 * @brief Position all servo arms to the hover position
 */
void simon_hover_buttons(void);

/**
 * @brief Position all servo arms in the retract position
 */
void simon_retract_buttons(void);

#endif
