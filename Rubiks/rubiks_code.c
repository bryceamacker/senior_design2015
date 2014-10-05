#include "rubiks_code.h"

void rubik_init(void) {
    platform_up();
    turn_servo_to_degree(RUBIKS_TWIST, 0);
}

void platform_down(void) {
    turn_servo(RUBIKS_PLATFORM, 600);
//    turn_servo_to_degree(RUBIKS_PLATFORM, 0);
}

void platform_up(void) {
    turn_servo(RUBIKS_PLATFORM, 2400);
//    turn_servo_to_degree(RUBIKS_PLATFORM, 180);
}

void twist_rubiks(void) {
    turn_servo(RUBIKS_TWIST, 2400);
//    turn_servo_to_degree(RUBIKS_TWIST, 180);
}

void play_rubiks(void) {
    platform_down();
    DELAY_MS(500);
    twist_rubiks();
    DELAY_MS(500);
    platform_up();
    DELAY_MS(500);
}
