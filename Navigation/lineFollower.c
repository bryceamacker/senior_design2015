#include "pic24_all.h"
#include "sensorArrayAPI.h"
#include "motorsAPI.h"
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


int main()
{
        // Basic init
        configClock();
        configDefaultUART(19200);
        printResetCause();
        outString(HELLO_MSG);
        configHeartbeat();
        motors_init();

        // Line follower init
        int i = 0;
        for (i = 0; i < 100; i++) {
          calibrate(QTR_EMITTERS_ON);
        }

        uint16_t pau16_sensorValues[SENSOR_NUM];
        uint16_t position = 0;

        int lineCenter = ((1000 * (SENSOR_NUM - 1)) / 2);

        uint8_t detectingSensors = 0;
        uint8_t foundObjective = 0;

        // Flags while in sharp turns
        uint8_t leftTurn = 0;
        uint8_t rightTurn = 0;

        int16_t error = 0;

        int countUp = 1;
        uint16_t temp_line;

        while(1) {
          // move_forward(.15);
           position = 1000 * getLine(pau16_sensorValues);
           error = position - lineCenter;
           detectingSensors = 0;

           // If we are at a 90 degree turn, stop regular line following
           // and try to turn extactly 90 degrees
           for (i = 0; i < SENSOR_NUM; i++)
           {
               detectingSensors += pau16_sensorValues[i];
           }
           if (detectingSensors >= SENSOR_NUM - 2)
           {
               foundObjective = 1;
           }

           // Stop when we reach a box
           if (foundObjective == 1)
           {
               stop();
               printf("\t Reached Objective \n");
           }

           // else
           // {
           // if ((leftTurn == 1) || (rightTurn == 1))
           // {
           //     if (leftTurn ==1)
           //     {
           //         printf("\t 90 Degree Left Turn \n");
           //         DRIVE_LEFT_HARD();
           //         DELAY_MS(750);
           //         leftTurn = 0;
           //     }

           //     if (rightTurn == 1)
           //     {
           //         printf("\t 90 Degree Right Turn \n");
           //         DRIVE_RIGHT_HARD();
           //         DELAY_MS(750);
           //         rightTurn = 0;
           //     }
           // }
           // else
           // {
           //     // Try our best to detect 90 degree turns and set a flag
           //     if (pau16_sensorValues[14] == 1 && pau16_sensorValues[13] == 1
           //         && pau16_sensorValues[12] == 1 && pau16_sensorValues[11] == 1
           //         && pau16_sensorValues[10] == 1
           //        && (pau16_sensorValues[5] == 1 || pau16_sensorValues[6] == 1
           //         || pau16_sensorValues[7] == 1 || pau16_sensorValues[8] == 1
           //         || pau16_sensorValues[9] == 1))
           //     {
           //         leftTurn = 1;
           //     }

           //     else if (pau16_sensorValues[0] == 1 && pau16_sensorValues[1] == 1
           //          && pau16_sensorValues[2] == 1 && pau16_sensorValues[3] == 1
           //          && pau16_sensorValues[4] == 1
           //         && (pau16_sensorValues[5] == 1 || pau16_sensorValues[6] == 1
           //          || pau16_sensorValues[7] == 1 || pau16_sensorValues[8] == 1))
           //     {
           //         rightTurn = 1;
           //     }

           //     // Keep us going straight down the middle of the line
           //     else
           //     {
           //         leftTurn = 0;
           //         rightTurn = 0;
           else { 
                   if (error > 1000)
                   {
                       turn_left(.15);
                       printf("\t Drive Left \n");
                   }
                   if (error < -1000)
                   {
                       turn_right(.15);
                       printf("\t Drive Right \n");
                   }
                   if ((error >= -1000) && (error <= 1000)) // drive straight
                   {
                       move_forward(.15);
                       printf("\t Drive Forward \n");
                   }
                 }
           //     }
           // }
           // }
        }
}
