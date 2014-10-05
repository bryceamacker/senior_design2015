#include <elapsedMillis.h>
#include <Servo.h>

elapsedMillis timeElapsed;

#define RED    0
#define GREEN  1
#define BLUE   2
#define YELLOW 3

#define BLUE_PRESS  1025
#define BLUE_AFTER_PRESS 1085
#define BLUE_BACK_OFF  2350

#define GREEN_PRESS  1825
#define GREEN_AFTER_PRESS 1750
#define GREEN_BACK_OFF  600

#define RED_PRESS  895
#define RED_AFTER_PRESS  925
#define RED_BACK_OFF  2350

#define YELLOW_PRESS  1050
#define YELLOW_AFTER_PRESS 1125
#define YELLOW_BACK_OFF  2300

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
int yellowLightPin = 3;

int lastRedValue = 0;
int lastGreenValue = 0;
int lastBlueValue = 0;
int lastYellowValue = 0;

int lowestRedValue = 1024;
int lowestGreenValue = 1024;
int lowestBlueValue = 1024;
int lowestYellowValue = 1024;

int currentRedValue = 0;
int currentGreenValue = 0;
int currentBlueValue = 0;
int currentYellowValue = 0;

int redDifference = 0;
int greenDifference = 0;
int blueDifference = 0;
int yellowDifference = 0;

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
  redServo.writeMicroseconds(RED_BACK_OFF);
  greenServo.writeMicroseconds(GREEN_BACK_OFF);
  yellowServo.writeMicroseconds(YELLOW_BACK_OFF);
  blueServo.writeMicroseconds(BLUE_BACK_OFF);
  delay(600);
  redServo.writeMicroseconds(RED_AFTER_PRESS);
  greenServo.writeMicroseconds(GREEN_AFTER_PRESS);
  yellowServo.writeMicroseconds(YELLOW_AFTER_PRESS);
  blueServo.writeMicroseconds(BLUE_AFTER_PRESS);
  delay(100);
  Serial.println("Calibrate");
  calibrateSensors();
  Serial.println("Calibrate Done");
  delay(1000);
  Serial.println("Pressing Start");
  //yellowServo.writeMicroseconds(YELLOW_PRESS);
}

void loop()
{
  readSensors();
//  findColor(roundNum);
//  if (firstRun == 1) {
//      delay(250);
//      yellowServo.writeMicroseconds(YELLOW_AFTER_PRESS);
//      firstRun = 0;
//    }
//  playButtons(roundNum);
//  roundNum++;
}
void playButtons(int num) {
  for(int i = 0; i < num; i++) {
    Serial.print("Number of buttons in array: ");
    Serial.println(num+1);
    if (buttonArray[i] == RED) {
      Serial.println("Pressing Red");
      redServo.write(RED_PRESS);
      delay(250);
      redServo.write(RED_AFTER_PRESS);
    }
    else if (buttonArray[i] == GREEN) {
      Serial.println("Pressing Green");
      greenServo.write(GREEN_PRESS);
      delay(250);
      greenServo.write(GREEN_AFTER_PRESS);
    } 
    else if (buttonArray[i] == BLUE) {
      Serial.println("Pressing Blue");
      blueServo.write(BLUE_PRESS);
      delay(250);
      blueServo.write(BLUE_AFTER_PRESS);
    } 
    else if (buttonArray[i] == YELLOW) {
      Serial.println("Pressing Yellow");
      yellowServo.write(YELLOW_PRESS);
      delay(250);
      yellowServo.write(YELLOW_AFTER_PRESS);
    } 
  }
  delay(500);
}

int findColor(int numberOfButtons) {
    // Attempt to identify color for 200 milliseconds each
    timeElapsed = 0;
      int color = YELLOW;
      int detectedButtons = 0;
      do {
        currentRedValue = analogRead(redLightPin);
        currentGreenValue = analogRead(greenLightPin);
        currentBlueValue = analogRead(blueLightPin);  
        currentYellowValue = analogRead(yellowLightPin);
        
        redDifference = currentRedValue - lowestRedValue;
        greenDifference = currentGreenValue - lowestGreenValue;
        blueDifference = currentBlueValue - lowestBlueValue;
        yellowDifference = currentYellowValue - lowestYellowValue;
  
      if (redDifference > 250)
      {
        buttonArray[detectedButtons] = RED;
        detectedButtons++;
        Serial.println("Red");
        color = RED;
        delay(250);
      }
      else if (greenDifference > 200)
      {
        buttonArray[detectedButtons] = GREEN;
        detectedButtons++;
        Serial.println("Green");
        color = GREEN;
        delay(250);
      }
      else if (blueDifference > 200)
      {
        buttonArray[detectedButtons] = BLUE;
        detectedButtons++;
        Serial.println("Blue");
        color = BLUE;
        delay(250);

      }      
      else if (yellowDifference > 200)
      {
        buttonArray[detectedButtons] = YELLOW;
        detectedButtons++;
        Serial.println("Yellow");
        color = YELLOW;
        delay(250);
      }
      
      //short delay for faster response to light.
      delay(10); 
      } while(detectedButtons < numberOfButtons);
    
    // Return yellow if we don't detect anything
    printArray(numberOfButtons);    
    // Wait for pause between button lights
    timeElapsed = 0;
}

void calibrateSensors() {
  for (int i = 0; i < 100; i++) {
      int tempRedValue = analogRead(redLightPin);
      int tempGreenValue = analogRead(greenLightPin);
      int tempBlueValue = analogRead(blueLightPin);
      int tempYellowValue = analogRead(yellowLightPin);
      
      Serial.print("Red : ");
      Serial.print(tempRedValue);
      Serial.print("\tGreen: ");
      Serial.print(tempGreenValue);
      Serial.print("\tBlue: ");
      Serial.print(tempBlueValue);
      Serial.print("\tYellow: ");
      Serial.print(tempYellowValue);
      
      if (tempRedValue < lowestRedValue) {
        lowestRedValue = tempRedValue;
      }
      if (tempGreenValue < lowestGreenValue) {
        lowestGreenValue = tempGreenValue;
      }
      if (tempBlueValue < lowestBlueValue) {
        lowestBlueValue = tempBlueValue;
      }
      if (tempYellowValue < lowestYellowValue) {
        lowestYellowValue = tempYellowValue;
      }
  delay(10);
  }
}

void readSensors() {
  while(true) {
      int tempRedValue = analogRead(redLightPin);
      int tempGreenValue = analogRead(greenLightPin);
      int tempBlueValue = analogRead(blueLightPin);
      int tempYellowValue = analogRead(yellowLightPin);
      
      Serial.print("Red : ");
      Serial.print(tempRedValue);
      Serial.print("\tGreen: ");
      Serial.print(tempGreenValue);
      Serial.print("\tBlue: ");
      Serial.print(tempBlueValue);
      Serial.print("\tYellow: ");
      Serial.println(tempYellowValue);
      delay(10);
  }
}

void printArray(int num) {
  for (int i = 0; i < num; i++) {
    Serial.print("Element ");
    Serial.print(i);
    Serial.print(": ");
    if (buttonArray[i] == RED)
      Serial.println("Red");
    if (buttonArray[i] == GREEN)
      Serial.println("Green");
    if (buttonArray[i] == BLUE)
      Serial.println("Blue");
    if (buttonArray[i] == YELLOW)
      Serial.println("Yellow");
  }
  Serial.println("Done printing array");
}
      

