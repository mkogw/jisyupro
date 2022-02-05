#!/usr/bin/env python

import cv2
import os
import mmap

ipaddress = "192.168.100.33" #change to your THETA's IP address
resolution = "1920x960" # 640x320 1024x512 1920x960 3840x1920

url = 'rtsp://' + ipaddress + ':8554/live?resolution=' + resolution

#os.environ["OPENCV_FFMPEG_CAPTURE_OPTIONS"] = "rtsp_transport;0"
os.environ["OPENCV_FFMPEG_CAPTURE_OPTIONS"] = "rtsp_transport;udp"

cap = cv2.VideoCapture(url)
ret, frame = cap.read()
image_size = frame.size
print(frame.shape) #(960, 1920, 3)


with open("image.bin", "r+b") as f:
    f.write(frame.tobytes())

if not cap.isOpened():
    raise IOError("Cannot open webcam")

while(1):
    ret, frame = cap.read()
    with open("image.bin", "r+b") as f:
        mm = mmap.mmap(f.fileno(), image_size)
        mm[:] = frame.tobytes()


cap.release()
cv2.destroyAllWindows()