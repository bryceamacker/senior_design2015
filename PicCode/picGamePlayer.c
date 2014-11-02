/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: pic.c
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Controls all the game playing mechanisms for the
* SECON 2015 robot.
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        9/20/2014               SECON 2015
*********************************************************************/

#include "picGamePlayer.h"

uint8_t u8_platformPos;
uint8_t u8_twistPos;
uint8_t u8_platformFlipped;
uint8_t u8_motor;
uint8_t u8_motorFunction;
uint16_t u16_pwm;
uint16_t u16_motorPwm;
char sz_motorBuf[32];
char u8_c;

int main(void) {
    u8_c = 0;
    u8_twistPos = 0;
    u8_platformPos = 0;
    u8_platformFlipped = 0;

    // Initialize pic and print out serial menu
    configBasic(HELLO_MSG);
    pic_game_player_init();
    serial_menu();

    while(1) {
        // Handle serial command
        u8_c = inChar();
        serial_command(u8_c);

        serial_menu();
        doHeartbeat();
    }
}

void serial_command(uint8_t u8_c) {
    char u8_servo;
    if (u8_c == 'e') {
        printf("\n*** Drawing 'IEEE' ***\n");
        play_etch();
    } else if (u8_c == 'c') {
        printf("\n*** Spinng Rubiks ***\n");
        play_rubiks();
    } else if (u8_c == 's') {
        printf("\n*** Playing Simon ***\n");
        play_simon();
    } else if (u8_c == 'p') {
        if (u8_platformPos == 0) { 
            u8_platformPos = 1;
            printf("\n*** Lowering platform ***\n");
            platform_rubiks();
        } else {
            u8_platformPos = 0;
            printf("\n*** Raising platform ***\n");
            platform_up();
        }
    } else if (u8_c == 't') {
        printf("\n*** Twisting rubiks twist ***\n");
        if (u8_twistPos == 0) {
            u8_twistPos = 1;
            twist_rubiks_clock();
        } else {                
            u8_twistPos = 0;
            twist_rubiks_counter();
        }
    } else if (u8_c == 'f') {
        if (u8_platformFlipped == 0) {
            u8_platformFlipped = 1;
            printf("\n*** Flipping back platform for Etch ***\n");
            platform_etch();
        } else {
            u8_platformFlipped = 0;
            printf("\n*** Reseting platform to original position ***\n");
            platform_init();
        }
    } else if (u8_c == 'r') {
        simon_menu();
        u8_servo = inChar();

        if (u8_servo == 'a') {
            printf("\n*** Retracting all buttons ***\n");
            simon_retract_buttons();
        } else if (u8_servo == 'y') {
            printf("\n*** Retracting yellow button ***\n");
            simon_retract_button(YELLOW_BUTTON);
        } else if (u8_servo == 'b') {
            printf("\n*** Retracting blue button ***\n");
            simon_retract_button(BLUE_BUTTON);
        } else if (u8_servo == 'r') {
            printf("\n*** Retracting red button ***\n");
            simon_retract_button(RED_BUTTON);
        } else if (u8_servo == 'g') {
            printf("\n*** Retracting green button ***\n");
            simon_retract_button(GREEN_BUTTON);
        } else if (u8_servo == 's') {
            printf("\n*** Retracting start button ***\n");
            simon_retract_button(START_BUTTON);
        } else {
            printf("Invalid command");
        }
    } else if (u8_c == 'h') {
        simon_menu();
        u8_servo = inChar();

        if (u8_servo == 'a') {
            printf("\n*** Hovering all buttons ***\n");
            simon_hover_buttons();
        } else if (u8_servo == 'y') {
            printf("\n*** Hovering yellow button ***\n");
            simon_hover_button(YELLOW_BUTTON);
        } else if (u8_servo == 'b') {
            printf("\n*** Hovering blue button ***\n");
            simon_hover_button(BLUE_BUTTON);
        } else if (u8_servo == 'r') {
            printf("\n*** Hovering red button ***\n");
            simon_hover_button(RED_BUTTON);
        } else if (u8_servo == 'g') {
            printf("\n*** Hovering green button ***\n");
            simon_hover_button(GREEN_BUTTON);
        } else if (u8_servo == 's') {
            printf("\n*** Hovering start button ***\n");
            simon_hover_button(START_BUTTON);
        } else {
            printf("Invalid command");
        }
    } else if (u8_c == 'b') {
        simon_menu();
        u8_servo = inChar();

        if (u8_servo == 'a') {
            printf("\n*** Pushing all buttons ***\n");
            simon_push_buttons();
        } else if (u8_servo == 'y') {
            printf("\n*** Pushing yellow button ***\n");
            simon_push_button(YELLOW_BUTTON);
        } else if (u8_servo == 'b') {
            printf("\n*** Pushing blue button ***\n");
            simon_push_button(BLUE_BUTTON);
        } else if (u8_servo == 'r') {
            printf("\n*** Pushing red button ***\n");
            simon_push_button(RED_BUTTON);
        } else if (u8_servo == 'g') {
            printf("\n*** Pushing green button ***\n");
            simon_push_button(GREEN_BUTTON);
        } else if (u8_servo == 's') {
            printf("\n*** Pushing start button ***\n");
            simon_push_button(START_BUTTON);
        } else {
            printf("Invalid command");
        }
    } else if (u8_c == 'n') {
        simon_menu();
        u8_servo = inChar();

        if (u8_servo == 'a') {
            printf("\n*** Pushing and hover all buttons ***\n");
            simon_push_and_hover_buttons();
        } else if (u8_servo == 'y') {
            printf("\n*** Pushing and hover yellow button ***\n");
            simon_push_and_hover_button(YELLOW_BUTTON);
        } else if (u8_servo == 'b') {
            printf("\n*** Pushing and hover blue button ***\n");
            simon_push_and_hover_button(BLUE_BUTTON);
        } else if (u8_servo == 'r') {
            printf("\n*** Pushing and hover red button ***\n");
            simon_push_and_hover_button(RED_BUTTON);
        } else if (u8_servo == 'g') {
            printf("\n*** Pushing and hover green button ***\n");
            simon_push_and_hover_button(GREEN_BUTTON);
        } else if (u8_servo == 's') {
            printf("\n*** Pushing and hover start button ***\n");
            simon_push_and_hover_button(START_BUTTON);
        } else {
            printf("Invalid command");            
        }
    } else if (u8_c == 'z') {
        photo_trans_print();
    } else if (u8_c == 'x') {
        // Set a sepcific servo
        servo_menu();
        u8_servo = inChar();
        set_servo(u8_servo);
    } else {
        printf("Invalid command");
    }
}

