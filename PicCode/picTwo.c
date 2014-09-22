#include "picTwoConfig.h"

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

void main() {
    pic_two_init();
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
