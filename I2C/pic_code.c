/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: pic_code.c
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Testing code for I2C code
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        9/17/2014               SECON 2015
*********************************************************************/

#include "pic24_all.h"

#define SLAVE_I2C_ADDR 0x18

#define CONFIG_LED1() CONFIG_RC1_AS_DIG_OUTPUT()
#define LED1 (_LATC1)

#define CONFIG_LED2() CONFIG_RC2_AS_DIG_OUTPUT()
#define LED2 (_LATC2)

typedef enum  {
  STATE_WAIT_FOR_ADDR,
  STATE_WAIT_FOR_WRITE_DATA,
  STATE_SEND_READ_DATA
} STATE;

volatile STATE e_mystate = STATE_WAIT_FOR_ADDR;
volatile uint8_t u8_num;
volatile uint8_t u8_flash;

void _ISRFAST _SI2C1Interrupt(void) {
  // Variable to clear RBF bit
  uint8_t u8_c;
  _SI2C1IF = 0;
  
  // No longer controlled by master's clock
  I2C1CONbits.SCLREL = 0;
  
  switch (e_mystate) {
    case STATE_WAIT_FOR_ADDR:
      //clear RBF bit for address
      u8_c = I2C1RCV;
      
      // Check the R/W bit and see if read or write transaction
      if (I2C1STATbits.R_W) {
        // Send number
        I2C1TRN = u8_num;
        
        // Release clock line so MASTER can drive it
        I2C1CONbits.SCLREL = 1;

        // Change to send data
        e_mystate = STATE_SEND_READ_DATA;
      } 
      else {
          e_mystate = STATE_WAIT_FOR_WRITE_DATA;
      }
      break;

    case STATE_WAIT_FOR_WRITE_DATA:
      // Read the byte
      u8_num = I2C1RCV;
      
      printf("Recieved number: %u\n", u8_num);

      // A few test commands
      if (u8_num == 1) {
          LED1 = !LED1;
      }
      if (u8_num == 2) {
          LED2 = !LED2;
      }
      if (u8_num == 3) {
          LED1 = 1;
          LED2 = 1;
      }
      if (u8_num == 4) {
          LED1 = 0;
          LED2 = 0;
      }
      if (u8_num == 5) {
          u8_flash = 1;
      }

      // Wait for next transaction
      e_mystate = STATE_WAIT_FOR_ADDR;
      break;
      
    case STATE_SEND_READ_DATA:
      // Send number
      I2C1TRN = u8_num;
      
      printf("Transmitted number: %u\n", u8_num);
      
      // Release clock line so MASTER can drive it
      I2C1CONbits.SCLREL = 1;

      // Wait for next transaction
      e_mystate = STATE_WAIT_FOR_ADDR;
      break;
      
    default:
      e_mystate = STATE_WAIT_FOR_ADDR;
  }
}

int main (void) {
  configBasic(HELLO_MSG);
  // Configure I2C for 400 KHz
  configI2C1(400);

  u8_flash = 0;

  // Config a couple LEDs
  CONFIG_LED1();
  LED1 = 1;
  CONFIG_LED2();
  LED2 = 1;

  // Initialize the address register
  I2C1ADD = SLAVE_I2C_ADDR;
  _SI2C1IF = 0;
  _SI2C1IP = 1;
  
  // Enable ISR
  _SI2C1IE = 1;

  // Heartbeat, let interrupts handle I2C
  while (1) {
      if (u8_flash == 1) {
          LED1 = 0;
          LED2 = 0;
          DELAY_MS(1000);
          LED1 = 1;
          LED2 = 0;
          DELAY_MS(1000);
          LED1 = 1;
          LED2 = 1;
          DELAY_MS(1000);
          LED1 = 0;
          LED2 = 1;
          DELAY_MS(1000);
          LED1 = 0;
          LED2 = 0;
          u8_num = 255;
          u8_flash = 0;
      }
      doHeartbeat();
  }
}