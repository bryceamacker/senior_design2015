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

#define MAXSIZE 20

#ifndef SECON_COMMON_H_
#define SECON_COMMON_H_

struct stack
{
    uint8_t stack[MAXSIZE];
    uint8_t top;
};
typedef struct stack stack_t;

static inline void push(stack_t stack, uint8_t u8_newVal) {
    stack.stack[stack.top] = u8_newVal;
    stack.top++;
}

static inline uint8_t pop(stack_t stack) {
    uint8_t u8_retVal;

    u8_retVal = stack.stack[stack.top];
    stack.stack[stack.top] = 0;
    stack.top--;

    return u8_retVal;
}

static inline uint8_t peek(stack_t stack) {
    uint8_t u8_retVal;

    u8_retVal = stack.stack[stack.top];
    return u8_retVal;
}

static inline void display(void) {

}

#endif
