/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName:
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        10/01/2014              SECON 2015
*********************************************************************/

#include "picMotorController.h"
#include "lineFollowerAPI.h"

int main (void) {
    configBasic(HELLO_MSG);
    pic_motor_controller_init();

    motors_init();

    uint16_t pau16_sensorValues[SENSOR_NUM];
    uint16_t position = 0;
    uint8_t detectingSensors = 0;
    uint8_t foundObjective = 0;
    int16_t error = 0;

    // Find the center of the line we are constantly trying to stay at
    int lineCenter = ((1000 * (SENSOR_NUM - 1)) / 2);

    // Flags while in sharp turns
    uint8_t leftTurn = 0;
    uint8_t rightTurn = 0;

    // Calibrate a bit for better array results
    int i = 0;
    for (i = 0; i < 100; i++) {
      calibrate(QTR_EMITTERS_ON);
    }

    while(1) {
           // Get the average position of the line
           position = 1000 * getLine(pau16_sensorValues);
           error = position - lineCenter;
           detectingSensors = 0;

           // Sum up the array
           for (i = 0; i < SENSOR_NUM; i++)
           {
               detectingSensors += pau16_sensorValues[i];
           }
           // If enough sensors are detecting, what appears to be a wide line is most 
           // likely the edge of a box
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
        }

    while (1) {
        inStringEcho(sz_1,BUFSIZE);       //get a string from the console
        if (*sz_1 == 0) continue;         //don't send empty string
        writeNI2C1(PIC_MOTOR_CONTROLLER_ADDR, (uint8_t *)sz_1, getStringLength(sz_1));   //send the string
        // readNI2C1(PIC_MOTOR_CONTROLLER_ADDR, (uint8_t *) sz_2,4) ;  //read the reversed string
        doHeartbeat();
    }
}

int16_t getStringLength(char* psz_1) {
  uint16_t u16_len;
  u16_len = 0;
  while (*psz_1) {
    psz_1++;
    u16_len++;
  }
  u16_len++;
  return u16_len;
}
