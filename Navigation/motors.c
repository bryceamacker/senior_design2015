#include "pic24_all.h"
#include "motors.h"
#define PULSEWIDTH1 0xFFFF
#define CONFIG_LEFT_MOTOR_PWM()				CONFIG_RD0_AS_DIG_OUTPUT()
#define CONFIG_LEFT_MOTOR_DIRECTION()       CONFIG_RC14_AS_DIG_OUTPUT()  //AIN1
//#define CONFIG_LEFT_MOTOR_BRAKE()           CONFIG_RC13_AS_DIG_OUTPUT()  //AIN2
#define CONFIG_RIGHT_MOTOR_PWM()			CONFIG_RD1_AS_DIG_OUTPUT()
#define CONFIG_RIGHT_MOTOR_DIRECTION()      CONFIG_RD3_AS_DIG_OUTPUT()  //AIN1
//#define CONFIG_RIGHT_MOTOR_BRAKE()          CONFIG_RD2_AS_DIG_OUTPUT()  //AIN2
	
void motors_init() 
	{
	
	//CONFIG_RD4_AS_DIG_OUTPUT();
	_TRISD4 = 0;
	T2CONbits.TON = 0;
	_T2IE = 1;
	_T2IF = 0;
	PR2 = PULSEWIDTH1;
	OC1CONbits.OCM = 6;
	OC2CONbits.OCM = 6;
	_OCM = 0b110;
	//_OCM = 0b110;
	OC1R = 0;
	OC2R = 0;
	OC1RS = 0;
	OC2RS = 0;
	T2CONbits.TON = 1;

	CONFIG_LEFT_MOTOR_PWM();
	CONFIG_LEFT_MOTOR_DIRECTION();
	//CONFIG_LEFT_MOTOR_BRAKE();
	CONFIG_RIGHT_MOTOR_PWM();
	CONFIG_RIGHT_MOTOR_DIRECTION();
	//CONFIG_RIGHT_MOTOR_BRAKE();

	OC1RS = 0;
	OC2RS = 0;
	//while (!_T2IF);
	stop();
	CONFIG_RD4_AS_DIG_INPUT();
	}

void PWM1_duty(float duty) 
	{
	OC1RS = PR2 * duty;
	}

void PWM2_duty(float duty) 
	{
	OC2RS = PR2 * duty;
	}

 void _ISR _T2Interrupt(void) 
	{
    _T2IF = 0; //clear the timer interrupt bit
  	}


void left_motor_fwd(float duty)
	{
	_RC13 = 0; // No brake							
	_RC14 = 0; //0 = Forward | 1 = Backwards
	PWM1_duty(duty);
	}

void left_motor_reverse(float duty)
	{
	_RC13 = 0; // No brake							
	_RC14 = 1; //0 = Forward | 1 = Backwards
	PWM1_duty(1-duty);
	}
	
void right_motor_fwd(float duty)
	{
	_RD2 = 0; // No brake							
	_RD3 = 0; //Forward. 1 = Backwards
	PWM2_duty(duty);
	}
	
void right_motor_reverse(float duty)
	{
	_RD2 = 0; // No brake							
	_RD3 = 1; //Forward. 1 = Backwards
	PWM2_duty(1-duty);
	}

void turn_left(float duty)
	{
	right_motor_fwd(duty);
	//left_motor_back(0);
	left_motor_reverse(duty);
	}

void turn_right(float duty)
	{
	right_motor_reverse(duty);
	//right_motor_back(0);
	left_motor_fwd(duty);
	}

void stop(void)
	{
	right_motor_fwd(0.0);
	left_motor_fwd(0.0);
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







	