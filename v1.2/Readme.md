This version of the vision tracking algorithm has improvised over the
last version in a way that takes care of the tracking in a more robust
manner. 

The algorithm takes into account : the previous frames trakcer window
in prevtrackWindow, and the current trackWindow. The change in the
area of the tracker window helps us decide to move forward or
backward. Since, as the object moves further away, the area of the
tracker window decreases in a ideal setting, and vice versa.

Decision to move right or left is based on the centroid of the window
in two consequitive frames. Centroid x and y coordinates are
calculated from the top-left and bottom-right points of the tracker
window.