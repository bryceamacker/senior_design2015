import serial
import Adafruit_BBIO.UART as UART

		
def waitForContact(serVar):
	while True:
		data = serVar.readline()
		if "Contact" in data:
			break

def main():
	# Config the serial line
	ser = serial.Serial(port = "/dev/tty01", baudrate=9600)

	ser.open()
	# Make definitions for the motors
	leftMotor = 1
	rightMotor = 2

	waitForContact(ser)
	while True:
		print(ser.readline())
			
if __name__ == "__main__":
    main()