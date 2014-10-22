#include <elapsedMillis.h>
#include <Servo.h>

elapsedMillis timeElapsed;

#define RED    0
#define GREEN  1
#define BLUE   2
#define YELLOW 3

#define BLUE_PRESS  990
#define BLUE_AFTER_PRESS 1025
#define BLUE_BACK_OFF  2350

#define GREEN_PRESS  1875
#define GREEN_AFTER_PRESS 1825
#define GREEN_BACK_OFF  600

#define RED_PRESS  950
#define RED_AFTER_PRESS  990
#define RED_BACK_OFF  2350

#define YELLOW_PRESS  1075
#define YELLOW_AFTER_PRESS 1115
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

int redOnValue = 0;
int greenOnValue = 0;
int blueOnValue = 0;
int yellowOnValue = 0;


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
}

void loop()
{
  readSensors();
 if (firstRun == 1) {
     yellowServo.writeMicroseconds(YELLOW_PRESS);
     delay(250);
     findColor(roundNum);
     yellowServo.writeMicroseconds(YELLOW_AFTER_PRESS);
     delay(250);
     playButtons(roundNum);
     roundNum++;
     firstRun = 0;
   }
 else {
   findColor(roundNum);
   playButtons(roundNum);
   roundNum++;
 }
}
void playButtons(int num) {
  delay(500);
  Serial.print("Number of buttons in array: ");
  Serial.println(num);
  for(int i = 0; i < num; i++) {
    if (buttonArray[i] == RED) {
      delay(500);
      Serial.println("Pressing Red");
      redServo.write(RED_PRESS);
      delay(250);
      redOnValue = confirmColor(RED);
      redServo.write(RED_AFTER_PRESS);
      confirmColorOff(RED, redOnValue);
    }
    else if (buttonArray[i] == GREEN) {
      delay(500);
      Serial.println("Pressing Green");
      greenServo.write(GREEN_PRESS);
      delay(250);
      greenOnValue = confirmColor(GREEN);
      greenServo.write(GREEN_AFTER_PRESS);
      confirmColorOff(GREEN, greenOnValue);
    } 
    else if (buttonArray[i] == BLUE) {
      delay(500);
      Serial.println("Pressing Blue");
      blueServo.write(BLUE_PRESS);
      delay(250);
      blueOnValue = confirmColor(BLUE);
      blueServo.write(BLUE_AFTER_PRESS);
      confirmColorOff(BLUE, blueOnValue);
    } 
    else if (buttonArray[i] == YELLOW) {
      delay(500);
      Serial.println("Pressing Yellow");
      yellowServo.write(YELLOW_PRESS);
      delay(250);
      yellowOnValue = confirmColor(YELLOW);
      yellowServo.write(YELLOW_AFTER_PRESS);
      confirmColorOff(YELLOW, yellowOnValue);
    } 
  }
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
        Serial.println("Detected Red");
        color = RED;
        confirmColorOff(RED, currentRedValue);
      }
      else if (greenDifference > 200)
      {
        buttonArray[detectedButtons] = GREEN;
        detectedButtons++;
        Serial.println("Detected Green");
        color = GREEN;
        confirmColorOff(GREEN, currentGreenValue);
      }
      else if (blueDifference > 200)
      {
        buttonArray[detectedButtons] = BLUE;
        detectedButtons++;
        Serial.println("Detected Blue");
        color = BLUE;
        confirmColorOff(BLUE, currentBlueValue);

      }      
      else if (yellowDifference > 200)
      {
        buttonArray[detectedButtons] = YELLOW;
        detectedButtons++;
        Serial.println("Detected Yellow");
        color = YELLOW;
        confirmColorOff(YELLOW, currentYellowValue);
      }
      
      //short delay for faster response to light.
      delay(10); 
      } while(detectedButtons < numberOfButtons);
}

int confirmColor(int color) {
    // Attempt to identify color for 200 milliseconds each
      while(true) {
        currentRedValue = analogRead(redLightPin);
        currentGreenValue = analogRead(greenLightPin);
        currentBlueValue = analogRead(blueLightPin);  
        currentYellowValue = analogRead(yellowLightPin);
        
        redDifference = currentRedValue - lowestRedValue;
        greenDifference = currentGreenValue - lowestGreenValue;
        blueDifference = currentBlueValue - lowestBlueValue;
        yellowDifference = currentYellowValue - lowestYellowValue;
  
      if (redDifference > 250 && color == RED)
      {
        return currentRedValue;
      }
      else if (greenDifference > 200 && color == GREEN)
      {
        return currentGreenValue;
      }
      else if (blueDifference > 200 && color == BLUE)
      {
        return currentBlueValue;

      }      
      else if (yellowDifference > 200 && color == YELLOW)
      {
        return currentYellowValue;
      }
      
      //short delay for faster response to light.
      delay(10);
      }
}

int confirmColorOff(int color, int onValue) {
    // Attempt to identify color for 200 milliseconds each
      while(true) {
        Serial.println("Waiting to turn off");
        currentRedValue = analogRead(redLightPin);
        currentGreenValue = analogRead(greenLightPin);
        currentBlueValue = analogRead(blueLightPin);  
        currentYellowValue = analogRead(yellowLightPin);
        
        redDifference = onValue - currentRedValue;
        greenDifference = onValue - currentGreenValue;
        blueDifference = onValue - currentBlueValue;
        yellowDifference = onValue - currentYellowValue;
  
      if (redDifference > 180 && color == RED)
      {
        return true;
      }
      else if (greenDifference > 150 && color == GREEN)
      {
        return true;
      }
      else if (blueDifference > 150 && color == BLUE)
      {
        return true;

      }      
      else if (yellowDifference > 150 && color == YELLOW)
      {
        return true;
      }
      
      //short delay for faster response to light.
      delay(10);
      }
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
      

