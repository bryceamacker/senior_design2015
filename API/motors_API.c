/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: motors_API.c
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Functions to help control DC motors
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        9/20/2014               SECON 2015
*********************************************************************/

#include "motors_API.h"

// Current encoder count for each motor
volatile int16_t i16_leftCounterROT = 0;
volatile int16_t i16_rightCounterROT = 0;

// Direction of each motor
uint8_t u8_leftMotorDirection = 0;
uint8_t u8_rightMotorDirection = 0;

// Current revolution count for each motor
volatile int16_t i16_rightRevolutionCount = 5000;
volatile int16_t i16_leftRevolutionCount = 5000;

// Target locations for each motor
float f_rightTargetPosition = 5000.0;
float f_leftTargetPosition = 5000.0;

// Whether or not the motors have reached their target location
uint8_t u8_rightAtTarget = 1;
uint8_t u8_leftAtTarget = 1;

// Delay time for 90 degreen turns
uint16_t u16_90DegreeTurnTime;
uint16_t u16_prepareTurnDelayTime;


///////////////////////////////////////////////
//
// Motor config
//
///////////////////////////////////////////////

void motors_init(void)
{
    // Configure timers
    config_motor_timer2();
    config_motor_timer3();

    // Configure output comparators
    motor_config_output_compare2();
    motor_config_output_compare3();
    motor_config_output_compare4();
    motor_config_output_compare5();

    // Turn on timers
    T2CONbits.TON = 1;
    T3CONbits.TON = 1;

    // Configure the pins for the motors
    CONFIG_RIGHT_MOTOR_IN1();
    CONFIG_RIGHT_MOTOR_IN2();
    CONFIG_LEFT_MOTOR_IN1();
    CONFIG_LEFT_MOTOR_IN2();

    // Config the encoders
    config_encoder_interrupts();

    // Stop both motors
    motors_stop();

    // Set up some constants that we can set from serial menu
    u16_90DegreeTurnTime = DEGREE_90_TURN_TIME;
    u16_prepareTurnDelayTime = PREPARE_TURN_TIME;
}

void config_motor_timer2(void) {
    T2CON = T2_OFF
            | T2_IDLE_CON
            | T2_GATE_OFF
            | T2_32BIT_MODE_OFF
            | T2_SOURCE_INT
            | T2_PS_1_64;
    PR2 = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T2CONbits)) - 1;
    //PR2 = PULSEWIDTH1;
    TMR2  = 0;       //clear timer2 value
    _T2IF = 0;
    _T2IP = 1;
    _T2IE = 1;    //enable the Timer2 interrupt
}

void config_motor_timer3(void) {
    T3CON = T3_OFF
            | T3_IDLE_CON
            | T3_GATE_OFF
            | T2_32BIT_MODE_OFF
            | T3_SOURCE_INT
            | T3_PS_1_64;
    PR3 = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T3CONbits)) - 1;
    //PR3 = PULSEWIDTH1;
    TMR3  = 0;       //clear timer3 value
    _T3IF = 0;
    _T3IP = 1;
    _T3IE = 1;    //enable the Timer3 interrupt
}

void config_encoder_interrupts(void) {
    // Right encoder
    _INT0IF = 0;     //Clear the interrupt flag
    _INT0IP = 2;     //Choose a priority
    _INT0EP = 0;     //positive edge triggerred
    _INT0IE = 1;     //enable INT0 interrupt

    // Left encoder
    _INT1IF = 0;     //Clear the interrupt flag
    _INT1IP = 2;     //Choose a priority
    _INT1EP = 0;     //positive edge triggerred
    _INT1IE = 1;     //enable INT1 interrupt

    // Enable the input pins
    CONFIG_RF6_AS_DIG_INPUT();
    CONFIG_RD8_AS_DIG_INPUT();
}

void motor_config_output_compare2(void) {
    T2CONbits.TON = 0;          //disable Timer when configuring Output compare
    //CONFIG_OC2_TO_RP(RB4_RP);   //map OC2 to RB4
    // OC2CONbits.OCM2 = 0b110;
    OC2CONbits.OCM = 0b110; // This is possibly incorrect and may need the line above it to work, but OCM2 is 1 bit, OCM is the 3 bit register

    OC2RS = 0;  //clear both registers
    OC2R = 0;
    #ifdef OC2CON1
        //turn on the compare toggle mode using Timer2
        OC2CON1 = OC_TIMER2_SRC |     //Timer2 source
                    OC_PWM_CENTER_ALIGN;  //PWM
        OC2CON2 = OC_SYNCSEL_TIMER2;   //synchronize to timer2
    #else
        //older families, this PWM mode is compatible with center-aligned, OC2R=0
        //as writes to OC2RS sets the pulse widith.
        OC2CON = OC_TIMER2_SRC //Timer3 source
                    | OC_PWM_FAULT_PIN_DISABLE;  //PWM, no fault detection
    #endif
}

