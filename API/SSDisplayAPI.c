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
    default:
      D1A_LED = ON_VALUE;
      break;
  }
}

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
    default:
      D2A_LED = ON_VALUE;
      break;
  }
}

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
    default:
      D1A_LED = OFF_VALUE;
      break;
  }
}

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
    default:
      D2A_LED = OFF_VALUE;
      break;
  }

}

void set_display1_segment(SSDsegments segment, uint8_t u8_value) {
  if (u8_value == ON_VALUE) {
    turn_on_display1_segment(segment);
  } else if (u8_value == OFF_VALUE) {
    turn_off_display1_segment(segment);
  }
}

void set_display2_segment(SSDsegments segment, uint8_t u8_value) {
  if (u8_value == ON_VALUE) {
    turn_on_display2_segment(segment);
  } else if (u8_value == OFF_VALUE) {
    turn_off_display2_segment(segment);
  }
}

void draw_zero(uint8_t u8_display) {
  if (u8_display == 1) {
    set_display1_segment(A, ON_VALUE);
    set_display1_segment(B, ON_VALUE);
    set_display1_segment(C, ON_VALUE);
    set_display1_segment(D, ON_VALUE);
    set_display1_segment(E, ON_VALUE);
    set_display1_segment(F, ON_VALUE);
    set_display1_segment(G, OFF_VALUE);
  } else if (u8_display == 2) {
    set_display2_segment(A, ON_VALUE);
    set_display2_segment(B, ON_VALUE);
    set_display2_segment(C, ON_VALUE);
    set_display2_segment(D, ON_VALUE);
    set_display2_segment(E, ON_VALUE);
    set_display2_segment(F, ON_VALUE);
    set_display2_segment(G, OFF_VALUE);
  }
}

void draw_one(uint8_t u8_display) {
  if (u8_display == 1) {
    set_display1_segment(A, OFF_VALUE);
    set_display1_segment(B, ON_VALUE);
    set_display1_segment(C, ON_VALUE);
    set_display1_segment(D, OFF_VALUE);
    set_display1_segment(E, OFF_VALUE);
    set_display1_segment(F, OFF_VALUE);
    set_display1_segment(G, OFF_VALUE);
  } else if (u8_display == 2) {
    set_display2_segment(A, OFF_VALUE);
    set_display2_segment(B, ON_VALUE);
    set_display2_segment(C, ON_VALUE);
    set_display2_segment(D, OFF_VALUE);
    set_display2_segment(E, OFF_VALUE);
    set_display2_segment(F, OFF_VALUE);
    set_display2_segment(G, OFF_VALUE);
  }
}

void draw_two(uint8_t u8_display) {
  if (u8_display == 1) {
    set_display1_segment(A, ON_VALUE);
    set_display1_segment(B, ON_VALUE);
    set_display1_segment(C, OFF_VALUE);
    set_display1_segment(D, ON_VALUE);
    set_display1_segment(E, ON_VALUE);
    set_display1_segment(F, OFF_VALUE);
    set_display1_segment(G, ON_VALUE);
  } else if (u8_display == 2) {
    set_display2_segment(A, ON_VALUE);
    set_display2_segment(B, ON_VALUE);
    set_display2_segment(C, OFF_VALUE);
    set_display2_segment(D, ON_VALUE);
    set_display2_segment(E, ON_VALUE);
    set_display2_segment(F, OFF_VALUE);
    set_display2_segment(G, ON_VALUE);
  }
}

void draw_three(uint8_t u8_display) {
  if (u8_display == 1) {
    set_display1_segment(A, ON_VALUE);
    set_display1_segment(B, ON_VALUE);
    set_display1_segment(C, ON_VALUE);
    set_display1_segment(D, ON_VALUE);
    set_display1_segment(E, OFF_VALUE);
    set_display1_segment(F, OFF_VALUE);
    set_display1_segment(G, ON_VALUE);
  } else if (u8_display == 2) {
    set_display2_segment(A, ON_VALUE);
    set_display2_segment(B, ON_VALUE);
    set_display2_segment(C, ON_VALUE);
    set_display2_segment(D, ON_VALUE);
    set_display2_segment(E, OFF_VALUE);
    set_display2_segment(F, OFF_VALUE);
    set_display2_segment(G, ON_VALUE);
  }
}

void draw_four(uint8_t u8_display) {
  if (u8_display == 1) {
    set_display1_segment(A, OFF_VALUE);
    set_display1_segment(B, ON_VALUE);
    set_display1_segment(C, ON_VALUE);
    set_display1_segment(D, OFF_VALUE);
    set_display1_segment(E, OFF_VALUE);
    set_display1_segment(F, ON_VALUE);
    set_display1_segment(G, ON_VALUE);
  } else if (u8_display == 2) {
    set_display2_segment(A, OFF_VALUE);
    set_display2_segment(B, ON_VALUE);
    set_display2_segment(C, ON_VALUE);
    set_display2_segment(D, OFF_VALUE);
    set_display2_segment(E, OFF_VALUE);
    set_display2_segment(F, ON_VALUE);
    set_display2_segment(G, ON_VALUE);
  }
}

