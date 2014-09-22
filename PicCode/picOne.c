#include "picOneConfig.h"

typedef enum {
    FOLLOW_LINE     = 0,
    GO_TO_FINISH    = 1
} picOneCommands_t;

typedef enum  {
  STATE_WAIT_FOR_ADDR,
  STATE_WAIT_FOR_WRITE_DATA,
  STATE_SEND_READ_DATA
} I2C_STATE;

picOneCommands_t        u8_command;
I2C_STATE               currentState;

void main() {
    pic_one_init();

    while(1) {
        get_motor_value();

    }
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
