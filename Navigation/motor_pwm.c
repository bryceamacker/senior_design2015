#include <stdio.h>
#include <pic24_all.h>
#include <dataXfer.h>
#include <stdbool.h>

#define CONFIG_RIGHT_MOTOR_IN1()        CONFIG_RD1_AS_DIG_OUTPUT()
#define CONFIG_RIGHT_MOTOR_IN2()        CONFIG_RD2_AS_DIG_OUTPUT()
#define CONFIG_LEFT_MOTOR_IN1()         CONFIG_RD3_AS_DIG_OUTPUT()
#define CONFIG_LEFT_MOTOR_IN2()         CONFIG_RD4_AS_DIG_OUTPUT()

#define RIN1_PIN                       _LATD1
#define RIN2_PIN                       _LATD2
#define LIN1_PIN                       _LATD3
#define LIN2_PIN                       _LATD4

#define RIN1_PULSE                      OC2RS
#define RIN2_PULSE                      OC3RS
#define LIN1_PULSE                      OC4RS
#define LIN2_PULSE                      OC5RS


#define PULSEWIDTH1 0xFFFF

#ifndef PWM_PERIOD
#define PWM_PERIOD 20000  // desired period, in us
#endif

void  configTimer2(void) {
  T2CON = T2_OFF | T2_IDLE_CON | T2_GATE_OFF
          | T2_32BIT_MODE_OFF
          | T2_SOURCE_INT
          | T2_PS_1_64;
  PR2 = usToU16Ticks(PWM_PERIOD, getTimerPrescale(T2CONbits)) - 1;
  //PR2 = PULSEWIDTH1;
  TMR2  = 0;       //clear timer2 value
  _T2IF = 0;
  _T2IP = 1;
  _T2IE = 1;    //enable the Timer2 interrupt
}

void  configTimer3(void) {
  T3CON = T3_OFF | T3_IDLE_CON | T3_GATE_OFF
          | T2_32BIT_MODE_OFF
          | T3_SOURCE_INT
          | T3_PS_1_64;
  PR3 = usToU16Ticks(PWM_PERIOD, getTimerPrescale(T3CONbits)) - 1;
  //PR3 = PULSEWIDTH1;
  TMR3  = 0;       //clear timer3 value
  _T3IF = 0;
  _T3IP = 1;
  _T3IE = 1;    //enable the Timer3 interrupt
}


void configOutputCompare2(void) {
  T2CONbits.TON = 0;          //disable Timer when configuring Output compare
  //CONFIG_OC2_TO_RP(RB4_RP);   //map OC2 to RB4
  OC2CONbits.OCM2 = 0b110;
  OC2RS = 0;  //clear both registers
  OC2R = 0;
#ifdef OC2CON1
//turn on the compare toggle mode using Timer2
  OC2CON1 = OC_TIMER2_SRC |     //Timer2 source
            OC_PWM_CENTER_ALIGN;  //PWM
  OC2CON2 = OC_SYNCSEL_TIMER2;   //synchronize to timer2
#else
//older families, this PWM mode is compatible with center-aligned, OC2R=0
//as writes to OC2RS sets the pulse widith.
  OC2CON = OC_TIMER2_SRC |     //Timer3 source
           OC_PWM_FAULT_PIN_DISABLE;  //PWM, no fault detection
#endif
}

void configOutputCompare3(void) {
  T2CONbits.TON = 0;          //disable Timer when configuring Output compare
  //CONFIG_OC1_TO_RP(RB4_RP);   //map OC1 to RB4
  OC3CONbits.OCM2 = 0b110;
  OC3RS = 0;  //clear both registers
  OC3R = 0;
#ifdef OC3CON2
//turn on the compare toggle mode using Timer3
  OC3CON1 = OC_TIMER2_SRC |     //Timer3 source
            OC_PWM_CENTER_ALIGN;  //PWM
  OC3CON2 = OC_SYNCSEL_TIMER2;   //synchronize to timer3
#else
//older families, this PWM mode is compatible with center-aligned, OC1R=0
//as writes to OC1RS sets the pulse widith.
  OC3CON = OC_TIMER2_SRC |     //Timer2 source
           OC_PWM_FAULT_PIN_DISABLE;  //PWM, no fault detection
#endif
}

void configOutputCompare4(void) {
  T3CONbits.TON = 0;          //disable Timer when configuring Output compare
  //CONFIG_OC1_TO_RP(RB4_RP);   //map OC1 to RB4
  OC4CONbits.OCM2 = 0b110;
  OC4RS = 0;  //clear both registers
  OC4R = 0;
#ifdef OC4CON2
//turn on the compare toggle mode using Timer3
  OC4CON1 = OC_TIMER3_SRC |     //Timer3 source
            OC_PWM_CENTER_ALIGN;  //PWM
  OC4CON2 = OC_SYNCSEL_TIMER3;   //synchronize to timer3
#else
//older families, this PWM mode is compatible with center-aligned, OC1R=0
//as writes to OC1RS sets the pulse widith.
  OC4CON = OC_TIMER3_SRC |     //Timer3 source
           OC_PWM_FAULT_PIN_DISABLE;  //PWM, no fault detection
#endif
}

