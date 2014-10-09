#include "servosAPI.h"
#include "port_mapping.h"

#ifndef RUBIKS_CODE_H_
#define RUBIKS_CODE_H_

#define PLATFORM_UP_PULSE       2050
#define PLATFORM_DOWN_PULSE     1550
#define PLATFORM_BACK_PULSE     1000
#define TWIST_CLOCK_PULSE       950
#define TWIST_COUNTER_PULSE     2050
#define TWIST_DIAG_PULSE        1225
#define WAIT_TIME               1500

/**
 * @brief Intialize platform up and twist to untwisted
 */
void rubik_init(void);

/**
 * @brief Lower the platform with the cube turner up
 */
void platform_down(void);

/**
 * @brief Raise the platform with the cube turner up
 */
void platform_up(void);

/**
 * @brief Twist the rubiks cube
 */
void twist_rubiks_clock(void);

/**
 * @brief Twist the rubiks cube
 */
void twist_rubiks_counter(void);

/**
 * @brief Move twist and platform to position etch servos
 */
void etch_prepare(void);

/**
 * @brief Lower platform, twist cube, and then raise platform
 */
void play_rubiks(void);

#endif