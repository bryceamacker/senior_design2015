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
* MODULE FUNCTION: Helps with the control of a seven segment display
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        1/26/2014               SECON 2015
*********************************************************************/

#include "SSDisplayAPI.h"


///////////////////////////////////////////////
//
// SSD config
//
///////////////////////////////////////////////
/**
* @brief Initializes every thing for both SSDs
*/
void ssd_init(void) {
  // Display 1
  CONFIG_RG15_AS_DIG_OUTPUT();
  CONFIG_RC1_AS_DIG_OUTPUT();
  CONFIG_RC2_AS_DIG_OUTPUT();
  CONFIG_RG6_AS_DIG_OUTPUT();
  CONFIG_RG7_AS_DIG_OUTPUT();
  CONFIG_RG8_AS_DIG_OUTPUT();
  CONFIG_RG9_AS_DIG_OUTPUT();

  // Display 2
  CONFIG_RB6_AS_DIG_OUTPUT();
  CONFIG_RB7_AS_DIG_OUTPUT();
  CONFIG_RB8_AS_DIG_OUTPUT();
  CONFIG_RB9_AS_DIG_OUTPUT();
  CONFIG_RB10_AS_DIG_OUTPUT();
  CONFIG_RB11_AS_DIG_OUTPUT();
  CONFIG_RB12_AS_DIG_OUTPUT();
}

///////////////////////////////////////////////
//
// SSD primitives
//
///////////////////////////////////////////////
/**
* @brief Turn on the given segment for display1
* @param segment Segment to be turned on
**/
void turn_on_display1_segment(SSDsegments segment) {
  switch (segment) {
    case A:
      D1A_LED = ON_VALUE;
      break;
    case B:
      D1B_LED = ON_VALUE;
      break;
    case C:
      D1C_LED = ON_VALUE;
      break;
    case D:
      D1D_LED = ON_VALUE;
      break;
    case E:
      D1E_LED = ON_VALUE;
      break;
    case F:
      D1F_LED = ON_VALUE;
      break;
    case G:
      D1G_LED = ON_VALUE;
      break;
  }
}

/**
* @brief Turn on the given segment for display2
* @param segment Segment to be turned on
**/
void turn_on_display2_segment(SSDsegments segment) {
  switch (segment) {
    case A:
      D2A_LED = ON_VALUE;
      break;
    case B:
      D2B_LED = ON_VALUE;
      break;
    case C:
      D2C_LED = ON_VALUE;
      break;
    case D:
      D2D_LED = ON_VALUE;
      break;
    case E:
      D2E_LED = ON_VALUE;
      break;
    case F:
      D2F_LED = ON_VALUE;
      break;
    case G:
      D2G_LED = ON_VALUE;
      break;
  }
}

/**
* @brief Turn off the given segment for display1
* @param segment Segment to be turned off
**/
void turn_off_display1_segment(SSDsegments segment) {
  switch (segment) {
    case A:
      D1A_LED = OFF_VALUE;
      break;
    case B:
      D1B_LED = OFF_VALUE;
      break;
    case C:
      D1C_LED = OFF_VALUE;
      break;
    case D:
      D1D_LED = OFF_VALUE;
      break;
    case E:
      D1E_LED = OFF_VALUE;
      break;
    case F:
      D1F_LED = OFF_VALUE;
      break;
    case G:
      D1G_LED = OFF_VALUE;
      break;
  }
}

/**
* @brief Turn off the given segment for display2
* @param segment Segment to be turned off
**/
void turn_off_display2_segment(SSDsegments segment) {
  switch (segment) {
    case A:
      D2A_LED = OFF_VALUE;
      break;
    case B:
      D2B_LED = OFF_VALUE;
      break;
    case C:
      D2C_LED = OFF_VALUE;
      break;
    case D:
      D2D_LED = OFF_VALUE;
      break;
    case E:
      D2E_LED = OFF_VALUE;
      break;
    case F:
      D2F_LED = OFF_VALUE;
      break;
    case G:
      D2G_LED = OFF_VALUE;
      break;
  }

}

/**
* @brief Set a particular segment on display1
* @param segment Segment to be set
* @param value Value to set the segment to
**/
void set_display1_segment(SSDsegments segment, uint8_t value) {
  if (value == ON_VALUE) {
    turn_on_display1_segment(segment);
  } else if (value == OFF_VALUE) {
    turn_off_display2_segment(segment);
  }
}

/**
* @brief Set a particular segment on display2
* @param segment Segment to be set
* @param value Value to set the segment to
**/
void set_display2_segment(SSDsegments segment, uint8_t value) {
  if (value == ON_VALUE) {
    turn_on_display2_segment(segment);
  } else if (value == OFF_VALUE) {
    turn_off_display2_segment(segment);
  }
}

