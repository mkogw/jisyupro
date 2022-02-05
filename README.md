# Alice in VRland   - a small Telexistence Robot with Oculus Quest2 -

## What you can
- experience bigger world with your VR headset, like when Alice’s body shrank by drinking something in “Alice in Wonderland”
- look around the world (the robot will follow your head’s rotation)
- walk with your own foot (you need not move, but just step on the spot in order to move the robot forward)


## What you need 
- Ubuntu 18.04 (with ROS melodic)
- Unity 2019.4.33f1
- Wi-fi (IEEE 802.11n)
- Oculus Quest2
- RICOH THETA Z1
- mini-robot  (with 2 wheels and motors, ESP32-DevKitC-32E, motor-driver and batteries)
<img src="https://github.com/mkogw/jisyupro/blob/main/image/robot.png" width="20%">

## How to get ready

＜oculus quest2 and unity＞
1. open unity project “AliceInVRland”
2. change IP address (in object "RosConnector") to the address of your PC(Ubuntu)
3. build the project toward your oculus quest2

＜RICOH THETA Z1>
1. install rtsp plug-in to your THETA from [here](https://pluginstore.theta360.com/plugins/com.sciencearts.rtspstreaming/)
2. launch the plug-in and check your THETA’s IP address via THETA’s app
3. change IP address in "/cam_src/mmap_rtsp_theta_publisher.py"

＜mini-robot＞
1. connect the pins properly to make the circuit (you can check which pins are used to control each wheel in "/sketch/ESP32_src")
2. write sketch program into your ESP32-DevKitC-32E (note that you’ll have to change IP address and Wi-fi settings to yours)



## How to run

1. launch THETA’s rtsp plug-in via THETA’s app
2. launch all of the programs on your PC as follows
  - `roslaunch rosbridge_server rosbridge_websocket.launch`  
  - `python3 mmap_rtsp_theta_capture.py`
  - `python mmap_rtsp_theta_publisher.py`
  - `rosrun rosserial_python serial_node.py tcp`
  - `python head2control.py`
  - `python walk2control.py`

※if your PC does not have some necessities, please download them before you run  
ex) install rosbridge_server by `sudo apt-get install ros-melodic-rosbridge-server`

3. launch unity project in your Oculus Quest2 (if the connection succeeds, you can see the image from THETA)
4. launch the robot

**Now you can go into the world of “Alice in VRland”!**
