#!/usr/bin/env python

"""robot_teleop.py: Receives joystick data and publishes cmd_vel Twist"""

__authors__ = ["Dexter Duckworth"]
__maintainer__ = "Dexter Duckworth"
__date__ = "10/13/14"

import roslib
import rospy, tf
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Twist

class teleop(object):
	max_linear = .5 # m/s
	max_angular = 3.0 # rad/s

	def __init__(self):
		rospy.init_node("teleop")
		rospy.Subscriber('joy', Joy, self.joy_cb)
		self.cv_pub = rospy.Publisher('cmd_vel', Twist)

	def joy_cb(self, data):
		msg = Twist()
		msg.linear.x = data.axes[1] * teleop.max_linear
		msg.angular.z = data.axes[0] * teleop.max_angular
		self.cv_pub.publish(msg)

if __name__ == "__main__":
	try:
		nd = teleop()
	except rospy.ROSInterruptException as er:
		rospy.logerr(str(er))
	else:
		rospy.spin()