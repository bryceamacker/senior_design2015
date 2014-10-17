#!/usr/bin/env python

"""line_sensor.py: Transcribes line sensor data"""

__authors__ = ["Dexter Duckworth"]
__maintainer__ = "Dexter Duckworth"
__date__ = "10/13/14"

import roslib
import rospy

import serial, atexit, sys
from std_msgs.msg import Int16MultiArray
from sensor_msgs.msg import Joy

class line_sensor(object):

	baud = 9600 

	def __init__(self):
		self.serial_port = None
		self.ready = False
		self.buttons = [0 for i in xrange(12)]

		rospy.init_node('line_sensor')
		self.pub = rospy.Publisher('line_sensor', Int16MultiArray)
		rospy.Subscriber('joy', Joy, self.joy_cb)
		self.port = rospy.get_param("line_sensor/port", 1)
		
		if self.init_serial():
			self.ready = True
			atexit.register(self.cleanup)
			rospy.sleep(.5)
		else:
			sys.exit(1)
			

	def run(self):
		while not self.ready and not rospy.is_shutdown(): #Wait for serial connection to be found
			rospy.sleep(.1)

		while not rospy.is_shutdown():
			#loop_rate = rospy.Rate(10)
			msg = Int16MultiArray()
			raw = self.serial_port.readline()
			parsed = raw.strip("\r\n")
			parsed = parsed.split(',')
			int_array = []
			for i in parsed:
				try:
					int_array.append(int(i))
				except:
					pass
			if self.is_valid(int_array):
				msg.data = int_array[:-1]
				self.pub.publish(msg)

			#loop_rate.sleep()

	def init_serial(self):
		"""Attempts to connect to device over a range of serial ports
			Returns true if any connection is successful
			Returns false if no connection is successful"""
		port_str = "/dev/ttyUSB" + str(self.port)
		try:
			self.serial_port = serial.Serial(port_str, line_sensor.baud)
		except Exception as er:
			rospy.logwarn("Unable to open connection on port {}...".format(port_str))
		else:
			rospy.loginfo("Connection opened on port {}".format(port_str))
			return True
		rospy.logerr("Unable to find valid serial connection.")
		return False

	def calibrate_sensor(self, len_sec):
		cal_str = "!1" + str(len_sec) + ',' + str(self.get_chksum(1, len_sec)) + '\n'
		self.serial_port.write(cal_str)
		self.serial_port.flush()

	def joy_cb(self, data):
		if data.buttons[0] and not self.buttons[0]:
			self.calibrate_sensor(5)
			self.buttons[0] = True
		elif not data.buttons[0]:
			self.buttons[0] = False


	def cleanup(self):
		"""Called at exit to close connection to device"""
		try:
			self.serial_port.close()
		except:
			pass

	def is_valid(self, in_array):
		return not (sum(in_array) & 0xFF)

	def get_chksum(self, *args, **kwargs):
		return  -1 * (sum(args) & 0xFF)

if __name__ == "__main__":
	try:
		nd = line_sensor()
	except rospy.ROSInterruptException as er:
		rospy.logerr(str(er))
	else:
		nd.run()