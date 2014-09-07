
#include "pic24_all.h"
#include "pic24_qtr8rc.h"

uint16_t u16_maxValue = 0x7FFF;

//configures pins as dig outputs
void CONFIG_OUTPUTS(void) {
	CONFIG_RB0_AS_DIG_OUTPUT();
	CONFIG_RB1_AS_DIG_OUTPUT();
	CONFIG_RB2_AS_DIG_OUTPUT();
	CONFIG_RB3_AS_DIG_OUTPUT();
	CONFIG_RB4_AS_DIG_OUTPUT();
	CONFIG_RB5_AS_DIG_OUTPUT();
	CONFIG_RB6_AS_DIG_OUTPUT();
	CONFIG_RB7_AS_DIG_OUTPUT();
	CONFIG_RB8_AS_DIG_OUTPUT();
	CONFIG_RB9_AS_DIG_OUTPUT();
	CONFIG_RB10_AS_DIG_OUTPUT();
        CONFIG_RB11_AS_DIG_OUTPUT();
	CONFIG_RB12_AS_DIG_OUTPUT();
	CONFIG_RB13_AS_DIG_OUTPUT();
	CONFIG_RB14_AS_DIG_OUTPUT();
}

//configures pins as dig inpts
void CONFIG_INPUTS(void) {
	CONFIG_RB0_AS_DIG_INPUT();
	DISABLE_RB0_PULLUP();
	CONFIG_RB1_AS_DIG_INPUT();
	DISABLE_RB1_PULLUP();
	CONFIG_RB2_AS_DIG_INPUT();
	DISABLE_RB2_PULLUP();
	CONFIG_RB3_AS_DIG_INPUT();
	DISABLE_RB3_PULLUP();
	CONFIG_RB4_AS_DIG_INPUT();
	DISABLE_RB4_PULLUP();
	CONFIG_RB5_AS_DIG_INPUT();
	DISABLE_RB5_PULLUP();
	CONFIG_RB6_AS_DIG_INPUT();
	DISABLE_RB6_PULLUP();
	CONFIG_RB7_AS_DIG_INPUT();
	DISABLE_RB7_PULLUP();
	CONFIG_RB8_AS_DIG_INPUT();
	DISABLE_RB8_PULLUP();
	CONFIG_RB9_AS_DIG_INPUT();
	DISABLE_RB9_PULLUP();
	CONFIG_RB10_AS_DIG_INPUT();
	DISABLE_RB10_PULLUP();
        CONFIG_RB11_AS_DIG_INPUT();
	DISABLE_RB11_PULLUP();
	CONFIG_RB12_AS_DIG_INPUT();
	DISABLE_RB12_PULLUP();
	CONFIG_RB13_AS_DIG_INPUT();
	DISABLE_RB13_PULLUP();
	CONFIG_RB14_AS_DIG_INPUT();
	DISABLE_RB14_PULLUP();
}

//configures timer to measure discharge time of the capacitor
void  configTimer4(void) {
  T4CON = T4_OFF | T4_IDLE_CON | T4_GATE_OFF
          | T4_32BIT_MODE_OFF
          | T4_SOURCE_INT
          | T4_PS_1_8;  //1 tick = 0.2 us at FCY=40 MHz
  PR4 = 0xFFFF;                    //maximum period
  TMR4  = 0;                       //clear timer2 value
  _T4IF = 0;                       //clear interrupt flag
  T4CONbits.TON = 1;               //turn on the timer
} 

//dissables LED
void emittersOff(void) {
	_RB8 = 0;
	DELAY_US(200);
}

//enables LED
void emittersOn(void) {
	_RB8 = 1;
	DELAY_US(200);
}

//measures the discharge time for each capacitor associated with a sensor
void readValues(uint16_t* pau16_sensor_values) {
	uint16_t u16_start, u16_delta, u16_i, u16_pin;
	uint8_t pau8_sensorCheck[SENSOR_NUM];
	CONFIG_OUTPUTS();
	DELAY_US(1);
	for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
		pau16_sensor_values[u16_i] = u16_maxValue;
		pau8_sensorCheck[u16_i] = 0;
	}
	//drive outputs RB0 - RB7 and emitter high
	//PORTB = 0x01FF;
        // 0000 0001 1111 1111
        // RB0 - RB8

        //drive outputs RB0 - RB14
        PORTB = 0x7FFF;
	DELAY_US(10);
        // 0111 1111 1111 1111
        // RB0 - RB14
	
	//change outputs to inputs
	//dissable internal pullups
	CONFIG_INPUTS();
	DELAY_US(1);
	//mesure time for each input

	configTimer4();
	u16_start = TMR4;
	while(TMR4 - u16_start < u16_maxValue) {
		u16_delta = TMR4 - u16_start;
		for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
			u16_pin = 0x0001;
			u16_pin = u16_pin << u16_i;
			if(((PORTB & u16_pin) == 0) && (u16_delta < pau16_sensor_values[u16_i])) {
				pau16_sensor_values[u16_i] = u16_delta;
			}
		}
	}
	T4CONbits.TON = 0;
}

//reads the sensor values using a specific read mode
void read(uint16_t* pau16_sensor_values, char u8_readMode) {
	uint16_t pau16_off_values[SENSOR_NUM];
	uint16_t u16_i;
	//emitter pin = RB8
	CONFIG_RB8_AS_DIG_OUTPUT();
	if(u8_readMode == QTR_EMITTERS_ON || u8_readMode == QTR_EMITTERS_ON_AND_OFF) {
		emittersOn();
	}
	readValues(pau16_sensor_values);

	emittersOff();
	if(u8_readMode == QTR_EMITTERS_ON_AND_OFF) {
		readValues(pau16_off_values);
	}

	if(u8_readMode == QTR_EMITTERS_ON_AND_OFF) {
		for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
			pau16_sensor_values[u16_i] += u16_maxValue - pau16_off_values[u16_i];
		}
	}
}

//kalibretes to set the max value
void kalibrate(char u8_readMode) {
	uint16_t* pau16_sensorValues[SENSOR_NUM];
	read(pau16_sensorValues, u8_readMode);
	uint16_t u16_i;
	uint16_t u16_minValue = pau16_sensorValues[0];
	for(u16_i = 1; u16_i < SENSOR_NUM; u16_i++) {
            printf(" %u \t", pau16_sensorValues[u16_i]);
		if(pau16_sensorValues[u16_i] < u16_minValue) {
			u16_minValue = pau16_sensorValues[u16_i];
		}
	}
	u16_maxValue = u16_minValue*2.5; 
} 

//binary encodes the raw values of each sensor
void readLine(uint16_t* pau16_sensorValues, char u8_readMode) {
	read(pau16_sensorValues,u8_readMode);
	uint16_t u16_i;
	/*
	for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
		printf(" %u \t",pau16_sensorValues[u16_i]);
		DELAY_MS(1);
	}
	*/
	for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
		if(pau16_sensorValues[u16_i] < u16_maxValue - 1) {
			pau16_sensorValues[u16_i] = 1;
		}
		else {
			pau16_sensorValues[u16_i] = 0;
		}
	}
} 

/*
int main() {
	configBasic(HELLO_MSG);
	uint16_t pau16_sensor_values[SENSOR_NUM];
	uint16_t u16_i;
	while(1) {
		outString("DATA: ");
		read(pau16_sensor_values, QTR_EMITTERS_ON);
		for(u16_i = 0; u16_i < SENSOR_NUM; u16_i++) {
			printf(" %ul \t",pau16_sensor_values[u16_i]);
			DELAY_MS(10);
		}
		DELAY_MS(1000);
	}
}
*/