#
# Arachne CV
#
# Written by David Hashe and Jacob Brunson
#
#
#
# Find balls and display.
#

import ArachneCV as acv

camera = acv.Camera(0, [1, 1, 1], 45, 90, 50);

while (True):
    camera.getFrame();
    camera.warpPerspective();
    camera.findBalls();
    camera.showFrame();
