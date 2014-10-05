#include "servosAPI.h"
#include "port_mapping.h"

#ifndef RUBIKS_CODE_H_
#define RUBIKS_CODE_H_

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
void twist_rubiks(void);

/**
 * @brief Lower platform, twist cube, and then raise platform
 */
void play_rubiks(void);

#endif