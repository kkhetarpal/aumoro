This version of the vision tracking algorithm has improvised over the
last version in a way that takes care of the tracking in a more robust
manner. 

Decision to move forward or backward is based on the area occupied by the 
tracker window in a frame. If the area occupied is only 10% (say), the human is further
away from the robot and robot moves further.

Decision to move right or left is based on the centroid of the window
in two consequitive frames. Centroid x and y coordinates are
calculated from the top-left and bottom-right points of the tracker
window.

This was the version demonstrated in the pre demo day for IMDL FALL 2014 class.
