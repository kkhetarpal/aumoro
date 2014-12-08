aumoro
============

IMDL Fall 2014
This repository comprises of all software written for 
Arduino Mega/Due: Obstacle Avoidance
                  Sensors Simulation Working Algorithm
                  Testing Motors
                  Predemoday
Vision: Object Tracking
        Human Detection
        Human Tracking
============
v1.3
This version of tracking performs hue based tracking :: the source code can be run using the mouse selection of color to be tracked. The tracking talks serially to Arduino Due and sends character commands : f/b/r/l/s. 

============
v1.4
This version of tracking performs hue based tracking :: the source code has been made automatic :: pre feed an image and store it with the name img2 in the given location. The selection is based on the hue extracted from that image. 

============
predemoday
The Arduino receives data serially from the laptop and in accordance to the data received it moves forward, backward, turns left/right. For each of these commands, a different color led glows in the robot for feedback. 

============