void serial_menu(void) {
    printf("\nChoose a command\n");
    printf("   Press 'e' to play Etch-a-Sketch\n");
    printf("   Press 'c' to spin Rubiks\n");
    printf("   Press 's' to play Simon\n");
    if (u8_platformPos == 0) {
        printf("   Press 'p' to raise platform\n");
    } else {
        printf("   Press 'p' to lower platform\n");    
    }
    printf("   Press 't' to spin rubiks twist servo\n");
    if (u8_platformFlipped == 0) {
        printf("   Press 'f' to prepare for etch\n");
    } else {
        printf("   Press 'f' to reset platform to init\n");
    }
    printf("   Press 'r' to retract Simon arms\n");
    printf("   Press 'h' to hover Simon arms\n");
    printf("   Press 'b' to push Simon buttons\n");
    printf("   Press 'n' to push and hover Simon buttons\n");
    printf("   Press 'm' motor control\n");
    printf("   Press 'z' to read photo transistors\n");
    printf("   Press 'x' to set a servo\n");
}

void servo_menu(void) {
    printf("\nChoose a servo\n");
    printf("   v) Etch vertical\n");
    printf("   h) Etch horizontal\n");
    printf("   p) Platform\n");
    printf("   t) Rubiks twist\n");
    printf("   y) Simon yellow\n");
    printf("   b) Simon blue\n");
    printf("   r) Simon red\n");
    printf("   g) Simon green\n");
    printf("   e) Arm extender\n");
    printf("   i) Arm pivot\n");
}

void simon_menu(void) {
    printf("\nChoose Simon arm\n");
    printf("   a) All arms\n");
    printf("   y) Yellow arm\n");
    printf("   b) Blue arm\n");
    printf("   r) Red arm\n");
    printf("   g) Green arm\n");
    printf("   s) Start arm\n");
}

