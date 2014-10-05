#include "pic_config.h"

#define PIC_I2C_ADDR 0x20

void pic_init() {
    // Basic init
    configClock();
    configDefaultUART(DEFAULT_BAUDRATE);

    // Servo init
    servo_init();

    // I2C init
    I2C1ADD = PIC_I2C_ADDR;
    _SI2C1IF = 0;
    _SI2C1IP = 1;
    _SI2C1IE = 1;
}

