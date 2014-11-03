#include "lineFollowerAPI.h"
#include <stdio.h>
#include <stdint.h>

float getLine(uint16_t* pau16_sensorValues) {
  float f_line = 0;
  readLine(pau16_sensorValues, QTR_EMITTERS_ON);
  uint16_t u16_i;
  for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
    f_line += pau16_sensorValues[u16_i] * (u16_i+1);
  }
  if(f_line == 0.0) {
    return 0.0;
  }
  uint16_t u16_sum = 0;
  for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
    u16_sum += pau16_sensorValues[u16_i];
  }
  f_line = f_line/u16_sum;
  return f_line;
}

void followLine(uint16_t* pau16_sensorValues, float f_maxSpeed) {
  float f_lineToSpeedFactor = f_maxSpeed/ 4.5;
  float f_line = getLine(pau16_sensorValues);
  //if f_line = 0 relie on previous readings
  if(f_line == 0) {
    return;
  }
  float f_rightDuty = f_line*f_lineToSpeedFactor;
  float f_leftDuty = (9-f_line)*f_lineToSpeedFactor;

  if(f_rightDuty > f_maxSpeed) {
    f_rightDuty = f_maxSpeed;
  }
  if(f_leftDuty > f_maxSpeed) {
    f_leftDuty = f_maxSpeed;
  }
  right_motor_fwd(f_rightDuty);
  left_motor_fwd(f_leftDuty);
  printf("line: %f left: %f right: %f \n",f_line, f_leftDuty, f_rightDuty);
 }

void followLineBack(uint16_t* pau16_sensorValues, float f_maxSpeed) {
  float f_lineToSpeedFactor = f_maxSpeed/ 4.5;

  float f_line = getLine(pau16_sensorValues);
  //if f_line = 0 relie on previous readings
  if(f_line == 0) {
    return;
  }
  float f_rightDuty = f_line*f_lineToSpeedFactor;
  float f_leftDuty = (9-f_line)*f_lineToSpeedFactor;
  if(f_rightDuty > f_maxSpeed) {
    f_rightDuty = f_maxSpeed;
  }
  if(f_leftDuty > f_maxSpeed) {
    f_leftDuty = f_maxSpeed;
  }
  left_motor_reverse(f_rightDuty);
  right_motor_reverse(f_leftDuty);
 }

// Print line follower data
 void printLineData(uint16_t* pau16_sensorValues) { 
  int i = 0;
  for (i = 0 ; i < SENSOR_NUM ; ++i)
  {
      printf("%d-", pau16_sensorValues[i]);
  }
}

