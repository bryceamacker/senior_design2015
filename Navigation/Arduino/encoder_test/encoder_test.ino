#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <tf/tf.h>
#include <odom_publisher/OdometryLite.h>
#include <Encoder.h>
#define ENCODER_DO_NOT_USE_INTERRUPTS

#define MAXVAL 2240 //Encoder ticks per rotation
#define WIDTH .26 //Width of robot from wheel to wheel in m
#define RADIUS .04 //Radius of each wheel in m
#define ENCODER_PERIOD 50 //time in ms between publishing of encoder/odom data
#define COMMAND_PERIOD 50 //time in ms between polling for velocity commands
#define VELOCITY_TIMEOUT 200 //time in ms before velocity is set to zero automatically
#define LIN1 2
#define LIN2 7
#define RIN1 3
#define RIN2 8
#define LOUT1 5
#define LOUT2 6
#define ROUT1 9
#define ROUT2 10

const float Kp = 0.4; // PID proportional control Gain (m/s)
const float Ki = 0; //PID Integral control gain (m/s)
const float Kd =  0; // PID Derivitave control gain (m/s)

//Encoder variables
//Encoder l_motor(LIN1, LIN2);
//Encoder r_motor(RIN1, RIN2);
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

//Control variables
String command_str = "";
float v_linear = 0;
float v_angular = 0;
float vl_target = 0;
float vr_target = 0;
float vl_corrected = 0;
float vr_corrected = 0;

/*
  No load speed @ 12V: 150 rpm
  No load speed @ 6V: 75 rpm
  Velocity = (RPM/60) * 2 * PI * RADIUS
  (PWM/255) * MAX_VEL = Velocity
  PWM = (Velocity/MAX_VEL) * 255
*/
float l_pwm = 0;
float r_pwm = 0;
const float MAX_VEL = (150/60) * 2 * PI * RADIUS; //Approx .6 m/s

//Error Variables
float l_prev_error = 0;
float l_cum_error = 0;
float r_prev_error = 0;
float r_cum_error = 0;

//Time variables
unsigned long prev_ms1 = 0;
unsigned long cur_ms1 = 0;
unsigned long prev_ms2 = 0;
unsigned long cur_ms2 = 0;

void cmd_vel_cb(const geometry_msgs::Twist& msg) {
  /*
    
  */
  
  v_linear = msg.linear.x;
  v_angular = msg.angular.z;
  get_encoder_velocities(); 
}

//ROS stuff
ros::NodeHandle nh;
odom_publisher::OdometryLite odom_msg;
geometry_msgs::Twist twist_msg;

ros::Publisher odom_pub("odom", &odom_msg);
ros::Subscriber<geometry_msgs::Twist> cmd_vel_sub("cmd_vel", &cmd_vel_cb);

void setup() {
  cur_ms1 = millis();
  cur_ms2 = millis();
  vel_time = millis();
  prev_ms1 = cur_ms1;
  prev_ms2 = cur_ms2;
  pinMode(LOUT1, OUTPUT);
  pinMode(LOUT2, OUTPUT);
  pinMode(ROUT1, OUTPUT);
  pinMode(ROUT2, OUTPUT);
  
  nh.initNode();
  nh.subscribe(cmd_vel_sub);
  nh.advertise(odom_pub);
}

void loop() {
  cur_ms1 = millis();
  cur_ms2 = millis();
  //nh.spinOnce();
  if (cur_ms1 - prev_ms1 > ENCODER_PERIOD) {
    prev_ms1 = cur_ms1;
    //get_encoders();
    print_odom();
  }
  if (cur_ms2 - prev_ms2 > COMMAND_PERIOD) {
    prev_ms2 = cur_ms2;
    //update_motors();
  }
  //delay(100);
}

void get_encoders(){
  /*
    Reads the value of each of the motor encoders then converts the encoder 
    ticks to x, y, and theta. Also extracts x, y, and theta velocities as well
    as wheel velocities
  */
  //l_encoderCnt = l_motor.read();
  //r_encoderCnt = -1 * r_motor.read();
  
  l_encoderCnt = wrap_val(l_encoderCnt, MAXVAL);
  //l_motor.write(l_encoderCnt);
  r_encoderCnt = wrap_val(r_encoderCnt, MAXVAL);
  //r_motor.write(-1 * r_encoderCnt);
  
  double dt = (millis() - vel_time); //time step
  dt /= 1000;//convert ms to s
  vel_time = millis();
  float l_diff;
  float r_diff;
  if (l_tmpCnt != l_encoderCnt || r_tmpCnt != r_encoderCnt) {
    //Only update odometry if there has been a change in encoder positions
    l_diff = l_encoderCnt - l_tmpCnt;
    r_diff = r_encoderCnt - r_tmpCnt;
    if (l_diff < (MAXVAL/-2)) {l_diff += MAXVAL;}
    if (l_diff > (MAXVAL/2)) {l_diff -= MAXVAL;}
    if (r_diff < (MAXVAL/-2)) {r_diff += MAXVAL;}
    if (r_diff > (MAXVAL/2)) {r_diff -= MAXVAL;}
    l_tmpCnt = l_encoderCnt; 
    r_tmpCnt = r_encoderCnt; 
  }
  get_odom(l_diff, r_diff, dt);
}