void motor_config_output_compare3(void) {
    T2CONbits.TON = 0;          //disable Timer when configuring Output compare
    //CONFIG_OC1_TO_RP(RB4_RP);   //map OC1 to RB4
    // OC3CONbits.OCM2 = 0b110;
    OC3CONbits.OCM = 0b110; // This is possibly incorrect and may need the line above it to work, but OCM2 is 1 bit, OCM is the 3 bit register

    OC3RS = 0;  //clear both registers
    OC3R = 0;
    #ifdef OC3CON2
        //turn on the compare toggle mode using Timer3
        OC3CON1 = OC_TIMER2_SRC |     //Timer3 source
                    OC_PWM_CENTER_ALIGN;  //PWM
        OC3CON2 = OC_SYNCSEL_TIMER2;   //synchronize to timer3
    #else
        //older families, this PWM mode is compatible with center-aligned, OC1R=0
        //as writes to OC1RS sets the pulse widith.
        OC3CON = OC_TIMER2_SRC      //Timer2 source
                | OC_PWM_FAULT_PIN_DISABLE;  //PWM, no fault detection
    #endif
}

void motor_config_output_compare4(void) {
    T3CONbits.TON = 0;          //disable Timer when configuring Output compare
    //CONFIG_OC1_TO_RP(RB4_RP);   //map OC1 to RB4
    // OC4CONbits.OCM2 = 0b110;
    OC4CONbits.OCM = 0b110; // This is possibly incorrect and may need the line above it to work, but OCM2 is 1 bit, OCM is the 3 bit register

    OC4RS = 0;  //clear both registers
    OC4R = 0;
    #ifdef OC4CON2
        //turn on the compare toggle mode using Timer3
        OC4CON1 = OC_TIMER3_SRC     //Timer3 source
                   | OC_PWM_CENTER_ALIGN;  //PWM
        OC4CON2 = OC_SYNCSEL_TIMER3;   //synchronize to timer3
    #else
        //older families, this PWM mode is compatible with center-aligned, OC1R=0
        //as writes to OC1RS sets the pulse widith.
        OC4CON = OC_TIMER3_SRC     //Timer3 source
                | OC_PWM_FAULT_PIN_DISABLE;  //PWM, no fault detection
    #endif
}

void motor_config_output_compare5(void) {
    T3CONbits.TON = 0;          //disable Timer when configuring Output compare
    //CONFIG_OC1_TO_RP(RB4_RP);   //map OC1 to RB4
    // OC5CONbits.OCM2 = 0b110;
    OC5CONbits.OCM = 0b110; // This is possibly incorrect and may need the line above it to work, but OCM2 is 1 bit, OCM is the 3 bit register

    OC5RS = 0;  //clear both registers
    OC5R = 0;
    #ifdef OC5CON2
        //turn on the compare toggle mode using Timer3
        OC5CON1 = OC_TIMER3_SRC    //Timer3 source
                    | OC_PWM_CENTER_ALIGN;  //PWM
        OC5CON2 = OC_SYNCSEL_TIMER3;   //synchronize to timer3
    #else
        //older families, this PWM mode is compatible with center-aligned, OC1R=0
        //as writes to OC1RS sets the pulse widith.
        OC5CON = OC_TIMER3_SRC     //Timer3 source
               | OC_PWM_FAULT_PIN_DISABLE;  //PWM, no fault detection
    #endif
}

void _ISR _T2Interrupt(void) {
    _T2IF = 0;    //clear the timer interrupt bit
}

void _ISR _T3Interrupt(void) {
      _T3IF = 0;    //clear the timer interrupt bit
}

// Interrupt Service Routine for INT0 for right encoder
void _ISRFAST _INT0Interrupt (void) {
    _INT0IF = 0;    //clear the interrupt bit
    process_right_rotary_data();
}

// Interrupt Service Routine for INT1 for left encoder
void _ISRFAST _INT1Interrupt (void) {
    _INT1IF = 0;    //clear the interrupt bit
    process_left_rotary_data();
}

///////////////////////////////////////////////
//
// Motor primitives
//
///////////////////////////////////////////////

// Left motor primitive movements
void left_motor_reverse (float f_duty) {
    LIN1_PULSE = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T2CONbits));
    LIN2_PULSE = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T2CONbits)) * (1-f_duty);

    u8_leftMotorDirection = BACKWARD_MOVEMENT;
}

void left_motor_fwd (float f_duty) {
    LIN1_PULSE = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T2CONbits)) * (1-f_duty);
    LIN2_PULSE = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T2CONbits));

    u8_leftMotorDirection = FORWARD_MOVEMENT;
}

