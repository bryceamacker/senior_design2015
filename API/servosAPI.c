#include "servosAPI.h"
/*
// Servo config
void servo_init() {
    uint8_t u8_i;
    uint16_t u16_initPW;
    u8_currentServo = 0;

    CONFIG_RD11_AS_DIG_OUTPUT();

    u16_initPW = usToU16Ticks(MIN_PW + (MAX_PW-MIN_PW)/2, getTimerPrescale(T3CONbits));

    //config all servos for half maximum pulse width

    for (u8_i=0; u8_i<NUM_SERVOS; u8_i++) au16_servoPWidths[u8_i]=u16_initPW;

    SERVO0 = 0; //all servo outputs low initially

    u16_slotWidthTicks = usToU16Ticks(SLOT_WIDTH, getTimerPrescale(T3CONbits));
}

void config_timer3(void) {
    T3CON = T3_OFF 
            | T3_IDLE_CON 
            | T3_GATE_OFF
            | T3_32BIT_MODE_OFF
            | T3_SOURCE_INT
            | T3_PS_1_256 ;  //1 tick = 1.6 us at FCY=40 MHz
 PR2 = usToU16Ticks(PWM_PERIOD, getTimerPrescale(T2CONbits)) - 1;
 TMR2  = 0;       //clear timer2 value
}


// Servo primitives
void turn_servo(servoIDs id, float degrees) {

}

// Servo usage
void push_simon_button (buttonIDs id) {

}

void turn_servo_clockwise(servoIDs id, float degrees) {

}

void turn_servo_counterwise(servoIDs id, float degrees) {

}

*/