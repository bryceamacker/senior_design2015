#include "picOneConfig.h"

#define PIC_ONE_I2C_ADDR 0x18

void pic_one_init() {
    // Basic init
    configClock();
    configDefaultUART(19200);
    printResetCause();
    outString(HELLO_MSG);
    configHeartbeat();

    // Motors init
    motors_init();

    // Calibrate line sensor
    calibrate(QTR_EMITTERS_ON);

    // I2C init
    I2C1ADD = PIC_ONE_I2C_ADDR;
    _SI2C1IF = 0;
    _SI2C1IP = 1;
    _SI2C1IE = 1;
}