/**
* @brief Draw a zero on the display
* @param display Display to be drawn on
**/
void draw_zero(uint8_t display) {
  if (display == 1) {
    set_display1_segment(A, ON_VALUE);
    set_display1_segment(B, ON_VALUE);
    set_display1_segment(C, ON_VALUE);
    set_display1_segment(D, ON_VALUE);
    set_display1_segment(E, ON_VALUE);
    set_display1_segment(F, ON_VALUE);
    set_display1_segment(G, OFF_VALUE);
  } else if (display == 2) {
    set_display2_segment(A, ON_VALUE);
    set_display2_segment(B, ON_VALUE);
    set_display2_segment(C, ON_VALUE);
    set_display2_segment(D, ON_VALUE);
    set_display2_segment(E, ON_VALUE);
    set_display2_segment(F, ON_VALUE);
    set_display2_segment(G, OFF_VALUE);
  }
}

/**
* @brief Draw a one on the display
* @param display Display to be drawn on
**/
void draw_one(uint8_t display) {
  if (display == 1) {
    set_display1_segment(A, OFF_VALUE);
    set_display1_segment(B, ON_VALUE);
    set_display1_segment(C, OFF_VALUE);
    set_display1_segment(D, OFF_VALUE);
    set_display1_segment(E, OFF_VALUE);
    set_display1_segment(F, OFF_VALUE);
    set_display1_segment(G, OFF_VALUE);
  } else if (display == 2) {
    set_display2_segment(A, OFF_VALUE);
    set_display2_segment(B, ON_VALUE);
    set_display2_segment(C, ON_VALUE);
    set_display2_segment(D, OFF_VALUE);
    set_display2_segment(E, OFF_VALUE);
    set_display2_segment(F, OFF_VALUE);
    set_display2_segment(G, OFF_VALUE);
  }
}

/**
* @brief Draw a two on the display
* @param display Display to be drawn on
**/
void draw_two(uint8_t display) {
  if (display == 1) {
    set_display1_segment(A, ON_VALUE);
    set_display1_segment(B, ON_VALUE);
    set_display1_segment(C, OFF_VALUE);
    set_display1_segment(D, ON_VALUE);
    set_display1_segment(E, ON_VALUE);
    set_display1_segment(F, OFF_VALUE);
    set_display1_segment(G, ON_VALUE);
  } else if (display == 2) {
    set_display2_segment(A, ON_VALUE);
    set_display2_segment(B, ON_VALUE);
    set_display2_segment(C, OFF_VALUE);
    set_display2_segment(D, ON_VALUE);
    set_display2_segment(E, ON_VALUE);
    set_display2_segment(F, OFF_VALUE);
    set_display2_segment(G, ON_VALUE);
  }
}

/**
* @brief Draw a three on the display
* @param display Display to be drawn on
**/
void draw_three(uint8_t display) {
  if (display == 1) {
    set_display1_segment(A, ON_VALUE);
    set_display1_segment(B, ON_VALUE);
    set_display1_segment(C, ON_VALUE);
    set_display1_segment(D, ON_VALUE);
    set_display1_segment(E, OFF_VALUE);
    set_display1_segment(F, OFF_VALUE);
    set_display1_segment(G, ON_VALUE);
  } else if (display == 2) {
    set_display2_segment(A, ON_VALUE);
    set_display2_segment(B, ON_VALUE);
    set_display2_segment(C, ON_VALUE);
    set_display2_segment(D, ON_VALUE);
    set_display2_segment(E, OFF_VALUE);
    set_display2_segment(F, OFF_VALUE);
    set_display2_segment(G, ON_VALUE);
  }
}

/**
* @brief Draw a four on the display
* @param display Display to be drawn on
**/
void draw_four(uint8_t display) {
  if (display == 1) {
    set_display1_segment(A, OFF_VALUE);
    set_display1_segment(B, ON_VALUE);
    set_display1_segment(C, ON_VALUE);
    set_display1_segment(D, OFF_VALUE);
    set_display1_segment(E, OFF_VALUE);
    set_display1_segment(F, ON_VALUE);
    set_display1_segment(G, ON_VALUE);
  } else if (display == 2) {
    set_display2_segment(A, OFF_VALUE);
    set_display2_segment(B, ON_VALUE);
    set_display2_segment(C, ON_VALUE);
    set_display2_segment(D, OFF_VALUE);
    set_display2_segment(E, OFF_VALUE);
    set_display2_segment(F, ON_VALUE);
    set_display2_segment(G, ON_VALUE);
  }
}

/**
* @brief Draw a five on the display
* @param display Display to be drawn on
**/
void draw_five(uint8_t display) {
  if (display == 1) {
    set_display1_segment(A, ON_VALUE);
    set_display1_segment(B, OFF_VALUE);
    set_display1_segment(C, ON_VALUE);
    set_display1_segment(D, ON_VALUE);
    set_display1_segment(E, OFF_VALUE);
    set_display1_segment(F, ON_VALUE);
    set_display1_segment(G, ON_VALUE);
  } else if (display == 2) {
    set_display2_segment(A, ON_VALUE);
    set_display2_segment(B, OFF_VALUE);
    set_display2_segment(C, ON_VALUE);
    set_display2_segment(D, ON_VALUE);
    set_display2_segment(E, OFF_VALUE);
    set_display2_segment(F, ON_VALUE);
    set_display2_segment(G, ON_VALUE);
  }
}

