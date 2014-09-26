#include <Servo.h>

#define RED    0
#define GREEN  1
#define BLUE   2
#define YELLOW 3

#define BLUE_PRESS  2050
#define BLUE_AFTER_PRESS 1970
#define GREEN_PRESS  925
#define GREEN_AFTER_PRESS 1000
#define RED_PRESS  2400
#define RED_AFTER_PRESS  2325
#define YELLOW_PRESS  925
#define YELLOW_AFTER_PRESS 1000

/* Simple test of the functionality of the photo resistor
 
 Connect the photoresistor one leg to pin 0, and pin to +5V
 Connect a resistor (around 10k is a good value, higher
 values gives higher readings) from pin 0 to GND. (see appendix of arduino notebook page 37 for schematics).
 
 ----------------------------------------------------
 
 PhotoR     10K
 +5    o---/\/\/--.--/\/\/---o GND
 |
 Pin 0 o-----------
 
 ----------------------------------------------------
 */

int redLightPin = 0;  //define a pin for Photo resistor
int greenLightPin = 1;
int blueLightPin = 2;

int lastRedValue = 0;
int lastGreenValue = 0;
int lastBlueValue = 0;

int lowestRedValue = 1024;
int lowestGreenValue = 1024;
int lowestBlueValue = 1024;

int currentRedValue = 0;
int currentGreenValue = 0;
int currentBlueValue = 0;

int redDifference = 0;
int greenDifference = 0;
int blueDifference = 0;

Servo redServo;  // create servo object to control a servo 
Servo greenServo;
Servo blueServo;
Servo yellowServo;

int buttonArray[8];
int roundNum = 1;    // AKA number of buttons to expect
int firstRun = 0;
int button;

void setup()
{
  Serial.begin(9600);  //Begin serial communcation
  redServo.attach(9);  // attaches the servo on pin 9 to the servo object 
  greenServo.attach(10);
  blueServo.attach(11);
  yellowServo.attach(6);
  firstRun = 1;
  greenServo.writeMicroseconds(GREEN_AFTER_PRESS);
  delay(250);
  redServo.writeMicroseconds(RED_AFTER_PRESS);
  blueServo.write(1950);
  yellowServo.writeMicroseconds(YELLOW_AFTER_PRESS);
  delay(100);
  calibrateSensors();
}

void loop()
{
  if (firstRun == 1) 
  {
    yellowServo.writeMicroseconds(1000);
    delay(50);
    buttonArray[0] = findColor();
    firstRun = 0;
    playButtons(1);
  }
  
  button = findColor();
  Serial.print("Red: "); 
  Serial.print(currentRedValue);
  Serial.print("\t");

  Serial.print("Green: ");
  Serial.print(currentGreenValue); 
  Serial.print("\t");

  Serial.print("Blue: ");
  Serial.print(currentBlueValue);
  Serial.print("\n");
  //with a 10k resistor divide the value by 2, for 100k resistor divide by 4.

}
void playButtons(int num) {
  for(int i = 0; i < num; i++) {
    if (buttonArray[i] == RED) {
      redServo.write(RED_PRESS);
      delay(250);
      redServo.write(RED_AFTER_PRESS);
    }
    else if (buttonArray[i] == GREEN) {
      greenServo.write(GREEN_PRESS);
      delay(250);
      greenServo.write(GREEN_AFTER_PRESS);
    } 
    else if (buttonArray[i] == BLUE) {
      blueServo.write(BLUE_PRESS);
      delay(250);
      blueServo.write(BLUE_AFTER_PRESS);
    } 
    else if (buttonArray[i] == YELLOW) {
      yellowServo.write(YELLOW_PRESS);
      delay(250);
      yellowServo.write(YELLOW_AFTER_PRESS);
    } 
  }
}

int findColor(void) {
    // Attempt to identify color for 50 milliseconds
    for (int i = 0; i < 10; i++) {
      currentRedValue = analogRead(redLightPin);
      currentGreenValue = analogRead(greenLightPin);
      currentBlueValue = analogRead(blueLightPin);
      
      redDifference = currentRedValue - lowestRedValue;
      greenDifference = currentGreenValue - lowestGreenValue;
      blueDifference = currentBlueValue - lowestBlueValue;

    if (redDifference > 250)
    {
      return RED;
    }
    else if (greenDifference > 200)
    {
      return GREEN;
    }
    else if (blueDifference > 200)
    {
      return BLUE;
    }
    
    //short delay for faster response to light.
    delay(5); 
    }
    
    // Return yellow if we don't detect anything
    return YELLOW;
}

void calibrateSensors() {
  for (int i = 0; i < 25; i++) {
      int tempRedValue = analogRead(redLightPin);
      int tempGreenValue = analogRead(greenLightPin);
      int tempBlueValue = analogRead(blueLightPin);
      
      if (tempRedValue < lowestRedValue) {
        lowestRedValue = tempRedValue;
      }
      if (tempGreenValue < lowestGreenValue) {
        lowestGreenValue = tempGreenValue;
      }
      if (tempBlueValue < lowestBlueValue) {
        lowestBlueValue = tempBlueValue;
      }
  delay(10);
  }
}

  

