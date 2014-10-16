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
from sensor_msgs.msg import Joy

import cv2, copy
import numpy as np

class follower(object):
	max_linear = .5 # m/s
	max_angular = 3.0 # rad/s

	def __init__(self):
		self.odom = Odometry()
		self.center = (0, 0)
		self.v_center = (0, 0)
		self.driving = False
		self.deriv = (0, 0)
		self.prev_center = (0, 0)

		rospy.init_node("follower")
		rospy.Subscriber('odom', Odometry, self.odom_cb)
		rospy.Subscriber('joy', Joy, self.joy_cb)
		self.cv_pub = rospy.Publisher('cmd_vel', Twist)

		self.cap = cv2.VideoCapture(1)
		cv2.namedWindow('raw')
		cv2.namedWindow('thresh')

	def odom_cb(self, data):
		self.odom = data

	def joy_cb(self, data):
		if data.buttons[0]:
			self.driving = True
		else:
			self.driving = False

	def get_deriv(self, event):
		if event.last_real is not None:
			d_t = event.current_real - event.last_real
			d_t = d_t.to_sec()
			d_c = (self.center[0] - self.prev_center[0], self.center[1] - self.prev_center[1])
			self.deriv = (float(d_c[0])/d_t, float(d_c[1])/d_t)
			self.prev_center = copy.deepcopy(self.center)

	def run(self):
		rospy.Timer(rospy.Duration(1), self.get_deriv)
		while not rospy.is_shutdown():
		    # Capture frame-by-frame
		    ret, frame = self.cap.read()

		    gray = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)
		    ret, thr = cv2.threshold(gray,150,255,0)

		    m = cv2.moments(thr)
		    try:
		        self.center = (int(m['m10']/m['m00']), int(m['m01']/m['m00']))
		    except:
		        pass
		    else:
		        cv2.circle(frame, self.center, 50, (0,0,255), -1)
		        cv2.line(frame, self.center, (self.center[0] + int(self.deriv[0]), self.center[1] + int(self.deriv[1])), (0,255,0), thickness=10)

		        
		    midX = frame.shape[1]/2
		    offset = self.center[0] - midX
		    offset_norm = float(offset) / float(frame.shape[0]/2) #[-1, 1]
		    msg = Twist()
		    if self.driving:
		    	msg.linear.x = follower.max_linear/2.0
		    msg.angular.z = -1 * offset_norm * follower.max_angular
		    self.cv_pub.publish(msg)

		    # Display the resulting frame
		    cv2.imshow('raw',frame)
		    cv2.imshow('thresh',thr)
		    if cv2.waitKey(1) & 0xFF == ord('q'):
		        break

		# When everything done, release the capture
		self.cap.release()
		cv2.destroyAllWindows()

		

if __name__ == "__main__":
	try:
		nd = follower()
	except rospy.ROSInterruptException as er:
		rospy.logerr(str(er))
	else:
		nd.run()