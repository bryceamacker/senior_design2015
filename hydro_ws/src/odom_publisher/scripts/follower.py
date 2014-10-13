#!/usr/bin/env python

"""follower.py: Receives line sensor data and odometry and uses them to update cmd_vel"""

__authors__ = ["Dexter Duckworth"]
__maintainer__ = "Dexter Duckworth"
__date__ = "10/13/14"

import roslib
import rospy, tf
from std_msgs.msg import Int16MultiArray
from geometry_msgs.msg import Twist
from nav_msgs.msg import Odometry

class follower(object):
	max_linear = .5 # m/s
	max_angular = 3.0 # rad/s

	def __init__(self):
		self.sensor_array = []
		self.l_line = 0
		self.r_line = 0
		self.odom = Odometry()

		rospy.init_node("follower")
		rospy.Subscriber('line_sensor', Int16MultiArray, self.line_cb)
		rospy.Subscriber('odom', Odometry, self.odom_cb)
		self.cv_pub = rospy.Publisher('cmd_vel', Twist)

	def line_cb(self, data):
		self.sensor_array = data.data[:-2]
		self.l_line = data.data[-2]
		self.r_line = data.data[-1]
		self.find_line()

	def odom_cb(self, data):
		self.odom = data

	def find_line(self):
		if (self.l_line and self.l_line != 5000.0) or (self.r_line and self.r_line != 5000.0):
			l_pos = -1 + (self.l_line/5000.0) 
			r_pos = (self.r_line/5000.0)
			print("Left: " + str(l_pos))
			print("Right: " + str(r_pos))

	def run(self):
		while not rospy.is_shutdown():
			r = rospy.Rate(10)


			r.sleep()
		

if __name__ == "__main__":
	try:
		nd = follower()
	except rospy.ROSInterruptException as er:
		rospy.logerr(str(er))
	else:
		rospy.spin()