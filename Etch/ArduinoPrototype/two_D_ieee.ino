#include <Servo.h>
Servo verticalServo;
Servo horizontalServo;
int verticalPin = 9;
int horizontalPin = 10;
int angle = 0;
float unit = 1;//this will be however long it takes to turn approximately 4.4mm.  Need to test/figure this out.
int left = 0; //either clockwise or counterclockwise.  Need to test.
int right = 180;//either clockwise or counterclockwise.  Need to test.
int up = 0; //either clockwise or counterclockwise.  Need to test.
int down = 180;//either clockwise or counterclockwise.  Need to test.
int pause = 90;

int 
void setup()
{
verticalServo.attach(verticalPin);
horizontalServo.attach(horizontalPin); // attaches the servo on pin 9 to the servo object
}


void draw()
{
  //wait to be safe
  verticalServo.write(pause);
  horizontalServo.write(pause);
  delay(2000);
  
  //draw the I
  verticalServo.write(pause);
  horizontalServo.write(right);
  delay(unit*3);
  verticalServo.write(pause);
  horizontalServo.write(left);
  delay(unit*1.5);
  verticalServo.write(down);
  horizontalServo.write(pause);
  delay(unit*4);
  verticalServo.write(pause);
  horizontalServo.write(left);
  delay(unit*1.5);
  verticalServo.write(pause);
  horizontalServo.write(right);
  delay(unit*3);
  
  //transition
  verticalServo.write(pause);
  horizontalServo.write(right);
  delay(unit*2);
  
  //draw E
  verticalServo.write(up);
  horizontalServo.write(pause);
  delay(unit*4); 
  verticalServo.write(pause);
  horizontalServo.write(right);
  delay(unit*3);  
  verticalServo.write(pause);
  horizontalServo.write(left);
  delay(unit*3);
  verticalServo.write(down);
  horizontalServo.write(pause);
  delay(unit*2);
  verticalServo.write(pause);
  horizontalServo.write(right);
  delay(unit*2);
  verticalServo.write(pause);
  horizontalServo.write(left);
  delay(unit*2);
  verticalServo.write(down);
  horizontalServo.write(pause);
  delay(unit*2);
  verticalServo.write(pause);
  horizontalServo.write(right);
  delay(unit*3);
  
   //transition
  verticalServo.write(pause);
  horizontalServo.write(right);
  delay(unit*2);
  
  //draw E
  verticalServo.write(up);
  horizontalServo.write(pause);
  delay(unit*4); 
  verticalServo.write(pause);
  horizontalServo.write(right);
  delay(unit*3);  
  verticalServo.write(pause);
  horizontalServo.write(left);
  delay(unit*3);
  verticalServo.write(down);
  horizontalServo.write(pause);
  delay(unit*2);
  verticalServo.write(pause);
  horizontalServo.write(right);
  delay(unit*2);
  verticalServo.write(pause);
  horizontalServo.write(left);
  delay(unit*2);
  verticalServo.write(down);
  horizontalServo.write(pause);
  delay(unit*2);
  verticalServo.write(pause);
  horizontalServo.write(right);
  delay(unit*3
  
   //transition
  verticalServo.write(pause);
  horizontalServo.write(right);
  delay(unit*2);
  
  //draw E
  verticalServo.write(up);
  horizontalServo.write(pause);
  delay(unit*4); 
  verticalServo.write(pause);
  horizontalServo.write(right);
  delay(unit*3);  
  verticalServo.write(pause);
  horizontalServo.write(left);
  delay(unit*3);
  verticalServo.write(down);
  horizontalServo.write(pause);
  delay(unit*2);
  verticalServo.write(pause);
  horizontalServo.write(right);
  delay(unit*2);
  verticalServo.write(pause);
  horizontalServo.write(left);
  delay(unit*2);
  verticalServo.write(down);
  horizontalServo.write(pause);
  delay(unit*2);
  verticalServo.write(pause);
  horizontalServo.write(right);
  delay(unit*3);
  
  
  //safety time
  verticalServo.write(pause);
  horizontalServo.write(pause);
  delay(2000);
  
}