/**
* @brief Draw a six on the display
* @param display Display to be drawn on
**/
void draw_six(uint8_t display) {
  if (display == 1) {
    set_display1_segment(A, ON_VALUE);
    set_display1_segment(B, OFF_VALUE);
    set_display1_segment(C, ON_VALUE);
    set_display1_segment(D, ON_VALUE);
    set_display1_segment(E, ON_VALUE);
    set_display1_segment(F, ON_VALUE);
    set_display1_segment(G, ON_VALUE);
  } else if (display == 2) {
    set_display2_segment(A, ON_VALUE);
    set_display2_segment(B, OFF_VALUE);
    set_display2_segment(C, ON_VALUE);
    set_display2_segment(D, ON_VALUE);
    set_display2_segment(E, ON_VALUE);
    set_display2_segment(F, ON_VALUE);
    set_display2_segment(G, ON_VALUE);
  }
}

/**
* @brief Draw a seven on the display
* @param display Display to be drawn on
**/
void draw_seven(uint8_t display) {
  if (display == 1) {
    set_display1_segment(A, ON_VALUE);
    set_display1_segment(B, ON_VALUE);
    set_display1_segment(C, ON_VALUE);
    set_display1_segment(D, OFF_VALUE);
    set_display1_segment(E, OFF_VALUE);
    set_display1_segment(F, OFF_VALUE);
    set_display1_segment(G, OFF_VALUE);
  } else if (display == 2) {
    set_display2_segment(A, ON_VALUE);
    set_display2_segment(B, ON_VALUE);
    set_display2_segment(C, ON_VALUE);
    set_display2_segment(D, OFF_VALUE);
    set_display2_segment(E, OFF_VALUE);
    set_display2_segment(F, OFF_VALUE);
    set_display2_segment(G, OFF_VALUE);
  }
}

/**
* @brief Draw a eight on the display
* @param display Display to be drawn on
**/
void draw_eight(uint8_t display) {
  if (display == 1) {
    set_display1_segment(A, ON_VALUE);
    set_display1_segment(B, ON_VALUE);
    set_display1_segment(C, ON_VALUE);
    set_display1_segment(D, ON_VALUE);
    set_display1_segment(E, ON_VALUE);
    set_display1_segment(F, ON_VALUE);
    set_display1_segment(G, ON_VALUE);
  } else if (display == 2) {
    set_display2_segment(A, ON_VALUE);
    set_display2_segment(B, ON_VALUE);
    set_display2_segment(C, ON_VALUE);
    set_display2_segment(D, ON_VALUE);
    set_display2_segment(E, ON_VALUE);
    set_display2_segment(F, ON_VALUE);
    set_display2_segment(G, ON_VALUE);
  }
}

/**
* @brief Draw a nine on the display
* @param display Display to be drawn on
**/
void draw_nine(uint8_t display) {
  if (display == 1) {
    set_display1_segment(A, ON_VALUE);
    set_display1_segment(B, ON_VALUE);
    set_display1_segment(C, ON_VALUE);
    set_display1_segment(D, OFF_VALUE);
    set_display1_segment(E, OFF_VALUE);
    set_display1_segment(F, ON_VALUE);
    set_display1_segment(G, ON_VALUE);
  } else if (display == 2) {
    set_display2_segment(A, ON_VALUE);
    set_display2_segment(B, ON_VALUE);
    set_display2_segment(C, ON_VALUE);
    set_display2_segment(D, OFF_VALUE);
    set_display2_segment(E, OFF_VALUE);
    set_display2_segment(F, ON_VALUE);
    set_display2_segment(G, ON_VALUE);
  }
}

///////////////////////////////////////////////
//
// SSD usage
//
///////////////////////////////////////////////
/**
* @brief Draw the give number on display1
* @param number Number to be drawn
**/
void display1_draw_number(uint8_t number) {
  switch(number){
    case 0:
      draw_zero(1);
      break;
    case 2:
      draw_zero(1);
      break;
    case 3:
      draw_zero(1);
      break;
    case 4:
      draw_zero(1);
      break;
    case 5:
      draw_zero(1);
      break;
    case 6:
      draw_zero(1);
      break;
    case 7:
      draw_zero(1);
      break;
    case 8:
      draw_zero(1);
      break;
    case 9:
      draw_zero(1);
      break;
  }
}

/**
* @brief Draw the give number on display2
* @param number Number to be drawn
**/
void display2_draw_number(uint8_t number) {
  switch(number) {
    case 0:
      draw_zero(2);
      break;
    case 2:
      draw_zero(2);
      break;
    case 3:
      draw_zero(2);
      break;
    case 4:
      draw_zero(2);
      break;
    case 5:
      draw_zero(2);
      break;
    case 6:
      draw_zero(2);
      break;
    case 7:
      draw_zero(2);
      break;
    case 8:
      draw_zero(2);
      break;
    case 9:
      draw_zero(2);
      break;
  }
}
