#!/usr/bin/env python

"""odom_pub.py: Transcribes odometry data received via serial into odometry and tf messages"""

__authors__ = ["Dexter Duckworth"]
__maintainer__ = "Dexter Duckworth"
__date__ = "10/13/14"

import roslib
import rospy, tf
from nav_msgs.msg import Odometry
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Twist

import serial, atexit, sys, json

class odom_pub(object):

	max_linear = .5 # m/s
	max_angular = 3.0 # rad/s
	baud = 57600 #

	def __init__(self):
		self.serial_port = None
		self.ready = False
		self.vel = "!0,0,0\n"

		rospy.init_node('odom_pub')
		self.pub = rospy.Publisher('odom', Odometry)
		rospy.Subscriber('cmd_vel', Twist, self.cmd_vel_cb)
		self.port = rospy.get_param("motor_controller/port", 0)
		
		if self.init_serial():
			self.ready = True
			atexit.register(self.cleanup)
		else:
			sys.exit(1)
			

	def run(self):
		while not self.ready and not rospy.is_shutdown(): #Wait for serial connection to be found
			rospy.sleep(.1)

		while not rospy.is_shutdown():
			loop_rate = rospy.Rate(20)
			msg = Odometry()
			raw = self.serial_port.readline()
			parsed = None
			try:
				parsed = json.loads(raw)
			except:
				pass

			if type(parsed) is dict and "data" in parsed:
				try:
					data = parsed["data"]
					odom = data["odom"]
					encoders = data["encoders"]
					msg.pose.pose.position.x = odom["x"]
					msg.pose.pose.position.y = odom["y"]
					msg.pose.pose.position.z = 0.0

					quaternion = tf.transformations.quaternion_from_euler(0, 0, odom["theta"])
					msg.pose.pose.orientation.x = quaternion[0]
					msg.pose.pose.orientation.y = quaternion[1]
					msg.pose.pose.orientation.z = quaternion[2]
					msg.pose.pose.orientation.w = quaternion[3]

					msg.twist.twist.linear.x = odom["vx"]
					msg.twist.twist.linear.y = odom["vy"]
					msg.twist.twist.angular.z = odom["vtheta"]

					msg.header.stamp = rospy.Time.now()
					msg.header.frame_id = "odom"
					msg.child_frame_id = "base_frame"
				except:
					pass
				else:
					self.pub.publish(msg)
			else:
				pass

			self.serial_port.write(self.vel)
			self.serial_port.flush()
			loop_rate.sleep()

	def cmd_vel_cb(self, data):
		"""Scale joystick input [0,1] to the robot's maximum velocity, then store velocity
			to be published"""
		linear = int(min(odom_pub.max_linear, data.linear.x) * 1000) #Linear velocity in mm/s
		angular = int(min(odom_pub.max_angular, data.angular.z) * 1000) #Angular velocity in mrad/s
		self.vel = self.format_vel(linear, angular)

	def init_serial(self):
		"""Attempts to connect to device over a range of serial ports
			Returns true if any connection is successful
			Returns false if no connection is successful"""
		port_str = "/dev/ttyUSB" + str(self.port)
		try:
			self.serial_port = serial.Serial(port_str, odom_pub.baud)
		except Exception as er:
			rospy.logwarn("Unable to open connection on port {}...".format(port_str))
		else:
			rospy.loginfo("Connection opened on port {}".format(port_str))
			return True
		rospy.logerr("Unable to find valid serial connection.")
		return False


	def cleanup(self):
		"""Called at exit to close connection to device"""
		try:
			self.serial_port.close()
		except:
			pass

	def format_vel(self, linear, angular):
		return "!" + str(linear) + ',' + str(angular) + ',' +  str(self.get_chksum(linear, angular)) + '\n'

	def get_chksum(self, *args, **kwargs):
		return  -1 * (sum(args) & 0xFF)


if __name__ == "__main__":
	try:
		nd = odom_pub()
	except rospy.ROSInterruptException as er:
		rospy.logerr(str(er))
	else:
		nd.run()