void configOutputCompare5(void) {
  T3CONbits.TON = 0;          //disable Timer when configuring Output compare
  //CONFIG_OC1_TO_RP(RB4_RP);   //map OC1 to RB4
  OC5CONbits.OCM2 = 0b110;
  OC5RS = 0;  //clear both registers
  OC5R = 0;
#ifdef OC5CON2
//turn on the compare toggle mode using Timer3
  OC5CON1 = OC_TIMER3_SRC |     //Timer3 source
            OC_PWM_CENTER_ALIGN;  //PWM
  OC5CON2 = OC_SYNCSEL_TIMER3;   //synchronize to timer3
#else
//older families, this PWM mode is compatible with center-aligned, OC1R=0
//as writes to OC1RS sets the pulse widith.
  OC5CON = OC_TIMER3_SRC |     //Timer3 source
           OC_PWM_FAULT_PIN_DISABLE;  //PWM, no fault detection
#endif
}

void _ISR _T2Interrupt(void) {
  _T2IF = 0;    //clear the timer interrupt bit
  //update the PWM duty cycle from the ADC value
  //compute new pulse width that is 0 to 99% of PR2
  // pulse width (PR2) * ADC/1024
}

void _ISR _T3Interrupt(void) {
  _T3IF = 0;    //clear the timer interrupt bit
  //update the PWM duty cycle from the ADC value
  //compute new pulse width that is 0 to 99% of PR2
  // pulse width (PR2) * ADC/1024
}

char sz_buf[32];

void getMotorValue(void) {
  int16_t u16_servo;
  int16_t u16_pw;
  printf("Choose dir (1 - forw, 2 - back, 3 - stop): ");
  inStringEcho(sz_buf,31);
  sscanf(sz_buf,"%d",(int *) &u16_servo);
  if ((u16_servo > 4) || (u16_servo < 1)) {
    printf("Invalid servo..\n");
    return;
  }
  printf("Enter pulse width: ");
  inStringEcho(sz_buf,31);
  sscanf(sz_buf,"%d",(int *) &u16_pw);

  if (u16_servo == 1)
  {
      printf("\n OC2RS = %u, OC3RS = %u , PWM_PER = %u \n", OC2RS, OC3RS, usToU16Ticks(PWM_PERIOD, getTimerPrescale(T2CONbits)));
      printf("%u = %u * duty \n", OC2RS, PWM_PERIOD);
      right_motor_fwd(u16_pw / 100.0);
      left_motor_fwd(u16_pw / 100.0);
  }
  else if (u16_servo == 2)
  {
      right_motor_reverse(u16_pw / 100.0);
      left_motor_reverse(u16_pw / 100.0);
  }

}

void motors_init(void)
{
  // Configure PWM
  configTimer2();
  configTimer3();
  configOutputCompare2();
  configOutputCompare3();
  configOutputCompare4();
  configOutputCompare5();
  T2CONbits.TON = 1;       //turn on Timer2 to start PWM
  T3CONbits.TON = 1;       //turn on Timer3 to start PWM
  CONFIG_RIGHT_MOTOR_IN1();
  CONFIG_RIGHT_MOTOR_IN2();
  CONFIG_LEFT_MOTOR_IN1();
  CONFIG_LEFT_MOTOR_IN2();
}

void left_motor_fwd(float duty)
	{
        LIN1_PULSE = usToU16Ticks(PWM_PERIOD, getTimerPrescale(T3CONbits));
        LIN2_PULSE = usToU16Ticks(PWM_PERIOD, getTimerPrescale(T3CONbits)) * (1 - duty);
	}

void left_motor_reverse(float duty)
	{
        LIN1_PULSE = usToU16Ticks(PWM_PERIOD, getTimerPrescale(T3CONbits)) * (1 - duty);
        LIN2_PULSE = usToU16Ticks(PWM_PERIOD, getTimerPrescale(T3CONbits));
	}

void right_motor_reverse(float duty)
	{
        RIN1_PULSE = usToU16Ticks(PWM_PERIOD, getTimerPrescale(T2CONbits));
        RIN2_PULSE = usToU16Ticks(PWM_PERIOD, getTimerPrescale(T2CONbits)) * (1 - duty);
	}

void right_motor_fwd(float duty)
	{
        RIN1_PULSE = usToU16Ticks(PWM_PERIOD, getTimerPrescale(T2CONbits)) * (1 - duty);
        RIN2_PULSE = usToU16Ticks(PWM_PERIOD, getTimerPrescale(T2CONbits));
	}

void turn_left(float duty)
	{
	right_motor_fwd(duty);
	left_motor_reverse(duty);
	}

void turn_right(float duty)
	{
	right_motor_reverse(duty);
	left_motor_fwd(duty);
	}

void stop(void)
	{
        OC2RS = 0;
        OC3RS = 0;
        OC4RS = 0;
        OC5RS = 0;
	}


void forward(float duty)
	{
	right_motor_fwd(duty);
	left_motor_fwd(duty);
	}

void reverse(float duty)
	{
	right_motor_reverse(duty);
	left_motor_reverse(duty);
	}