void draw_five(uint8_t u8_display) {
  if (u8_display == 1) {
    set_display1_segment(A, ON_VALUE);
    set_display1_segment(B, OFF_VALUE);
    set_display1_segment(C, ON_VALUE);
    set_display1_segment(D, ON_VALUE);
    set_display1_segment(E, OFF_VALUE);
    set_display1_segment(F, ON_VALUE);
    set_display1_segment(G, ON_VALUE);
  } else if (u8_display == 2) {
    set_display2_segment(A, ON_VALUE);
    set_display2_segment(B, OFF_VALUE);
    set_display2_segment(C, ON_VALUE);
    set_display2_segment(D, ON_VALUE);
    set_display2_segment(E, OFF_VALUE);
    set_display2_segment(F, ON_VALUE);
    set_display2_segment(G, ON_VALUE);
  }
}

void draw_six(uint8_t u8_display) {
  if (u8_display == 1) {
    set_display1_segment(A, ON_VALUE);
    set_display1_segment(B, OFF_VALUE);
    set_display1_segment(C, ON_VALUE);
    set_display1_segment(D, ON_VALUE);
    set_display1_segment(E, ON_VALUE);
    set_display1_segment(F, ON_VALUE);
    set_display1_segment(G, ON_VALUE);
  } else if (u8_display == 2) {
    set_display2_segment(A, ON_VALUE);
    set_display2_segment(B, OFF_VALUE);
    set_display2_segment(C, ON_VALUE);
    set_display2_segment(D, ON_VALUE);
    set_display2_segment(E, ON_VALUE);
    set_display2_segment(F, ON_VALUE);
    set_display2_segment(G, ON_VALUE);
  }
}

void draw_seven(uint8_t u8_display) {
  if (u8_display == 1) {
    set_display1_segment(A, ON_VALUE);
    set_display1_segment(B, ON_VALUE);
    set_display1_segment(C, ON_VALUE);
    set_display1_segment(D, OFF_VALUE);
    set_display1_segment(E, OFF_VALUE);
    set_display1_segment(F, OFF_VALUE);
    set_display1_segment(G, OFF_VALUE);
  } else if (u8_display == 2) {
    set_display2_segment(A, ON_VALUE);
    set_display2_segment(B, ON_VALUE);
    set_display2_segment(C, ON_VALUE);
    set_display2_segment(D, OFF_VALUE);
    set_display2_segment(E, OFF_VALUE);
    set_display2_segment(F, OFF_VALUE);
    set_display2_segment(G, OFF_VALUE);
  }
}

void draw_eight(uint8_t u8_display) {
  if (u8_display == 1) {
    set_display1_segment(A, ON_VALUE);
    set_display1_segment(B, ON_VALUE);
    set_display1_segment(C, ON_VALUE);
    set_display1_segment(D, ON_VALUE);
    set_display1_segment(E, ON_VALUE);
    set_display1_segment(F, ON_VALUE);
    set_display1_segment(G, ON_VALUE);
  } else if (u8_display == 2) {
    set_display2_segment(A, ON_VALUE);
    set_display2_segment(B, ON_VALUE);
    set_display2_segment(C, ON_VALUE);
    set_display2_segment(D, ON_VALUE);
    set_display2_segment(E, ON_VALUE);
    set_display2_segment(F, ON_VALUE);
    set_display2_segment(G, ON_VALUE);
  }
}

void draw_nine(uint8_t u8_display) {
  if (u8_display == 1) {
    set_display1_segment(A, ON_VALUE);
    set_display1_segment(B, ON_VALUE);
    set_display1_segment(C, ON_VALUE);
    set_display1_segment(D, OFF_VALUE);
    set_display1_segment(E, OFF_VALUE);
    set_display1_segment(F, ON_VALUE);
    set_display1_segment(G, ON_VALUE);
  } else if (u8_display == 2) {
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
void display1_draw_number(uint8_t u8_number) {
  switch(u8_number){
    case 0:
      draw_zero(1);
      break;
    case 1:
      draw_one(1);
      break;
    case 2:
      draw_two(1);
      break;
    case 3:
      draw_three(1);
      break;
    case 4:
      draw_four(1);
      break;
    case 5:
      draw_five(1);
      break;
    case 6:
      draw_six(1);
      break;
    case 7:
      draw_seven(1);
      break;
    case 8:
      draw_eight(1);
      break;
    case 9:
      draw_nine(1);
      break;
  }
}

void display2_draw_number(uint8_t u8_number) {
  switch(u8_number){
    case 0:
      draw_zero(2);
      break;
    case 1:
      draw_one(2);
      break;
    case 2:
      draw_two(2);
      break;
    case 3:
      draw_three(2);
      break;
    case 4:
      draw_four(2);
      break;
    case 5:
      draw_five(2);
      break;
    case 6:
      draw_six(2);
      break;
    case 7:
      draw_seven(2);
      break;
    case 8:
      draw_eight(2);
      break;
    case 9:
      draw_nine(2);
      break;
  }
}

void display_draw_number(uint8_t u8_number) {
    uint8_t u8_tensDigit;
    uint8_t u8_onesDigit;

    u8_tensDigit = u8_number/10;
    u8_onesDigit = u8_number - (10*u8_tensDigit);

    display1_draw_number(u8_tensDigit);
    display2_draw_number(u8_onesDigit);
}

#ifdef DEBUG_BUILD
void test_ss_displays(void) {
    uint8_t u8_count;
    u8_count = 0;

    while (1) {
        display_draw_number(u8_count);
        printf("Current number: %u\n", u8_count);

        u8_count++;
        if (u8_count > 99) {
            return;
        }

        DELAY_MS(500);
    }
}
#endif