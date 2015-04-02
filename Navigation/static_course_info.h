/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: static_course_info.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Static information about courses
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        2/24/2014               SECON 2015
*********************************************************************/

#include "pic24_all.h"

#ifdef DEBUG_BUILD
#include <stdio.h>
#endif

#ifndef _STATIC_COURSE_INFO_H
#define _STATIC_COURSE_INFO_H

extern queue_t navigationRoutineQueue;
extern queue_t navigationMoveDistanceQueue;

static inline void prepare_course_routines_one(void) {
    // Game 1
    enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
    enqueue(&navigationMoveDistanceQueue, 840);
    enqueue(&navigationRoutineQueue, LEFT_TURN);
    enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
    enqueue(&navigationMoveDistanceQueue, 150);
    enqueue(&navigationRoutineQueue, PLAY_GAME_PAUSE);

    // Game 2
    enqueue(&navigationRoutineQueue, MOVE_REVERSE_DISTANCE);
    enqueue(&navigationMoveDistanceQueue, 150);
    enqueue(&navigationRoutineQueue, RIGHT_TURN);
    enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
    enqueue(&navigationMoveDistanceQueue, 320);
    enqueue(&navigationRoutineQueue, RIGHT_TURN);
    enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
    enqueue(&navigationMoveDistanceQueue, 80);
    enqueue(&navigationRoutineQueue, PLAY_GAME_PAUSE);

    // Game 3
    enqueue(&navigationRoutineQueue, MOVE_REVERSE_DISTANCE);
    enqueue(&navigationMoveDistanceQueue, 80);
    enqueue(&navigationRoutineQueue, LEFT_TURN);
    enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
    enqueue(&navigationMoveDistanceQueue, 300);
    enqueue(&navigationRoutineQueue, LEFT_TURN);
    enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
    enqueue(&navigationMoveDistanceQueue, 150);
    enqueue(&navigationRoutineQueue, PLAY_GAME_PAUSE);

    // Game 4
    enqueue(&navigationRoutineQueue, MOVE_REVERSE_DISTANCE);
    enqueue(&navigationMoveDistanceQueue, 150);
    enqueue(&navigationRoutineQueue, RIGHT_TURN);
    enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
    enqueue(&navigationMoveDistanceQueue, 160);
    enqueue(&navigationRoutineQueue, RIGHT_TURN);
    enqueue(&navigationRoutineQueue, PLAY_GAME_PAUSE);

    // Finish Line
    enqueue(&navigationRoutineQueue, LEFT_TURN);
    enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
    enqueue(&navigationMoveDistanceQueue, 470);
    enqueue(&navigationRoutineQueue, RIGHT_TURN);
    enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
    enqueue(&navigationMoveDistanceQueue, 230);
}


static inline uint8_t prepare_course_routines(uint8_t u8_course) {
    uint8_t u8_setCourse;

    u8_setCourse = 1;

    switch(u8_course) {
        case 1:
            prepare_course_routines_one();
            break;
        default:
            u8_setCourse = 0;
            break;
    }

    return u8_setCourse;
}


/**
                    x
                    |
              ______|
       x     |   |
       |     |   x
       |_____|
             |
             |
             |----x
             |
             |
         x---|
             |
             |
         ____
        |
        |___
            |
            x
**/
static inline void prepare_static_course_turn_info_one(uint8_t* pau8_turnList) {
    uint8_t u8_turnCount = 0;

    // To game 1
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;

    // Leaving game 1
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;

    // To game 2
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;

    // Leaving game 2
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;

    // To game 3
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;
    pau8_turnList[u8_turnCount++] = CURVE_RIGHT;

    // Leaving game 3
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;

    // To game 4
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;

    // Leaving game 4
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;

    // To finish line
    pau8_turnList[u8_turnCount++] = CURVE_LEFT;
}

/**
                x
                |
                |---x
                |
        ________|
        |       |
        x       |
                |
                |---x
                |
                |
                |_____
           x          |
           |__________|
                |
                x
**/
static inline void prepare_static_course_turn_info_two(uint8_t* pau8_turnList) {
    uint8_t u8_turnCount = 0;

    // To game 1
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;

    // Leaving game 1

    // To game 2
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;

    // Leaving game 2
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;

    // To game 3
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;
    pau8_turnList[u8_turnCount++] = CURVE_LEFT;

    // Leaving game 3
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;

    // To game 4
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;

    // Leaving game 4
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;

    // To finish line
}


/**
                 ____x
        x       |
        |       |
        |_______|     x
                |     |
                |_____|
                |
                |
                |---x
                |
           x    |
           |____|
                |
                x
**/
static inline void prepare_static_course_turn_info_three(uint8_t* pau8_turnList) {
    uint8_t u8_turnCount = 0;

    // To game 1
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;

    // Leaving game 1
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;

    // To game 2
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;

    // Leaving game 2
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;

    // To game 3
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;
    pau8_turnList[u8_turnCount++] = CURVE_LEFT;

    // Leaving game 3
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;

    // To game 4
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;
    pau8_turnList[u8_turnCount++] = CURVE_RIGHT;

    // Leaving game 4
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;

    // To finish line
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;
}

/**
                x
                |
         _______|
        |       |
        |       |
        x       |
                |----x
                |
                |
                |____
                |    |
                |    |
           x----|    x
                |
                |
                |
                |
                x
**/
static inline void prepare_static_course_turn_info_four(uint8_t* pau8_turnList) {
    uint8_t u8_turnCount = 0;

    // To game 1
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;

    // Leaving game 1
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;

    // To game 2
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;

    // Leaving game 2
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;

    // To game 3
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;

    // Leaving game 3
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;

    // To game 4
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;
    pau8_turnList[u8_turnCount++] = CURVE_LEFT;

    // Leaving game 4
    pau8_turnList[u8_turnCount++] = NORMAL_RIGHT;
    pau8_turnList[u8_turnCount++] = NORMAL_LEFT;

    // To finish line
}

static inline uint8_t prepare_static_course_turn_info(uint8_t u8_course, uint8_t* pau8_turnList) {
    uint8_t u8_setCourseTurns;

    u8_setCourseTurns = 1;

    switch (u8_course) {
        case 1:
            prepare_static_course_turn_info_one(pau8_turnList);
            break;
        case 2:
            prepare_static_course_turn_info_two(pau8_turnList);
            break;
        case 3:
            prepare_static_course_turn_info_three(pau8_turnList);
            break;
        case 4:
            prepare_static_course_turn_info_four(pau8_turnList);
            break;
        default:
            u8_setCourseTurns = 0;
            break;
    }
    return u8_setCourseTurns;
}

#endif
