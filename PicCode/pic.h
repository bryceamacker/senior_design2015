#include "pic_config.h"

#ifndef PIC_H_
#define PIC_H_

typedef enum {
    PLAY_SIMON      = 0,
    PLAY_ETCH       = 1,
    PLAY_RUBIK      = 2,
    PLAY_CARDS      = 3
} picTwoCommands_t;

typedef enum  {
  STATE_WAIT_FOR_ADDR,
  STATE_WAIT_FOR_WRITE_DATA,
  STATE_SEND_READ_DATA
} I2C_STATE;

picTwoCommands_t        u8_command;
I2C_STATE               currentState;

void serial_menu(void);

void servo_menu(void);

void set_servo(char u8_servo, uint16_t u16_pwm);

#endif