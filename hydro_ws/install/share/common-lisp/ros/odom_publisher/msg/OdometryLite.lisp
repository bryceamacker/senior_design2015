; Auto-generated. Do not edit!


(cl:in-package odom_publisher-msg)


;//! \htmlinclude OdometryLite.msg.html

(cl:defclass <OdometryLite> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (child_frame_id
    :reader child_frame_id
    :initarg :child_frame_id
    :type cl:string
    :initform "")
   (pose
    :reader pose
    :initarg :pose
    :type geometry_msgs-msg:Pose
    :initform (cl:make-instance 'geometry_msgs-msg:Pose))
   (twist
    :reader twist
    :initarg :twist
    :type geometry_msgs-msg:Twist
    :initform (cl:make-instance 'geometry_msgs-msg:Twist)))
)

(cl:defclass OdometryLite (<OdometryLite>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <OdometryLite>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'OdometryLite)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name odom_publisher-msg:<OdometryLite> is deprecated: use odom_publisher-msg:OdometryLite instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <OdometryLite>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader odom_publisher-msg:header-val is deprecated.  Use odom_publisher-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'child_frame_id-val :lambda-list '(m))
(cl:defmethod child_frame_id-val ((m <OdometryLite>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader odom_publisher-msg:child_frame_id-val is deprecated.  Use odom_publisher-msg:child_frame_id instead.")
  (child_frame_id m))

(cl:ensure-generic-function 'pose-val :lambda-list '(m))
(cl:defmethod pose-val ((m <OdometryLite>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader odom_publisher-msg:pose-val is deprecated.  Use odom_publisher-msg:pose instead.")
  (pose m))

(cl:ensure-generic-function 'twist-val :lambda-list '(m))
(cl:defmethod twist-val ((m <OdometryLite>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader odom_publisher-msg:twist-val is deprecated.  Use odom_publisher-msg:twist instead.")
  (twist m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <OdometryLite>) ostream)
  "Serializes a message object of type '<OdometryLite>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'child_frame_id))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'child_frame_id))
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'pose) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'twist) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <OdometryLite>) istream)
  "Deserializes a message object of type '<OdometryLite>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'child_frame_id) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'child_frame_id) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'pose) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'twist) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<OdometryLite>)))
  "Returns string type for a message object of type '<OdometryLite>"
  "odom_publisher/OdometryLite")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'OdometryLite)))
  "Returns string type for a message object of type 'OdometryLite"
  "odom_publisher/OdometryLite")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<OdometryLite>)))
  "Returns md5sum for a message object of type '<OdometryLite>"
  "358519fefe994854542e1fa153efc73a")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'OdometryLite)))
  "Returns md5sum for a message object of type 'OdometryLite"
  "358519fefe994854542e1fa153efc73a")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<OdometryLite>)))
  "Returns full string definition for message of type '<OdometryLite>"
  (cl:format cl:nil "Header header~%string child_frame_id~%geometry_msgs/Pose pose~%geometry_msgs/Twist twist~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.secs: seconds (stamp_secs) since epoch~%# * stamp.nsecs: nanoseconds since stamp_secs~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of postion and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%================================================================================~%MSG: geometry_msgs/Twist~%# This expresses velocity in free space broken into its linear and angular parts.~%Vector3  linear~%Vector3  angular~%~%================================================================================~%MSG: geometry_msgs/Vector3~%# This represents a vector in free space. ~%~%float64 x~%float64 y~%float64 z~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'OdometryLite)))
  "Returns full string definition for message of type 'OdometryLite"
  (cl:format cl:nil "Header header~%string child_frame_id~%geometry_msgs/Pose pose~%geometry_msgs/Twist twist~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.secs: seconds (stamp_secs) since epoch~%# * stamp.nsecs: nanoseconds since stamp_secs~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of postion and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%================================================================================~%MSG: geometry_msgs/Twist~%# This expresses velocity in free space broken into its linear and angular parts.~%Vector3  linear~%Vector3  angular~%~%================================================================================~%MSG: geometry_msgs/Vector3~%# This represents a vector in free space. ~%~%float64 x~%float64 y~%float64 z~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <OdometryLite>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4 (cl:length (cl:slot-value msg 'child_frame_id))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'pose))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'twist))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <OdometryLite>))
  "Converts a ROS message object to a list"
  (cl:list 'OdometryLite
    (cl:cons ':header (header msg))
    (cl:cons ':child_frame_id (child_frame_id msg))
    (cl:cons ':pose (pose msg))
    (cl:cons ':twist (twist msg))
))
