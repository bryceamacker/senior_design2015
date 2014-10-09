#include "pic.h"
#include <stdio.h>

int main(void) {
    char u8_c, u8_servo;
    char sz_buf[32];
    uint16_t u16_pwm;

    // Initialize pic an dprint out serial menu
    pic_init();
    configBasic(HELLO_MSG);
    serial_menu();

    while(1) {
        u8_c = inChar();

        if (u8_c == 's') {
            printf("\n*** Drawing 'IEEE' ***\n");
            draw_IEEE();
        } else if (u8_c == 'r') {
            printf("\n*** Spinng Rubiks ***\n");
            play_rubiks();
        } else if (u8_c == 'e') {
            printf("\n*** Reseting Rubiks ***\n");
            rubik_init();
        } else if (u8_c == 'u') {
            printf("\n*** Raising platform ***\n");
            platform_up();
        } else if (u8_c == 'd') {
            printf("\n*** Lowering platform ***\n");
            platform_down();
        } else if (u8_c == 't') {
            printf("\n*** Twisting rubiks twist ***\n");
            twist_rubiks_counter();
        } else if (u8_c == 'y') {
            printf("\n*** Twisting rubiks twist the other way ***\n");
            twist_rubiks_clock();
        } else if (u8_c == 'h') {
            printf("\n*** Preparing for Etch ***\n");
            etch_prepare();
        } else if (u8_c == 'x') {
            // Set servo
            servo_menu();
            u8_servo = inChar();

            printf("\nEnter pulse width: ");
            inStringEcho(sz_buf,31);
            sscanf(sz_buf,"%d",(uint16_t *) &u16_pwm);
            set_servo(u8_servo, u16_pwm);
        } else {
            printf("Invalid command");
        }
        serial_menu();
        doHeartbeat();
    }
}

void serial_menu(void) {
    printf("\nChoose a command\n");
    printf("   Press 's' to draw IEEE\n");
    printf("   Press 'r' to spin rubiks\n");
    printf("   Press 'e' to reset rubiks platform and twist\n");
    printf("   Press 'u' to raise platform\n");
    printf("   Press 'd' to lower platform\n");    
    printf("   Press 't' to spin rubks twist servo\n");
    printf("   Press 'y' to spin rubks twist servo the other way\n");
    printf("   Press 'h' to prepare for etch\n");
    printf("   Press 'x' to set a servo\n");
}

void servo_menu(void) {
    printf("\nChoose a servo\n");
    printf("   0) Etch vertical\n");
    printf("   1) Etch horizontal\n");
    printf("   2) Platform\n");
    printf("   3) Rubiks twist\n");
    printf("   4) Arm extender\n");
    printf("   5) Simon yellow\n");
    printf("   6) Simon blue\n");
    printf("   7) Simon red\n");
    printf("   8) Simon green\n");
    printf("   9) Arm pivot\n");
}

void set_servo(char u8_servo, uint16_t u16_pwm) {
    if (u8_servo == '0') {
        turn_servo_by_pulse(ETCH_VERTICAL, u16_pwm);
        printf("\n*** Setting etch vertical to %u ***\n", u16_pwm);
    } else if (u8_servo == '1') {
        turn_servo_by_pulse(ETCH_HORIZ, u16_pwm);
        printf("\n*** Setting etch horizontal to %u ***\n", u16_pwm);
    } else if (u8_servo == '2') {
        turn_servo_by_pulse(RUBIKS_PLATFORM, u16_pwm);
        printf("\n*** Setting platform to %u ***\n", u16_pwm);
    } else if (u8_servo == '3') {
        turn_servo_by_pulse(RUBIKS_TWIST, u16_pwm);
        printf("\n*** Setting rubiks twist to %u ***\n", u16_pwm);
    } else if (u8_servo == '4') {
        turn_servo_by_pulse(ARM_EXTEND, u16_pwm);
        printf("\n*** Setting arm extend to %u ***\n", u16_pwm);
    } else if (u8_servo == '5') {
        turn_servo_by_pulse(SIMON_YELLOW, u16_pwm);
        printf("\n*** Setting simon yellow to %u ***\n", u16_pwm);
    } else if (u8_servo == '6') {
        turn_servo_by_pulse(SIMON_BLUE, u16_pwm);
        printf("\n*** Setting simon blue to %u ***\n", u16_pwm);
    } else if (u8_servo == '7') {
        turn_servo_by_pulse(SIMON_RED, u16_pwm);
        printf("\n*** Setting simon red to %u ***\n", u16_pwm);
    } else if (u8_servo == '8') {
        turn_servo_by_pulse(SIMON_GREEN, u16_pwm);
        printf("\n*** Setting simon green to %u ***\n", u16_pwm);
    } else if (u8_servo == '9') {
        turn_servo_by_pulse(ARM_PIVOT, u16_pwm);
        printf("\n*** Setting arm pivot to %u ***\n", u16_pwm);
    } else {
        printf("Invalid choice\n");
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
