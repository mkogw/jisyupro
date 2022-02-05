#!/usr/bin/env python

import rospy
import cv2
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import Image
from sensor_msgs.msg import CompressedImage
import mmap
import numpy as np

rospy.init_node('theta_publisher')

bridge = CvBridge()
pub = rospy.Publisher('/theta/compressed_image', CompressedImage, queue_size=1)
frame_shape = (960, 1920, 3)

def main():

    while True:
        with open("image.bin", "r+b") as f:
            mm = mmap.mmap(f.fileno(), 0)
            mm.seek(0)
            frame_b = f.read()

        frame = np.frombuffer(frame_b, dtype=np.uint8).reshape(frame_shape)

        pub.publish(bridge.cv2_to_compressed_imgmsg(frame))


if __name__=='__main__':
    main()