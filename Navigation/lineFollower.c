#include "pic24_all.h"
#include "sensorArrayAPI.h"
#include "motorsAPI.h"
#include <stdio.h>
#include <stdint.h>



typedef enum  {
  STATE_RESET = 0,
  STATE_INIT,
  STATE_FOLLOW_LINE_UNTIL_INTERSECTION,
  STATE_TURN_LEFT,
  STATE_FOLLOW_LINE_UNTIL_BLOCK,
  STATE_TURN_180,
  STATE_FOLLOW_LINE_UNTIL_INTERSECTION2,
  STATE_TURN_LEFT2,
  STATE_END
} STATE;

volatile STATE e_mystate = STATE_RESET;
volatile uint8_t u8_timerExpired = 0;

//Interrupt Service Routine for Timer5
void _ISRFAST _T5Interrupt (void) {
  T5CONbits.TON = 0;
  TMR5  = 0;
  u8_timerExpired = 1;
  _T5IF = 0;                 //clear the timer interrupt bit
}

#define ISR_PERIOD     180                // in ms
void  configTimer5(void) {
  //T5CON set like this for documentation purposes.
  //could be replaced by T5CON = 0x0020
  T5CON = T5_OFF |T5_IDLE_CON | T5_GATE_OFF
          | T5_SOURCE_INT
          | T5_PS_1_256;
  //PR5 = msToU16Ticks (ISR_PERIOD, getTimerPrescale(T5CONbits)) - 1;
  TMR5  = 0;                       //clear timer5 value
  _T5IF = 0;                       //clear interrupt flag
  _T5IP = 1;                       //choose a priority
  _T5IE = 1;                       //enable the interrupt
  T5CONbits.TON = 0;               //turn off the timer
}

void _ISRFAST _T7Interrupt (void) {
  T6CONbits.TON = 0;
  TMR7HLD = 0;                     //write MSW first
  TMR6  = 0;                       //then LSW
  u8_timerExpired = 1;
  _T7IF = 0;                 //clear the timer interrupt bit
}

//Timer6/7 used as single 32-bit timer, control word of Timer6 controls timer,
//interrupt status of Timer7 used for the combined timer
void  configTimer67(void) {
  T6CON = T6_OFF | T6_IDLE_CON | T6_GATE_OFF
          | T6_32BIT_MODE_ON
          | T6_SOURCE_INT
          | T6_PS_1_1 ;
  PR6 = 0x3B00;                    //maximum period
  PR7 = 0x0858;                    //maximum period
  TMR7HLD = 0;                     //write MSW first
  TMR6  = 0;                       //then LSW
  _T7IF = 0;                       //clear interrupt flag
  _T7IP = 1;                       //choose a priority
  _T7IE = 1;                       //enable the interrupt
  T6CONbits.TON = 0;               //turn off the timer
}

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

//uint16_t getLine(uint16_t* pau16_sensorValues) {
//  float f_line = 0;
//  readLine(pau16_sensorValues, QTR_EMITTERS_ON);
//  uint16_t u16_i;
//  for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
//    f_line += pau16_sensorValues[u16_i] * (u16_i+1);
//  }
//  if(f_line == 0.0) {
//    return 0.0;
//  }
//  uint16_t u16_sum = 0;
//  for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
//    u16_sum += pau16_sensorValues[u16_i];
//  }
//  f_line = (f_line/u16_sum) * 1000;
//  return f_line;
//}

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
  //printf("left: %f right: %f ",f_leftDuty, f_rightDuty);
 }

uint8_t intersectionCheck(uint16_t* pau16_sensorValues) {
  if(pau16_sensorValues[0] == 1) {
    if(pau16_sensorValues[4] == 1 || pau16_sensorValues[5] == 1 || pau16_sensorValues[6] == 1 || pau16_sensorValues[7] == 1) {
      return 1;
    }
  }
  else if(pau16_sensorValues[7] == 1) {
    if(pau16_sensorValues[0] == 1 || pau16_sensorValues[1] == 1 || pau16_sensorValues[2] == 1 || pau16_sensorValues[3] == 1) {
      return 1;
    }
  }
  else {
    return 0;
  }
}

uint8_t blockCheck(uint16_t* pau16_sensorValues) {
  if(pau16_sensorValues[0] == 1) {
    if(pau16_sensorValues[3] == 1 || pau16_sensorValues[4] == 1 || pau16_sensorValues[5] == 1 || pau16_sensorValues[6] == 1 || pau16_sensorValues[7] == 1) {
      return 1;
    }
  }
  else if(pau16_sensorValues[7] == 1) {
    if(pau16_sensorValues[0] == 1 || pau16_sensorValues[1] == 1 || pau16_sensorValues[2] == 1 || pau16_sensorValues[3] == 1 || pau16_sensorValues[4] == 1) {
      return 1;
    }
  }
  else {
    return 0;
  }
}


//read a line from UART2 from RPI
//the RPI likes to stick a garbage character
//with is MSB set as the first character.
//filter these out
void getLine2(char *buf){
    int i;
    char c;

    i = 0;
    c=0;
    do {
        c = inChar2();
        if ((c & 0x80) == 0) {
          if(c == '\n') buf[i++]=0;
          else buf[i++]=c;
        }

    }while(c != '\n');

}
void outString2(const char* psz_s) {
  while (*psz_s) {
    outChar2(*psz_s);
    psz_s++;
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
        printf("\n Made it past motors_init()... \n");

        T2CONbits.TON = 1;

        // Line follower init
        int i = 0;
        for (i = 0; i < 100; i++) {
          calibrate(QTR_EMITTERS_ON);
        }

        uint16_t pau16_sensorValues[SENSOR_NUM];
        char* serial_buf[SENSOR_NUM];
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


           // Print line follower data
           // for (i = 0 ; i < SENSOR_NUM ; ++i)
           // {
           //     serial_buf[i] = pau16_sensorValues[i];
           //     printf("%d-", pau16_sensorValues[i]);
           // }


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
