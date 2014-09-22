#include <Encoder.h>

#define MAXVAL 2240 //Encoder ticks per rotation
#define WIDTH .26 //Width of robot from wheel to wheel in m
#define RADIUS .04 //Radius of each wheel in m
#define ENCODER_PERIOD 100 //time in ms between publishing of encoder/odom data

//Encoder variables
Encoder l_motor(2, 4);
Encoder r_motor(3, 5);
int l_encoderCnt = 0;
int l_tmpCnt = 0;
int r_encoderCnt = 0;
int r_tmpCnt = 0;

//Odometry variables
float x;
float y;
float theta;
float vx;
float vy;
float vtheta;
float vl;
float vr;
unsigned long vel_time = 0; 

//Time variables
unsigned long prev_ms = 0;
unsigned long cur_ms = 0;

void setup() {
  Serial.begin(9600);
  cur_ms = millis();
  prev_ms = cur_ms;
  vel_time = millis();
}

void loop() {
  
  cur_ms = millis();
  if (cur_ms - prev_ms > ENCODER_PERIOD) {
    get_encoders();
    //print_odom();
    prev_ms = cur_ms;
  }
}

void get_encoders(){
  l_encoderCnt = l_motor.read();
  r_encoderCnt = -1 * r_motor.read();
  
  l_encoderCnt = wrap_val(l_encoderCnt, MAXVAL);
  l_motor.write(l_encoderCnt);
  r_encoderCnt = wrap_val(r_encoderCnt, MAXVAL);
  r_motor.write(-1 * r_encoderCnt);
  
  double dt = (millis() - vel_time);
  dt /= 1000;//convert ms to s
  vel_time = millis();
  
  if (l_tmpCnt != l_encoderCnt || r_tmpCnt != r_encoderCnt) {
    float l_diff = l_encoderCnt - l_tmpCnt;
    float r_diff = r_encoderCnt - r_tmpCnt;
    if (l_diff < (MAXVAL/-2)) {l_diff += MAXVAL;}
    if (l_diff > (MAXVAL/2)) {l_diff -= MAXVAL;}
    if (r_diff < (MAXVAL/-2)) {r_diff += MAXVAL;}
    if (r_diff > (MAXVAL/2)) {r_diff -= MAXVAL;}
    get_odom(l_diff, r_diff, dt);
    Serial.print("Time step: ");
    Serial.println(dt);
    print_odom();
    l_tmpCnt = l_encoderCnt; 
    r_tmpCnt = r_encoderCnt; 
  }
}

/*
  Takes two difference values for left and right encoders then 
  updates the x, y, and theta values
*/
void get_odom(float left_d, float right_d, double time_d) {
  float center_d;
  left_d = 2 * PI * RADIUS * left_d/MAXVAL; //Convert ticks to distance
  right_d = 2 * PI * RADIUS * right_d/MAXVAL; //Convert ticks to distance
  center_d = (left_d + right_d)/2.0; 
  double dx = center_d * cos(theta); //Calculate change in x
  double dy = center_d * sin(theta); //Calculate change in y
  double dth = (right_d - left_d)/(2 * WIDTH); //Calculate change in theta
  x += dx;
  y += dy;
  theta += dth;
  if (theta > PI) {theta -= 2 * PI;}
  if (theta < -1 * PI) {theta += 2 * PI;}
  
  vx = dx/time_d; // m/s
  vy = dy/time_d; // m/s
  vtheta = dth/time_d; // rad/s
  vl = left_d/time_d; // m/s
  vr = right_d/time_d; // m/s
  
  
}

void print_odom() {
  Serial.print("X: ");
  Serial.println(x);
  Serial.print("Y: ");
  Serial.println(y);
  Serial.print("Theta: ");
  Serial.println(theta * 180/PI);
  Serial.print("VX: ");
  Serial.println(vx);
  Serial.print("VY: ");
  Serial.println(vy);
  Serial.print("VTheta: ");
  Serial.println(vtheta);
  Serial.print("Left encoder: ");
  Serial.println(l_encoderCnt);
  Serial.print("Right encoder: ");
  Serial.println(r_encoderCnt);
  Serial.print("Left vel: ");
  Serial.println(vl);
  Serial.print("Right vel: ");
  Serial.println(vr);
}

/*
  Take a given value and wrap it from zero to the given maximum
*/
int wrap_val(int encVal, int maxVal) {
  encVal = (encVal > maxVal) ? (encVal - maxVal) : encVal;
  encVal = (encVal < 0) ? (encVal + maxVal) : encVal;
  return encVal;
}

