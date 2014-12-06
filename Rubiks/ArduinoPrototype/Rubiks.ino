// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.


#include <Servo.h> 
 
 Servo platform;
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  platform.attach(10);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
//  platform.write(180);
//  delay(2000);
//  myservo.write(0);
//  platform.write(90 - 30 - 6);
//  myservo.write(10);
//  delay(2000);
} 
 
 
void loop() 
{ 
//  myservo.write(0);
//  delay(2000);
//  myservo.write(170);
//  delay(2000);
} 
