#include "pic_config.h"


typedef enum {
    PLAY_SIMON      = 0,
    PLAY_ETCH       = 1,
    PLAY_RUBIK      = 2,
    PLAY_CARDS      = 3
} picTwoCommands_t;

typedef enum  {
  STATE_WAIT_FOR_ADDR,
  STATE_WAIT_FOR_WRITE_DATA,
  STATE_SEND_READ_DATA
} I2C_STATE;

picTwoCommands_t        u8_command;
I2C_STATE               currentState;

int main(void) {
    uint8_t u8_c;

    configBasic(HELLO_MSG);
    pic_init();

    rubik_init();
    servo_calibration_mode();
    serial_menu();

    while(1) {
        u8_c = inChar();
        if (u8_c == 's') {
            // Etch-a-sketch
            u8_c = 0;
            outString("Drawing 'IEEE'\n");
            draw_IEEE();
            underline_to_reset();
            servo_calibration_mode();
            serial_menu();
        } else if (u8_c == 'r') {
            // Rubiks cube
            u8_c = 0;
            outString("Spinng Rubiks\n");
            play_rubiks();
            rubik_init();
            serial_menu();
        } else if (u8_c == 'u') {
            u8_c = 0;
            platform_up();
        } else if (u8_c == 'd') {
            u8_c = 0;
            platform_down();
        } else if (u8_c == 't') {
            u8_c = 0;
            turn_servo(RUBIKS_TWIST, 2400);
        } else if (u8_c == 'y') {
            u8_c = 0;
            turn_servo(RUBIKS_TWIST, 600);
        }
        doHeartbeat();
    }
}

void serial_menu(void) {
    outString("\nIn calibrate mode'\n");
    outString("\tPress 's' to draw IEEE\n");
    outString("\tPress 'r' to sping rubiks\n");    
}

void _ISRFAST _SI2C1Interrupt(void) {
    // Variable to clear RBF bit
    uint8_t u8_c;
    _SI2C1IF = 0;

    // No longer controlled by master's clock
    I2C1CONbits.SCLREL = 0;

    switch (currentState) {
        case STATE_WAIT_FOR_ADDR:
            //clear RBF bit for address
            u8_c = I2C1RCV;

            // Check the R/W bit and see if read or write transaction
            if (I2C1STATbits.R_W) { 
                // Send number
                I2C1TRN = u8_command;
                
                // Release clock line so MASTER can drive it
                I2C1CONbits.SCLREL = 1;

                // Change to send data
                currentState = STATE_SEND_READ_DATA;
            } 
            else {
                currentState = STATE_WAIT_FOR_WRITE_DATA;
            }
        break;

    case STATE_WAIT_FOR_WRITE_DATA:
        // Read the byte
        u8_command = I2C1RCV;

        printf("Recieved Command: %u\n", u8_command);

        //////////////////////////////////////////////////////
        // I2C commands go here


        //////////////////////////////////////////////////////

        // Wait for next transaction
        currentState = STATE_WAIT_FOR_ADDR;
        break;
      
    case STATE_SEND_READ_DATA:
        // Send number
        I2C1TRN = u8_command;

        printf("Transmitted number: %u\n", u8_command);

        // Release clock line so MASTER can drive it
        I2C1CONbits.SCLREL = 1;

        // Wait for next transaction
        currentState = STATE_WAIT_FOR_ADDR;
        break;
      
    default:
        currentState = STATE_WAIT_FOR_ADDR;
    }
}
