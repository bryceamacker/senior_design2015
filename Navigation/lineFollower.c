#include "pic24_all.h"
#include "pic24_qtr8rc.h"
#include "motors.h"
#include <stdio.h>
#include <stdint.h>

#
//*****************************************************
//  BEGIN SETUP FOR SERVO CONTROL
//*****************************************************
//<editor-fold defaultstate="collapsed" desc="Servo Setup">
#define PWM_PERIOD 20000

void  configTimer2(void) {
  T2CON = T2_OFF | T2_IDLE_CON | T2_GATE_OFF
          | T2_32BIT_MODE_OFF
          | T2_SOURCE_INT
          | T2_PS_1_256 ;  //1 tick = 1.6 us at FCY=40 MHz
  PR2 = usToU16Ticks(PWM_PERIOD, getTimerPrescale(T2CONbits)) - 1;
  TMR2  = 0;       //clear timer2 value
}

//just pick four digital outputs
#define NUM_SERVOS 4
#define SERVO0  _LATC1
#define SERVO1  _LATC2
#define SERVO2  _LATG6
#define SERVO3  _LATG7

#define MIN_PW  600            //minimum pulse width, in us
#define MAX_PW 2400            //minimum pulse width, in us
#define SLOT_WIDTH 2800        //slot width, in us

volatile uint16_t au16_servoPWidths[NUM_SERVOS];
volatile uint8_t u8_currentServo =0;
volatile uint8_t u8_servoEdge = 1;  //1 = RISING, 0 = FALLING
volatile uint16_t u16_slotWidthTicks = 0;

void initServos(void) {
  uint8_t u8_i;
  uint16_t u16_initPW;

  u8_currentServo = 0;
  CONFIG_RC1_AS_DIG_OUTPUT();
  CONFIG_RC2_AS_DIG_OUTPUT();
  CONFIG_RG6_AS_DIG_OUTPUT();
  CONFIG_RG7_AS_DIG_OUTPUT();
  u16_initPW = usToU16Ticks(MIN_PW + (MAX_PW-MIN_PW)/2, getTimerPrescale(T2CONbits));

  //config all servos for half maximum pulse width
  for (u8_i=0; u8_i<NUM_SERVOS; u8_i++) au16_servoPWidths[u8_i]=u16_initPW;
  SERVO0 = 0; //all servo outputs low initially
  SERVO1 = 0;
  SERVO2 = 0;
  SERVO3 = 0;  //outputs initially low
  u16_slotWidthTicks = usToU16Ticks(SLOT_WIDTH, getTimerPrescale(T2CONbits));
}


/**
* Select servo and value to write to it
* @param u8_servo  Select the servo port to write to
* @param u8_val    Value to send to the servo
*/
void setServoOutput (uint8_t u8_servo, uint8_t u8_val) {
  switch (u8_servo) {
    case 0:
      SERVO0 = u8_val;
      break;
    case 1:
      SERVO1 = u8_val;
      break;
    case 2:
      SERVO2 = u8_val;
      break;
    case 3:
      SERVO3 = u8_val;
      break;
    default:
      break;
  }
}

void _ISR _OC1Interrupt(void) {
  _OC1IF = 0;
  //change the servo's value
  setServoOutput(u8_currentServo, u8_servoEdge);
//schedule next interrupt
  if (u8_servoEdge == 1) {  //rising edge
    //next interrupt occurs after pulse width has elapsed
    OC1R = OC1R + au16_servoPWidths[u8_currentServo];
    u8_servoEdge = 0;     //change to falling edge
  } else { //falling edge
    //next interrupt occurs at beginning of next slot
    if (u8_currentServo != NUM_SERVOS -1)
      OC1R = u16_slotWidthTicks*(u8_currentServo+1);
    else //last servo!
      OC1R = 0;
    u8_servoEdge = 1;     //change to rising edge
    u8_currentServo++;
    if (u8_currentServo == NUM_SERVOS) u8_currentServo = 0;
  }
}


void configOutputCapture1(void) {
  T2CONbits.TON = 0;       //disable Timer when configuring Output compare
  OC1R  =  0;  //initialize to 0
//turn on the compare toggle mode using Timer2
#ifdef OC1CON1
  OC1CON1 = OC_TIMER2_SRC |      //Timer2 source
            OC_TOGGLE_PULSE;     //single compare toggle, just care about compare event
  OC1CON2 = OC_SYNCSEL_TIMER2;   //synchronize to timer2
#else
  OC1CON = OC_TIMER2_SRC |      //Timer2 source
           OC_TOGGLE_PULSE;     //single compare toggle, just care about compare event
#endif
  _OC1IF = 0;
  _OC1IP = 1;
  _OC1IE = 1;    //enable the OC1 interrupt
}


char sz_buf[32];

/**
 * Set servo width from console window
 */
void getServoValue(void) {
  int16_t u16_servo;
  int16_t u16_pw;
  printf("Choose servo (1,2,3,4): ");
  inStringEcho(sz_buf,31);
  sscanf(sz_buf,"%d",(int *) &u16_servo);
  if ((u16_servo > 4) || (u16_servo < 1)) {
    printf("Invalid servo..\n");
    return;
  }
  printf("Enter pulse width (min 600, max 2400): ");
  inStringEcho(sz_buf,31);
  sscanf(sz_buf,"%d",(int *) &u16_pw);
  if ((u16_pw > 2400) || (u16_pw < 600)) {
    printf("Invalid pulse width..\n");
    return;
  }
  //set the pulse width
  _OC1IE = 0; //disable the interrupt while changing
  au16_servoPWidths[u16_servo-1]=usToU16Ticks(u16_pw, getTimerPrescale(T2CONbits));
  _OC1IE = 1;
}

//</editor-fold>
// *****************************************************
// END SERVO SETUP
// *****************************************************


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
	//printf("left: %f right: %f ",f_leftDuty, f_rightDuty);
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
        configBasic(HELLO_MSG);
        configHeartbeat();
        
        // Servo init
        configTimer2();
        initServos();
        configOutputCapture1();
        T2CONbits.TON = 1;

        // Line follower init
        kalibrate(QTR_EMITTERS_ON);
        uint16_t pau16_sensorValues[SENSOR_NUM];
        char* serial_buf[SENSOR_NUM];
        uint8_t i;
        float temp_line;

        while(1) {
            // <editor-fold defaultstate="collapsed" desc="Print line follower data">
//            temp_line = getLine(pau16_sensorValues);
//            for (i = 0 ; i < SENSOR_NUM ; ++i)
//            {
//                serial_buf[i] = pau16_sensorValues[i];
//                printf("%d-", pau16_sensorValues[i]);
//            }
//            printf("%f",temp_line);
//            printf("\n");
            // </editor-fold>

            // <editor-fold defaultstate="collapsed" desc="Control servos from serial">
            // Note, you must use the "Send&\n" option if using BullyBootloader
            getServoValue();
            // </editor-fold>

        }
}