void left_motor_stop() {
    LIN1_PULSE = 0;
    LIN2_PULSE = 0;

    u8_leftMotorDirection = STOPPED;
}

// Right motor primitive movements
void right_motor_reverse (float f_duty) {
    RIN1_PULSE = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T3CONbits));
    RIN2_PULSE = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T3CONbits)) * (1-f_duty);

    u8_rightMotorDirection = BACKWARD_MOVEMENT;
}

void right_motor_fwd (float f_duty) {
    RIN1_PULSE = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T3CONbits)) * (1-f_duty);
    RIN2_PULSE = usToU16Ticks(MOTOR_PWM_PERIOD, getTimerPrescale(T3CONbits));

    u8_rightMotorDirection = FORWARD_MOVEMENT;
}

void right_motor_stop() {
    RIN1_PULSE = 0;
    RIN2_PULSE = 0;

    u8_rightMotorDirection = STOPPED;
}

// Process the right encoder's data
void process_right_rotary_data() {
    // Increment if moving forward
    if (u8_rightMotorDirection == FORWARD_MOVEMENT) {
        i16_rightCounterROT++;
        if ((get_right_motor_location() >= f_rightTargetPosition) && (u8_rightAtTarget == 0)){
            u8_rightAtTarget = 1;
            right_motor_stop();
            printf("Right reached target\n");
        }
    }

    // Decrement if moving backwards
    if (u8_rightMotorDirection == BACKWARD_MOVEMENT) {
        i16_rightCounterROT--;
        if ((get_right_motor_location() <= f_rightTargetPosition) && (u8_rightAtTarget == 0)){
            u8_rightAtTarget = 1;
            right_motor_stop();
            printf("Right reached target\n");
        }
    }

    // Reset counter to 0 we've reached max and increment revolution count
    if (i16_rightCounterROT > ROT_MAX) {
        i16_rightCounterROT = 0;
        i16_rightRevolutionCount++;
    }

    // Reset counter to max we've reached 0 and decrement revolution count
    if (i16_rightCounterROT < 0 ) {
        i16_rightCounterROT = ROT_MAX;
        i16_rightRevolutionCount--;
    }
}

// Process the left encoder's data
void process_left_rotary_data() {
    // Increment if moving forward
    if (u8_leftMotorDirection == FORWARD_MOVEMENT) {
        i16_leftCounterROT++;

        if ((get_left_motor_location() >= f_leftTargetPosition) && (u8_leftAtTarget == 0)) {
            u8_leftAtTarget = 1;
            left_motor_stop();
            printf("Left reached target\n");
        }
    }

    // Decrement if moving backwards
    if (u8_leftMotorDirection == BACKWARD_MOVEMENT) {
        i16_leftCounterROT--;

        if ((get_left_motor_location() <= f_leftTargetPosition) && (u8_leftAtTarget == 0)) {
            u8_leftAtTarget = 1;
            left_motor_stop();
            printf("Left reached target\n");
        }
    }

    // Reset counter to 0 we've reached max and increment revolution count
    if (i16_leftCounterROT > ROT_MAX) {
        i16_leftCounterROT = 0;
        i16_leftRevolutionCount++;
    }

    // Reset counter to max we've reached 0 and decrement revolution count
    if (i16_leftCounterROT < 0 ) {
        i16_leftCounterROT = ROT_MAX;
        i16_leftRevolutionCount--;
    }
}

// Get the current location of the right motor
float get_right_motor_location() {
    float f_fractionPart;
    float f_total;

    f_fractionPart = (1.0 * i16_rightCounterROT)/(1.0 * ROT_MAX);

    // Located in the positive direction
    if (i16_rightRevolutionCount > 0) {
        f_total = i16_rightRevolutionCount + f_fractionPart;
    }
    // Located in the negative direction
    else if (i16_rightRevolutionCount < 0){
        f_total = i16_rightRevolutionCount - f_fractionPart;
    }
    // At zero so we have to look at our movement
    else {
        if (u8_rightMotorDirection == FORWARD_MOVEMENT) {
            f_total = 0 + f_fractionPart;
        }
        else {
            f_total = 0 - f_fractionPart;
        }
    }
    return f_total;
}

