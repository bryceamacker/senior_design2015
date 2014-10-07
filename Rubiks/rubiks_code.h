#include "servosAPI.h"
#include "port_mapping.h"

#ifndef RUBIKS_CODE_H_
#define RUBIKS_CODE_H_

#define PLATFORM_UP_PULSE       1900
#define PLATFORM_DOWN_PULSE     1350
#define TWIST_CLOCK_PULSE       950
#define TWIST_COUNTER_PULSE     2050
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
 * @brief Lower platform, twist cube, and then raise platform
 */
void play_rubiks(void);

#endif