#include <Servo.h>

Servo verticalServo;
Servo horizontalServo;

int verticalPin = 9;
int horizontalPin = 10;

int angle = 0;

void setup() 
{ 
  verticalServo.attach(verticalPin);
  horizontalServo.attach(horizontalPin);  
} 
 
 
void loop() 
{ 
  horizontalServo.write(0);
  verticalServo.write(90);
  delay(2000);
  horizontalServo.write(90);
  verticalServo.write(0);
  delay(2000);
  horizontalServo.write(180);
  verticalServo.write(90);
  delay(2000);
  horizontalServo.write(90);
  verticalServo.write(180);
  delay(2000);
} 
