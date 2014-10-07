#include "etch_code.h"

/////////////////////////////////////////////// 
//
// Etch Characters/wrods
//
///////////////////////////////////////////////

void turn_servo_vertical_etch_distance(float distance) {
    uint16_t u16_degrees;
    distance = distance * VERTICAL_ADJUSTMENT;

    if (distance > 0) {
        // Turn clockwise to go right
        u16_degrees = (distance/FULL_ROTATION_VERT_DIST) * 360;
        turn_servo_clockwise(ETCH_VERTICAL, u16_degrees);
    } else {
        // Turn counter to go left
        u16_degrees = (distance/FULL_ROTATION_VERT_DIST) * -360;
        turn_servo_counterwise(ETCH_VERTICAL, u16_degrees);
    }
}

void turn_servo_horizontal_etch_distance(float distance) {
    uint16_t u16_degrees;
    distance = distance * HORIZ_ADJUSTMENT;

    if (distance > 0) {
        // Turn clockwise to go right
        u16_degrees = (distance/FULL_ROTATION_HORIZ_DIST) * 360;
        turn_servo_clockwise(ETCH_HORIZ, u16_degrees);
    } else {
        // Turn counter to go left
        u16_degrees = (distance/FULL_ROTATION_HORIZ_DIST) * -360;
        turn_servo_counterwise(ETCH_HORIZ, u16_degrees);
    }
}

/////////////////////////////////////////////// 
//
// Etch Characters/words
//
///////////////////////////////////////////////

void etch_init() {
    stop_servo(ETCH_VERTICAL);
    stop_servo(ETCH_HORIZ);
}

void draw_I_character() {
    // Top
    turn_servo_horizontal_etch_distance(3 * ETCH_UNIT);
    turn_servo_horizontal_etch_distance(-1.5 * ETCH_UNIT);

    // Length
    turn_servo_vertical_etch_distance(-4 * ETCH_UNIT);

    // Bottom
    turn_servo_horizontal_etch_distance(-1.5 * ETCH_UNIT);
    turn_servo_horizontal_etch_distance(3 * ETCH_UNIT);
}

void draw_E_character_from_bottom() {
    // Bottom
    turn_servo_horizontal_etch_distance(3 * ETCH_UNIT);
    turn_servo_horizontal_etch_distance(-3 * ETCH_UNIT);

    // Middle
    turn_servo_vertical_etch_distance(2 * ETCH_UNIT);
    turn_servo_horizontal_etch_distance(2 * ETCH_UNIT);
    turn_servo_horizontal_etch_distance(-2 * ETCH_UNIT);

    // Top
    turn_servo_vertical_etch_distance(2 * ETCH_UNIT);
    turn_servo_horizontal_etch_distance(3 * ETCH_UNIT);
}

void drawE_character_from_top() {
    // Top
    turn_servo_horizontal_etch_distance(3 * ETCH_UNIT); 
    turn_servo_horizontal_etch_distance(-3 * ETCH_UNIT);

    // Middle
    turn_servo_vertical_etch_distance(-2 * ETCH_UNIT);
    turn_servo_horizontal_etch_distance(2 * ETCH_UNIT);
    turn_servo_horizontal_etch_distance(-2 * ETCH_UNIT);

    // Bottom
    turn_servo_vertical_etch_distance(-2 * ETCH_UNIT);
    turn_servo_horizontal_etch_distance(3 * ETCH_UNIT);

}

void draw_IEEE() {
    draw_I_character();
    turn_servo_horizontal_etch_distance(2.5);

    draw_E_character_from_bottom();
    turn_servo_horizontal_etch_distance(2.5);

    drawE_character_from_top();
    turn_servo_horizontal_etch_distance(2.5);

    draw_E_character_from_bottom();    
}

void underline_to_reset() {
    // Get to bottom right
    turn_servo_horizontal_etch_distance(-3 * ETCH_UNIT);
    turn_servo_vertical_etch_distance(-4 * ETCH_UNIT);
    turn_servo_horizontal_etch_distance(3 * ETCH_UNIT);

    // Move down under the letters
    turn_servo_vertical_etch_distance(-1 * ETCH_UNIT);

    // Underline
    turn_servo_horizontal_etch_distance((-12 * ETCH_UNIT) - 20);

    // Move back up to starting point
    turn_servo_vertical_etch_distance(5 * ETCH_UNIT);
}