// Get the current location of the left motor
float get_left_motor_location() {
    float f_fractionPart;
    float f_total;

    f_fractionPart = (1.0 * i16_leftCounterROT)/(1.0 * ROT_MAX);

    // Located in the positive direction
    if (i16_leftRevolutionCount > 0) {
        f_total = i16_leftRevolutionCount + f_fractionPart;
    }
    // Located in the negative direction
    else if (i16_leftRevolutionCount < 0){
        f_total = i16_leftRevolutionCount - f_fractionPart;
    }
    // At zero so we have to look at our movement
    else {
        if (u8_leftMotorDirection == FORWARD_MOVEMENT) {
            f_total = 0 + f_fractionPart;
        }
        else {
            f_total = 0 - f_fractionPart;
        }
    }
    return f_total;
}
///////////////////////////////////////////////
//
// Motor usage
//
///////////////////////////////////////////////

// Stop robot
void motors_stop(void) {
    left_motor_stop();
    right_motor_stop();
}

// Turn robot right
void motors_turn_right(float f_duty) {
    right_motor_reverse(f_duty);
    left_motor_fwd(f_duty);
}

// Turn robot left
void motors_turn_left(float f_duty) {
    right_motor_fwd(f_duty);
    left_motor_reverse(f_duty);
}

// Move robot forward
void motors_move_forward(float f_duty) {
    right_motor_fwd(f_duty);
    left_motor_fwd(f_duty);
}

// Move robot in reverse
void motors_move_reverse(float f_duty) {
    right_motor_reverse(f_duty);
    left_motor_reverse(f_duty);
}

// Prepare robot for 90 degree turn by time
void prepare_for_90_degree_turn(float f_duty) {
    move_by_distance(PREPARE_90_TURN_DISTANCE, f_duty);
    DELAY_MS(PREPARE_TURN_TIME + 500);
    motors_stop();
}

// Turn robot 90 degrees by time
void turn_90_degrees(float f_duty, uint8_t u8_direction) {
    prepare_for_90_degree_turn(f_duty);
    if (u8_direction == RIGHT_DIRECTION) {
        move_right_motor_by_revolutions((-1.0 * DEGREE_90_TURN_REVS), f_duty);
        move_left_motor_by_revolutions(DEGREE_90_TURN_REVS, f_duty);
    }
    if (u8_direction == LEFT_DIRECTION) {
        move_right_motor_by_revolutions(DEGREE_90_TURN_REVS, f_duty);
        move_left_motor_by_revolutions((-1.0 * DEGREE_90_TURN_REVS), f_duty);
    }
    DELAY_MS(DEGREE_90_TURN_TIME + 500);
}

// Move right motor by revolutions
void move_right_motor_by_revolutions(float f_revolutions, float f_duty) {
    float f_currentPosition;

    f_currentPosition = get_right_motor_location();
    f_rightTargetPosition = f_currentPosition + f_revolutions;
    u8_rightAtTarget = 0;

    if (f_revolutions > 0) {
        right_motor_fwd(f_duty);
    }
    else if (f_revolutions < 0) {
        right_motor_reverse(f_duty);
    }
    #ifdef DEBUG_BUILD
    printf("Current right position: %f\n", (double) f_currentPosition);
    printf("New right target: %f\n", (double) f_rightTargetPosition);
    #endif
}

// Move left motor by revolutions
void move_left_motor_by_revolutions(float f_revolutions, float f_duty) {
    float f_currentPosition;

    f_currentPosition = get_left_motor_location();
    f_leftTargetPosition = f_currentPosition + f_revolutions;
    u8_leftAtTarget = 0;

    if (f_revolutions > 0) {
        left_motor_fwd(f_duty);
    }
    else if (f_revolutions < 0) {
        left_motor_reverse(f_duty);
    }
    #ifdef DEBUG_BUILD
    printf("Current left position: %f\n", (double) f_currentPosition);
    printf("New left target: %f\n", (double) f_leftTargetPosition);
    #endif
}

// Move right motor by mm
void move_right_motor_by_distance(float f_distance, float f_duty) {
    move_right_motor_by_revolutions(f_distance/WHEEL_CIRCUMFERENCE, f_duty);
}

// Move left motor by mm
void move_left_motor_by_distance(float f_distance, float f_duty) {
    move_left_motor_by_revolutions(f_distance/WHEEL_CIRCUMFERENCE, f_duty);
}

// Move entire robot by revolutions
void move_by_revolutions(float f_revolutions, float f_duty) {
    move_right_motor_by_revolutions(f_revolutions, f_duty);
    move_left_motor_by_revolutions(f_revolutions, f_duty);
}

// Move entire robot by mm
void move_by_distance(float f_distance, float f_duty) {
    move_right_motor_by_distance(f_distance, f_duty);
    move_left_motor_by_distance(f_distance, f_duty);
}

#ifdef DEBUG_BUILD
void set_prepare_time(uint16_t u16_newTime) {
    u16_prepareTurnDelayTime = u16_newTime;
}

void set_turn_time(uint16_t u16_newTime) {
    u16_90DegreeTurnTime = u16_newTime;
}
#endif
