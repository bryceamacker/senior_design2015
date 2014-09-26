#include "pic24_all.h"
#include "port_mapping.h"

#ifndef SERVOS_API_H_
#define SERVOS_API_H_

#define NUM_SERVOS 4

#define CONT_COUNTER_FULL_ROTATION_TIME  905    // Full counter rotation time for a continuous servo in seconds
#define CONT_CLOCK_FULL_ROTATION_TIME    915    // Full clock rotation time for a continuous servo in seconds

#define MIN_PW      600     // Minimum pulse width, in us
#define MAX_PW      2400    // Maximum pulse width, in us
#define SLOT_WIDTH  2800    // Slot width, in us
#define PWM_PERIOD  20000   // Full pulse width in ms

typedef enum {
    ETCH_VERTICAL =     0,
    ETCH_HORIZ =        1,
    RUBIKS_PLATFORM =   2,
    RUBIKS_TWIST =      3,
    SIMON_YELLOW =      4,
    SIMON_BLUE =        5,
    SIMON_RED =         6,
    SIMON_GREEN =       7,
    ARM_EXTEND =        8,
    ARM_PIVOT =         9
} servoIDs;

typedef enum {
    RED_BUTTON =    0,
    GREEN_BUTTON =  1,
    YELLOW_BUTTON = 2,
    BLUE_BUTTON =   3
} buttonIDs;

/////////////////////////////////////////////// 
//
// Servo config
//
///////////////////////////////////////////////
/**
 * @brief Configs timer 3 to drive servos
 */
void config_servo_timer3(void);

/**
 * @brief Initializes every thing for all the servos in the servoID enum
 */
void servo_init(void);

/**
 * @brief Configure the output comparator for servo usage
 * @details Uses OC1
 */
void config_output_capture1(void);

/////////////////////////////////////////////// 
//
// Servo primitives
//
///////////////////////////////////////////////
/**
 * @brief Provides the given servo a pulse width in order to turn it
 * 
 * @param id the servoID of the servo to turn
 * @param pulseWidth The pulse width to send the servo
 */
void turn_servo(servoIDs id, int pulseWidth);

/**
 * @brief Set a new pusle width to a servo, called by the interrupt ISR
 * 
 * @param u8_servo servo number to change
 * @param u8_val new value to assign
 */
void set_servo_output (uint8_t u8_servo, uint8_t u8_val);

/////////////////////////////////////////////// 
//
// Servo usage
//
///////////////////////////////////////////////
/**
 * @brief Stops a servo from turning
 * 
 * @param id the servoID of the servo to stop
 */
void stop_servo (servoIDs id);

/**
 * @brief Turn a servo clockwise by a certain degree amount
 * @details This turns the servo counter clockwise when looking at 
 * the back of the servo
 *
 * @param id the servoID of the servo to turn
 * @param degrees number of degrees to turn the servo
 */
void turn_servo_clockwise_degrees(servoIDs id, float degrees);

/**
 * @brief Turn a servo counter-clockwise by a certain degree amount
 * @details This turns the servo counter clockwise when looking at 
 * the back of the servo
 * 
 * @param id the servoID of the servo to turn
 * @param degrees number of degrees to turn the servo
 */
void turn_servo_counterwise_degrees(servoIDs id, float degrees);

/**
 * @brief Turn a servo to a certain degree
 * 
 * @param id the servoID of the servo to turn
 * @param degrees number of degrees to turn the servo
 */
void turn_servo_to_degree(servoIDs id, float degree);

/**
 * @brief Set all servos to calibration mode
 * @details Sets all servos to pw of 1500, where the should not move at all
 */
void servo_calibration_mode(void);

#endif