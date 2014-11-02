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

int main (void) {
    configBasic(HELLO_MSG);
    pic_motor_controller_init();
    
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
