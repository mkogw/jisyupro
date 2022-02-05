#!/usr/bin/env python

import rospy
from geometry_msgs.msg import PoseStamped
from std_msgs.msg import Empty
import numpy as np
from scipy import fftpack

N = 64 

data = np.zeros(N)
time = np.linspace(0,100,num=N) #time (not real timescale)
time_step = time[1]-time[0]

walking = False
threshold_power = 0.5
threshold_freq = 0.01

pub_f = rospy.Publisher('/forward', Empty, queue_size=1)
pub_b = rospy.Publisher('/back', Empty, queue_size=1)
pub_l = rospy.Publisher('/TTL', Empty, queue_size=1)
pub_r = rospy.Publisher('/TTR', Empty, queue_size=1)
pub_s = rospy.Publisher('/stop', Empty, queue_size=1)

def cb(msg):
    global data, walking
    z = msg.pose.position.z
    data[:N-1] = data[1:] #dequeue
    data[-1] = z #enqueue

    #FFT
    sample_freq = fftpack.fftfreq(data[:].size, d=time_step)
    y_fft = fftpack.fft(data[:])
    pidxs = np.where(sample_freq>0)
    freqs, power = sample_freq[pidxs], np.abs(y_fft)[pidxs]
    freq = freqs[power.argmax()]
    power_max = power.max()

    if power_max>threshold_power and freq>threshold_freq:
        pub_f.publish()
        if not walking:
            walking = True
            print("walking!!")
    else:
        #pub_s.publish() # this is only for when not using head2control.py
        if walking:
            walking = False
            print("stopping")



rospy.init_node('walk2control')
rospy.Subscriber('/oculus/headset/pose', PoseStamped, cb)

rate = rospy.Rate(10)

rospy.spin()
rate.sleep()
