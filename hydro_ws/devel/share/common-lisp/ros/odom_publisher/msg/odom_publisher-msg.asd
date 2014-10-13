
(cl:in-package :asdf)

(defsystem "odom_publisher-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "OdometryLite" :depends-on ("_package_OdometryLite"))
    (:file "_package_OdometryLite" :depends-on ("_package"))
  ))