void set_servo(char u8_servo) {
    uint16_t u16_pwm;
    char sz_buf[32];
    char u8_c;
    u8_c = 0;
    
    // Get pulse width for non-continuous servos
    if (u8_servo != '0' && u8_servo != '1') {
        printf("\nEnter pulse width: ");
        inStringEcho(sz_buf,31);
        sscanf(sz_buf,"%d",(uint16_t *) &u16_pwm);
    }

    // Set servo appropriately
    if (u8_servo == 'v') {
        printf("\nUse 'a' and 'd' to move servo clockwise and counter clockwise and 'q' to quit");  
        while (u8_c != 'q') {
            u8_c = inChar();
            if (u8_c == 'a') {
                step_servo(0, ETCH_VERTICAL);
            } else if (u8_c == 'd') {
                step_servo(1, ETCH_VERTICAL);
            } else {
                stop_servo(ETCH_VERTICAL);
            }
        }
    } else if (u8_servo == 'h') {
        printf("\nUse 'a' and 'd' to move servo and 'q' to quit");  
        while (u8_c != 'q') {
            u8_c = inChar();
            if (u8_c == 'a') {
                step_servo(0, ETCH_HORIZ);
            } else if (u8_c == 'd') {
                step_servo(1, ETCH_HORIZ);
            } else {
                stop_servo(ETCH_HORIZ);
            }
        }
    } else if (u8_servo == 'p') {
        turn_servo_by_pulse(RUBIKS_PLATFORM, u16_pwm);
        printf("\n*** Setting platform to %u ***\n", u16_pwm);
    } else if (u8_servo == 't') {
        turn_servo_by_pulse(RUBIKS_TWIST, u16_pwm);
        printf("\n*** Setting rubiks twist to %u ***\n", u16_pwm);
    } else if (u8_servo == 'y') {
        turn_servo_by_pulse(SIMON_YELLOW, u16_pwm);
        printf("\n*** Setting simon yellow to %u ***\n", u16_pwm);
    } else if (u8_servo == 'b') {
        turn_servo_by_pulse(SIMON_BLUE, u16_pwm);
        printf("\n*** Setting simon blue to %u ***\n", u16_pwm);
    } else if (u8_servo == 'r') {
        turn_servo_by_pulse(SIMON_RED, u16_pwm);
        printf("\n*** Setting simon red to %u ***\n", u16_pwm);
    } else if (u8_servo == 'g') {
        turn_servo_by_pulse(SIMON_GREEN, u16_pwm);
        printf("\n*** Setting simon green to %u ***\n", u16_pwm);
    } else if (u8_servo == 'e') {
        turn_servo_by_pulse(ARM_EXTEND, u16_pwm);
        printf("\n*** Setting arm extend to %u ***\n", u16_pwm);
    } else if (u8_servo == 'i') {
        turn_servo_by_pulse(ARM_PIVOT, u16_pwm);
        printf("\n*** Setting arm pivot to %u ***\n", u16_pwm);
    } else {
        printf("Invalid choice\n");
    }
}

void I2C_check_command(volatile char *psz_s1) {
    if (strcmp(psz_s1, "Etch") == 0) {
        printf("\n*** Drawing 'IEEE' ***\n");
        play_etch();
    } else if(strcmp(psz_s1, "Rubiks") == 0) {  
        printf("\n*** Spinng Rubiks ***\n");
        play_rubiks();
    } else if(strcmp(psz_s1, "Simon") == 0) {
        printf("\n*** Playing Simon ***\n");
        play_simon();
    } else if(strcmp(psz_s1, "Card") == 0) {
        printf("\n*** Playing Card ***\n");
    } else {
        printf("\nInvalid commmand\n");
        printf("%s\n", psz_s1);
    }
    serial_menu();
}

void _ISRFAST _SI2C1Interrupt(void) {
  uint8_t u8_c;
  _SI2C1IF = 0;
  switch (e_mystate) {
    case STATE_WAIT_FOR_ADDR:
      u8_c = I2C1RCV; //clear RBF bit for address
      u16_index = 0;
      //check the R/W bit and see if read or write transaction
      if (I2C1STATbits.R_W) {
        I2C1TRN = sz_2[u16_index++];  //get first data byte
        I2C1CONbits.SCLREL = 1;     //release clock line so MASTER can drive it
        e_mystate = STATE_SEND_READ_DATA; //read transaction
      } else e_mystate = STATE_WAIT_FOR_WRITE_DATA;
      break;
    case STATE_WAIT_FOR_WRITE_DATA:
      //character arrived, place in buffer
      sz_1[u16_index++] = I2C1RCV;  //read the byte
      if (sz_1[u16_index-1] == 0) {
        //have a complete string, check the command
        I2C_check_command(sz_1);
        e_mystate = STATE_WAIT_FOR_ADDR; //wait for next transaction
      }
      break;
    case STATE_SEND_READ_DATA:
      I2C1TRN = sz_2[u16_index++];
      I2C1CONbits.SCLREL = 1;     //release clock line so MASTER can drive it
      if (sz_2[u16_index-1] == 0) e_mystate = STATE_SEND_READ_LAST;
      //this is the last character, after byte is shifted out, release the clock line again
      break;
    case STATE_SEND_READ_LAST:  //this is interrupt for last character finished shifting out
      e_mystate = STATE_WAIT_FOR_ADDR;
      break;
    default:
      e_mystate = STATE_WAIT_FOR_ADDR;

  }
}

