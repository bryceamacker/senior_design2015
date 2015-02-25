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
    enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
    enqueue(&navigationMoveDistanceQueue, 1000);
}

static inline void prepare_course_routines_two(void) {
    enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
    enqueue(&navigationMoveDistanceQueue, 1000);
}

static inline void prepare_course_routines_three(void) {
    enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
    enqueue(&navigationMoveDistanceQueue, 1000);
}

static inline void prepare_course_routines_four(void) {
    enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
    enqueue(&navigationMoveDistanceQueue, 1000);
}

static inline void prepare_course_routines_five(void) {
    enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
    enqueue(&navigationMoveDistanceQueue, 1000);
}

static inline void prepare_course_routines_six(void) {
    enqueue(&navigationRoutineQueue, MOVE_FORWARD_DISTANCE);
    enqueue(&navigationMoveDistanceQueue, 1000);
}

static inline uint8_t prepare_course_routines(uint8_t u8_course) {
    uint8_t u8_setCourse;

    u8_setCourse = 1;

    switch(u8_course) {
        case 1:
            prepare_course_routines_one();
            break;
        case 2:
            prepare_course_routines_two();
            break;
        case 3:
            prepare_course_routines_three();
            break;
        case 4:
            prepare_course_routines_four();
            break;
        case 5:
            prepare_course_routines_five();
            break;
        case 6:
            prepare_course_routines_six();
            break;
        default:
            u8_setCourse = 0;
            break;
    }

    return u8_setCourse;
}

#endif
