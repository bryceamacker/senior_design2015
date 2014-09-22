#include "picTwoConfig.h"

#define PIC_ONE_I2C_ADDR 0x20

void pic_two_init() {
    // Basic init
    configClock();
    configDefaultUART(19200);
    printResetCause();
    outString(HELLO_MSG);
    configHeartbeat();

    // I2C init
    I2C1ADD = PIC_ONE_I2C_ADDR;
    _SI2C1IF = 0;
    _SI2C1IP = 1;
    _SI2C1IE = 1;
}

