#include "rubiks_code.h"

void rubik_init() {
    platform_up();
    twist_rubiks_counter();
}

void platform_down() {
    turn_servo_by_pulse(RUBIKS_PLATFORM, PLATFORM_DOWN_PULSE);
}

void platform_up() {
    turn_servo_by_pulse(RUBIKS_PLATFORM, PLATFORM_UP_PULSE);
}

void etch_prepare() {
    turn_servo_by_pulse(RUBIKS_TWIST,TWIST_DIAG_PULSE); 
    turn_servo_by_pulse(RUBIKS_PLATFORM, PLATFORM_BACK_PULSE);
}

void twist_rubiks_clock() {
    turn_servo_by_pulse(RUBIKS_TWIST, TWIST_CLOCK_PULSE);
}

void twist_rubiks_counter() {
    turn_servo_by_pulse(RUBIKS_TWIST, TWIST_COUNTER_PULSE);
}

void play_rubiks() {
    platform_down();
    DELAY_MS(WAIT_TIME);
    twist_rubiks_clock();
    DELAY_MS(WAIT_TIME);
    platform_up();
    DELAY_MS(WAIT_TIME);
    twist_rubiks_counter();
}
