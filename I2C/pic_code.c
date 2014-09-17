/*
 * "Copyright (c) 2008 Robert B. Reese, Bryan A. Jones, J. W. Bruce ("AUTHORS")"
 * All rights reserved.
 * (R. Reese, reese_AT_ece.msstate.edu, Mississippi State University)
 * (B. A. Jones, bjones_AT_ece.msstate.edu, Mississippi State University)
 * (J. W. Bruce, jwbruce_AT_ece.msstate.edu, Mississippi State University)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the authors appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE "AUTHORS" BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE "AUTHORS"
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE "AUTHORS" SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE "AUTHORS" HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Please maintain this header in its entirety when copying/modifying
 * these files.
 *
 *
 */
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
  while (1) doHeartbeat();
}