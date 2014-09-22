#include <Servo.h>

Servo verticalServo;
Servo horizontalServo;

int verticalPin = 9;
int horizontalPin = 10;

int angle = 0;

void setup() 
{ 
  verticalServo.attach(verticalPin);
  horizontalServo.attach(horizontalPin);  // attaches the servo on pin 9 to the servo object 
} 
 
 
void loop() 
{ 
  horizontalServo.write(0);
  verticalServo.write(0);
  delay(2000);
  horizontalServo.write(170);
  verticalServo.write(170);
  delay(2000);
} 
