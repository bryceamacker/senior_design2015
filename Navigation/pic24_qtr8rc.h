#include "pic24_all.h"
#ifndef _PIC24_QTR8RC_H_
#define _PIC24_QTR8RC_H_

#define SENSOR_NUM	15
//#define MAX_VALUE	0x01F4
#define QTR_EMITTERS_OFF 0
#define QTR_EMITTERS_ON 1
#define QTR_EMITTERS_ON_AND_OFF 2


void CONFIG_OUTPUTS(void);
void CONFIG_INPUTS(void);
void configTimer4(void);
void emittersOff(void);
void emittersOn(void);
void readValues(uint16_t* pau16_sensor_values);
void kalibrate(char u8_readMode);
void read(uint16_t* pau16_sensor_values, char u8_readMode);
void readLine(uint16_t* pau16_sensorValues, char u8_readMode);

#endif