void get_odom(float left_d, float right_d, double time_d) {
  /*
  Takes two difference values for left and right encoders as well as time
  step then updates the x, y, and theta values
  */
  float center_d;
  left_d = 2 * PI * RADIUS * left_d/MAXVAL; //Convert ticks to distance
  right_d = 2 * PI * RADIUS * right_d/MAXVAL; //Convert ticks to distance
  center_d = (left_d + right_d)/2.0; 
  double dx = center_d * cos(theta); //Calculate change in x
  double dy = center_d * sin(theta); //Calculate change in y
  double dth = (left_d - right_d)/(2 * WIDTH); //Calculate change in theta
  x += dx;
  y += dy;
  theta += dth;
  if (theta > PI) {theta -= 2 * PI;} //wrap angle
  if (theta < -1 * PI) {theta += 2 * PI;}
  
  vx = dx/time_d; // m/s
  vy = dy/time_d; // m/s
  vtheta = dth/time_d; // rad/s
  vl = left_d/time_d; // m/s
  vr = right_d/time_d; // m/s
  
  
}

void print_odom() {
  /*
    
  */
  
  geometry_msgs::Quaternion q = tf::createQuaternionFromYaw(theta);
  
  odom_msg.header.stamp = nh.now();
  odom_msg.header.frame_id = "odom";
  odom_msg.child_frame_id = "base_frame";
  odom_msg.pose.position.x = x;
  odom_msg.pose.position.y = y;
  odom_msg.pose.orientation = q;
  odom_msg.twist.linear.x = vx;
  odom_msg.twist.linear.y = vy;
  odom_msg.twist.angular.z = vtheta;
  odom_pub.publish(&odom_msg);
}

void get_encoder_velocities() {
  /*
    Converts from linear and angular velocity to wheel velocity
      
    VELOCITY EQUATIONS (derived from odometry equations here: 
    http://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-186-mobile-autonomous-systems-laboratory-january-iap-2005/study-materials/odomtutorial.pdf
    Vleft = Vlinear - (width/2) * Vangular
    Vright = Vlinear + (width/2) * Vangular
  */
  
  vl_target = v_linear - (WIDTH/2) * v_angular;
  vr_target = v_linear + (WIDTH/2) * v_angular;
}

void update_motors() {
  /*
    PID controller section that finds the error terms for the left and right wheels
    then uses the control function to get the new PID update value and add it to the
    current velocity.
    Finally, it converts from wheel velocity to PWM output value and writes to the 
    appropriate pins.
  */
  float l_error = abs(vl_target - vl);
  float r_error = abs(vr_target - vr);
  l_cum_error += l_error;
  r_cum_error += r_error;
  float l_pid = (Kp * l_error) + (Kd * (l_error - l_prev_error)) + Ki * l_cum_error;
  float r_pid = (Kp * r_error) + (Kd * (r_error - r_prev_error)) + Ki * r_cum_error;
  
  l_prev_error = l_error;
  r_prev_error = r_error;
  
  vl_corrected += l_pid;
  vr_corrected += r_pid;
  
  float l_pwm = (vl_corrected/MAX_VEL) * 255;
  float r_pwm = (vr_corrected/MAX_VEL) * 255;
  
  /*
    If left pwm value is greater than the maximum, divide it by 255 to get the proportion
    by which it needs to be changed, then set it to 255 and divide the right pwm value
    by that value so that the left and right values remain proportional.
  */
  if (abs(l_pwm) > 255) {
    float proportion = abs(l_pwm)/255;
    l_pwm = sign(l_pwm) * 255;
    r_pwm /= proportion;
  }
  /*
    If the right value is still over the maximum, repeat the process for the right side.
  */
  if (abs(r_pwm) > 255) {
    float proportion = abs(r_pwm)/255;
    r_pwm = sign(r_pwm) * 255;
    l_pwm /= proportion;
  }
  
  /*
    Check the sign of the left and right pwm signals and then write to the appropriate pins
    in order to have the motors go forward for positive and backward for negative
  */
  if (l_pwm > 0) {
    analogWrite(LOUT1, abs(l_pwm));
    analogWrite(LOUT2, 0);
  }
  else {
    analogWrite(LOUT1, 0);
    analogWrite(LOUT2, abs(l_pwm));
  }
  
  if (r_pwm < 0) {
    analogWrite(ROUT1, abs(r_pwm));
    analogWrite(ROUT2, 0);
  }
  else {
    analogWrite(ROUT1, 0);
    analogWrite(ROUT2, abs(r_pwm));
  }
  
}

int wrap_val(int encVal, int maxVal) {
  /*
    Take a given value and wrap it from zero to the given maximum
  */
  encVal = (encVal > maxVal) ? (encVal - maxVal) : encVal;
  encVal = (encVal < 0) ? (encVal + maxVal) : encVal;
  return encVal;
}

int sign(float num) {
  /* Return the sign of a number */
  if (num > 0) {return 1;}
  else if (num < 0) {return -1;}
  else {return 0;}
}

