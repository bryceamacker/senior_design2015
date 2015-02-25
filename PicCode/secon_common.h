/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName:   secon_common.h
* Dependenies: See INCLUDES setion below
* Proessor: PIC33EP512GP806
* Compiler: gcc-pic33
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Port definitions for the navigation PIC
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        2/18/2015               SECON 2015
*********************************************************************/

#define MAX_SIZE 20

#ifdef DEBUG_BUILD
#include <stdio.h>
#endif

#ifndef SECON_COMMON_H_
#define SECON_COMMON_H_

#define FALSE (0)
#define TRUE (!FALSE)

///////////////////////////////////////////////
//
// Stack data structure
//
///////////////////////////////////////////////
struct stack
{
    uint8_t theStack[MAX_SIZE];
    uint8_t top;
};
typedef struct stack stack_t;

#ifdef DEBUG_BUILD
static inline void display_stack(stack_t stack) {
    uint8_t i;

    for (i=0;i<stack.top;i++) {
        printf("%u", stack.theStack[i]);

        if (i == stack.top - 1) {
            printf ("<--");
        }
        printf("\n");
    }
}
#endif

static inline void init_stack(stack_t * stack) {
    stack->top = 0;
}

static inline void push(stack_t * stack, uint8_t u8_newVal) {
    stack->theStack[stack->top] = u8_newVal;
    stack->top++;
}

static inline uint8_t pop(stack_t * stack) {
    uint8_t u8_retVal;

    stack->top--;
    u8_retVal = stack->theStack[stack->top];
    stack->theStack[stack->top] = 0;

    return u8_retVal;
}

static inline uint8_t getSize(stack_t stack) {
    return stack.top;
}

static inline uint8_t peek(stack_t stack) {
    uint8_t u8_retVal;

    u8_retVal = stack.theStack[stack.top-1];
    return u8_retVal;
}

///////////////////////////////////////////////
//
// Queue data structure
//
///////////////////////////////////////////////

struct queue
{
    uint16_t theQueue[MAX_SIZE];
    uint16_t head;
    uint16_t tail;
};
typedef struct queue queue_t;

static inline void init_queue(queue_t * queue) {
    queue->head = queue->tail = -1;
}

static inline void clear_queue(queue_t * queue) {
    queue->head = queue->tail = -1; // Reset indices to start over
}

static inline int queue_is_empty(queue_t queue) {
    return (queue.head == queue.tail);
}

static inline int queue_is_full(queue_t queue) {
    return ((queue.tail - MAX_SIZE) == queue.head);
}

static inline uint16_t enqueue(queue_t * queue, uint16_t u8_newVal) {
    // Check to see if the Queue is full
    if(queue_is_full(*queue)) {
        return FALSE;
    }

    // Increment tail index
    queue->tail++;
    // Add the item to the Queue
    queue->theQueue[queue->tail % MAX_SIZE] = u8_newVal;
    return TRUE;
}

static inline uint16_t dequeue(queue_t * queue) {
    uint16_t u8_val;

    // Check for empty Queue
    if(queue_is_empty(*queue)) {
        return '\0';  // Return null character if queue is empty
    } else {
        queue->head++;
        u8_val = queue->theQueue[queue->head % MAX_SIZE];
        return u8_val;
    }
}

#ifdef DEBUG_BUILD
static inline void display_queue(queue_t queue) {
    uint8_t i;

    if (queue_is_empty(queue)) {
        printf("EMPTY");
    } else {
        for (i = queue.head+1;i <= queue.tail;i++) {
            printf("%d->", queue.theQueue[i]);
        }
        printf("x");
    }
    printf("\n");
}
#endif

#endif
