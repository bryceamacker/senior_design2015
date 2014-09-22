#include "pic24_all.h"

#ifndef SENSOR_ARRAY_API_H_
#define SENSOR_ARRAY_API_H_

#define SENSOR_NUM              8
#define QTR_EMITTERS_OFF        0
#define QTR_EMITTERS_ON         1
#define QTR_EMITTERS_ON_AND_OFF 2

// Sensor configuration
void config_outputs(void);
void config_inputs(void);
void config_timer4(void);
void emitters_off(void);
void emitters_on(void);

// Sensor primitives
void calibrate(char u8_readMode);
void read(uint16_t* pau16_sensorValues, char u8_readMode);
void read_values(uint16_t* pau16_sensorValues);

// Sensor usage
void read_line(uint16_t* pau16_sensorValues, char u8_readMode);

#endif