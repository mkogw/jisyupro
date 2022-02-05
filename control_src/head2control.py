#!/usr/bin/env python

# robot will follow the direction of the headset

import numpy as np
import rospy
from geometry_msgs.msg import PoseStamped
from std_msgs.msg import Empty
from std_msgs.msg import String
from std_msgs.msg import Int8
import tf
import time

N = 36

pub = rospy.Publisher('/location', Int8, queue_size=1)

def quaternion_to_euler(quaternion):
    e = tf.transformations.euler_from_quaternion((quaternion.x, quaternion.y, quaternion.z, quaternion.w))
    return e

def cb(msg):
    ang = quaternion_to_euler(msg.pose.orientation)
    z = ang[2] # -pi to pi / left z>0 / right z<0

    z += np.pi/N
    if z<0:
        z += 2*np.pi #<- 0-360

    location = Int8(int(z/2/np.pi*N))
    pub.publish(location)

    
def main():
    rospy.init_node('head2control')
    rospy.Subscriber('/oculus/headset/pose', PoseStamped, cb)

    rate = rospy.Rate(10) # Hz

    rospy.spin()
    rate.sleep()

if __name__=='__main__':
    main()