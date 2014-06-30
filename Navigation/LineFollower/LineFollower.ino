#include <QTRSensors.h>
#include <Servo.h>

// For this example, we are using parallax continuous rotation servos
// You can refer to this webpage for further documentation on interfacing these motors w/ Arduino
// http://learn.parallax.com/KickStart/900-00008

// This example is designed for use with eight QTR-1RC sensors or the eight sensors of a
// QTR-8RC module.  These reflectance sensors should be connected to digital inputs 4 to 11.
// The QTR-8RC's emitter control pin (LEDON) can optionally be connected to digital pin 2,
// or you can leave it disconnected and change the EMITTER_PIN #define below from 12 to
// QTR_NO_EMITTER_PIN.

// The setup phase of this example calibrates the sensor for ten seconds and turns on
// the LED built in to the Arduino on pin 13 while calibration is going on.
// During this phase, you should expose each reflectance sensor to the lightest and
// darkest readings they will encounter.
// For example, if you are making a line follower, you should slide the sensors across the
// line during the calibration phase so that each sensor can get a reading of how dark the
// line is and how light the ground is.  Improper calibration will result in poor readings.
// If you want to skip the calibration phase, you can get the raw sensor readings
// (pulse times from 0 to 2500 us) by calling qtrrc.read(sensorValues) instead of
// qtrrc.readLine(sensorValues).

// The main loop of the example reads the calibrated sensor values and uses them to
// estimate the position of a line.  You can test this by taping a piece of 3/4" black
// electrical tape to a piece of white paper and sliding the sensor across it.  It
// prints the sensor values to the serial monitor as numbers from 0 (maximum reflectance)
// to 1000 (minimum reflectance) followed by the estimated location of the line as a number
// from 0 to 5000.  1000 means the line is directly under sensor 1, 2000 means directly
// under sensor 2, etc.  0 means the line is directly under sensor 0 or was last seen by
// sensor 0 before being lost.  5000 means the line is directly under sensor 5 or was
// last seen by sensor 5 before being lost.


#define NUM_SENSORS    8     // number of sensors used
#define TIMEOUT        2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN    12     // emitter is controlled by digital pin 2
#define LEFT_WHEEL     2
#define LEFT           2
#define RIGHT_WHEEL    3
#define LEFT           3

// sensors 0 through 7 are connected to digital pins 4 through 11, respectively
QTRSensorsRC qtrrc((unsigned char[])
{
    4, 5, 6, 7, 8, 9, 10, 11
},
NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];
Servo right;
Servo left;
boolean foundObjective = false;
boolean start = false;

const int buttonPin(13);
int buttonState = 0;
int lastButtonState = 0;


void setup()
{
    right.attach(RIGHT_WHEEL);
    left.attach(LEFT_WHEEL);
    delay(500);

    for (int i = 0; i < 360; i++)  // make the calibration take about 10 seconds
    {
        qtrrc.calibrate();
        // Swings the bot over the line and back while calibrating 
        if (i >= 135 && i < 180)
        {
            driveRev("right", 8);
            drive("left", 8);
        }
        else if (i >= 180 && i < 270)
        {
            drive("right", 8);
            driveRev("left", 8);
        }
        else if ( i >= 270 && i < 293)
        {
            driveRev("right", 8);
            drive("left", 8);
        }
        else
        {
            stopMotors();
        }
    }
    //digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

    // print the calibration minimum values measured when emitters were on
    Serial.begin(9600);
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        Serial.print(qtrrc.calibratedMinimumOn[i]);
        Serial.print(' ');
    }
    Serial.println();

    // print the calibration maximum values measured when emitters were on
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        Serial.print(qtrrc.calibratedMaximumOn[i]);
        Serial.print(' ');
    }
    Serial.println();
    Serial.println();
    delay(1000);
    drive("rigth", 10);
    drive("left", 10);
    delay(200);
    stopMotors();
}


void loop()
{
    // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
    // To get raw sensor values, call:
    //  qtrrc.read(sensorValues); instead of unsigned int position = qtrrc.readLine(sensorValues);
    unsigned int position = qtrrc.readLine(sensorValues);

    // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
    // 1000 means minimum reflectance, followed by the line position
    for (unsigned char i = 0; i < NUM_SENSORS; i++)
    {
        Serial.print(sensorValues[i]);
        Serial.print('\t');
    }
    //Serial.println(); // uncomment this line if you are using raw values
    Serial.println(position); // comment this line out if you are using raw values

    buttonState = digitalRead(buttonPin);
    if (buttonState != lastButtonState or start == true)
    {
        start = true;
        if (position > 4000) // 2500 is the position for 7 sensors when nothing is detected
        {
            //left.writeMicroseconds(1515);  // Move left motor forward, at a low speed ( 50 / 255)
            //right.writeMicroseconds(1500);
            drive("left", 8);
            stopMotor("rigth");
        }
        if (position < 2500)
        {
            //right.writeMicroseconds(1485);  // Move right motor forward
            //left.writeMicroseconds(1500);
            drive("right", 8);
            stopMotor("left");
        }
        if ((position >= 2500) && (position <= 4000)) // drive straight
        {
            //left.writeMicroseconds(1515);
            //right.writeMicroseconds(1485);
            drive("left", 8);
            drive("right", 8);
        }

        if (position == 0)
        {
            foundObjective = true;
            //digitalWrite(13, HIGH);
        }
        else
        {
            foundObjective = false;
        }

    }
    lastButtonState = buttonState;

    delay(250);
}


void drive(String motor, int spd)
{
  if (motor == "left")
    left.writeMicroseconds(map(spd, 0, 100, 1500, 1700));
  else if (motor == "right")
    right.writeMicroseconds(map(spd, 0, 100, 1500, 1300));
}

void driveRev(String motor, int spd)
{
  if (motor == "left")
    left.writeMicroseconds(map(spd, 0, 100, 1500, 1300));
  else if (motor == "right")
    right.writeMicroseconds(map(spd, 0, 100, 1500, 1700));
}

void stopMotor(String motor)
{
  if (motor == "left")
    left.writeMicroseconds(1500);
  else if (motor == "right")
    right.writeMicroseconds(1500);
}

void stopMotors()
{
  left.writeMicroseconds(1500);
  right.writeMicroseconds(1500);
}

