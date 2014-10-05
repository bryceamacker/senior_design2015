#include "servosAPI.h"

static volatile uint16_t    au16_servoPWidths[NUM_SERVOS];  // Array to hold a servo pulse widths
volatile uint8_t            u8_currentServo = 0;            // Servo currently being controlled
volatile uint8_t            u8_servoEdge = 1;               // 1 = RISING, 0 = FALLING
volatile uint16_t           u16_slotWidthTicks = 0;         // Slot width in ticks

/////////////////////////////////////////////// 
//
// Servo config
//
///////////////////////////////////////////////

void servo_init() {
    uint8_t u8_i;
    uint16_t u16_initPW;
    u8_currentServo = 0;
    
    config_servo_timer3();
    config_output_capture1();
    CONFIG_RD1_AS_DIG_OUTPUT();
    CONFIG_RD2_AS_DIG_OUTPUT();
    CONFIG_RD10_AS_DIG_OUTPUT();
    CONFIG_RD11_AS_DIG_OUTPUT();
    T3CONbits.TON = 1;
    
    u16_initPW = usToU16Ticks(MIN_PW + (MAX_PW-MIN_PW)/2, getTimerPrescale(T3CONbits));


    // Initialize all servo speeds as stopped
    for (u8_i=0; u8_i<NUM_SERVOS; u8_i++) stop_servo(u8_i);

    // All servo outputs low initially
    ETCH_VERTICAL_PIN = 0;
    ETCH_HORIZ_PIN = 0;

    u16_slotWidthTicks = usToU16Ticks(SLOT_WIDTH, getTimerPrescale(T3CONbits));
}

void config_servo_timer3(void) {
    // Config bits
    T3CON = T3_OFF 
            | T3_IDLE_CON 
            | T2_32BIT_MODE_OFF
            | T3_GATE_OFF
            | T3_SOURCE_INT
            | T3_PS_1_256 ;  //1 tick = 1.6 us at FCY=40 MHz
    PR3 = usToU16Ticks(PWM_PERIOD, getTimerPrescale(T3CONbits)) - 1;
    TMR3  = 0;       //clear timer3 value
}

void _ISR _OC1Interrupt(void) {
    _OC1IF = 0;

    // Change the servo's value
    set_servo_output(u8_currentServo, u8_servoEdge);

    // Schedule next interrupt
    if (u8_servoEdge == 1) {  // Rising edge
       // Next interrupt occurs after pulse width has elapsed
       OC1R = OC1R + au16_servoPWidths[u8_currentServo];
       u8_servoEdge = 0;     // Change to falling edge

    } else { // Falling edge
        // Next interrupt occurs at beginning of next slot
        if (u8_currentServo != NUM_SERVOS - 1) {
            OC1R = u16_slotWidthTicks*(u8_currentServo+1);
        } else { // Last servo
            OC1R = 0;
        }
        u8_servoEdge = 1;     // Change to rising edge
        u8_currentServo++;
        if (u8_currentServo == NUM_SERVOS) 
            u8_currentServo = 0;
    }
}

void config_output_capture1(void) {
    T3CONbits.TON = 0;       //disable Timer when configuring Output compare
    OC1R  =  0;  //initialize to 0
    //turn on the compare toggle mode using Timer3
#ifdef OC1CON1
    OC1CON1 =   OC_TIMER3_SRC |      //Timer3 source
                OC_TOGGLE_PULSE;     //single compare toggle, just care about compare event
    OC1CON2 =   OC_SYNCSEL_TIMER3;   //synchronize to timer2
#else
    OC1CON =    OC_TIMER3_SRC |      //Timer3 source
                OC_TOGGLE_PULSE;     //single compare toggle, just care about compare event
#endif
    _OC1IF = 0;
    _OC1IP = 1;
    _OC1IE = 1;    //enable the OC1 interrupt
}


/////////////////////////////////////////////// 
//
// Servo primatives
//
///////////////////////////////////////////////

void turn_servo(servoIDs id, int pulseWidth) {
    _OC1IE = 0; //disable the interrupt while changing
    au16_servoPWidths[id] = usToU16Ticks(pulseWidth, getTimerPrescale(T3CONbits));
    _OC1IE = 1;
    DELAY_MS(100);
}

void set_servo_output (uint8_t u8_servo, uint8_t u8_val) {
    switch (u8_servo) {
        case 0:
            ETCH_VERTICAL_PIN = u8_val;
            break;
        case 1:
            ETCH_HORIZ_PIN = u8_val;
            break;
        case 2:
            RUBIKS_PLATFORM_PIN = u8_val;
            break;
        case 3:
            RUBIKS_TWIST_PIN = u8_val;
            break;
        default:
            break;
    }
}

/////////////////////////////////////////////// 
//
// Servo usage
//
///////////////////////////////////////////////

void stop_servo (servoIDs id) {
    turn_servo(id, 1500);
}

void turn_servo_clockwise(servoIDs id, float degrees) {
    uint16_t u16_delayTime = (degrees/360) * CONT_CLOCK_FULL_ROTATION_TIME;

    turn_servo(id, 2000);
    DELAY_MS(u16_delayTime);
    turn_servo(id, 1500);
}

void turn_servo_counterwise(servoIDs id, float degrees) {
    uint16_t u16_delayTime = (degrees/360) * CONT_COUNTER_FULL_ROTATION_TIME;

    turn_servo(id, 1000);
    DELAY_MS(u16_delayTime);
    turn_servo(id, 1500);
}

void turn_servo_to_degree(servoIDs id, float degree) {
    if (degree == 0) {
        turn_servo(id, 1000);
    } else {
        turn_servo(id, ((degree/180) * 1000) + 1000);
    }
}

void servo_calibration_mode(void) {
    uint8_t u8_i;
    for (u8_i=0; u8_i<NUM_SERVOS; u8_i++) stop_servo(u8_i);
}