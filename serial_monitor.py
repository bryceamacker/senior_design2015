#import serial
#import Adafruit_BBIO.UART as UART
from bbio import Serial2
import Adafruit_BBIO.PWM as PWM

def waitForContact():
	while True:
		data = Serial2.read()
		if data is 55:
			break

def move(motor, speed, direction):
	Serial2.write(motor)
	Serial2.write(speed)
	Serial2.write(direction)
	
def main():
	# Config the serial line
	# UART.setup("UART1")
	
	# Serial2 are TX/RX pins P9.21 and P9.22 on the BeagleBone
	Serial2.begin(9600)
	
	# Make definitions for the motors
	leftMotor = 1
	rightMotor = 2

	# Make servo definitions
	servo_pin = "P9_14"
	duty_min = 3
	duty_max = 14.5
	duty_span = duty_max - duty_min
	PWM.start(servo_pin, (100-duty_min), 60.0)

	#Ignoring this function for now, but is good practice 
	#waitForContact()
	while True:
		position = Serial2.read()
		print(position)
		move(1, 57, 1)
		# if (position < 2000):
			# move(1, INC_SPEED, 1)
			# move(2, NORMAL_SPEED, 1)
		# if (position > 3000):
			# move(2, INC_SPEED, 1)  
			# move(1, NORMAL_SPEED, 1)
		# if ((position > 2000) && (position < 3000)):
			# move(1, NORMAL_SPEED, 1)
			# move(2, NORMAL_SPEED, 1)

		if (position == 0):
			duty = 100 - ((angle_f / 180) * duty_span + duty_min)
			PWM.set_duty_cycle(servo_pin, duty)

			
if __name__ == "__main__":
    main()
