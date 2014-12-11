#
# Arachne CV
#
# Written by David Hashe and Jacob Brunson
#
#
#
# Find balls from file and display.
#

import ArachneCV as acv

camera = acv.Camera(0, [1, 1, 1], 45, 90, 50);

while (True):
    camera.getFrameFromImage("../image.jpg");
    camera.warpPerspective();
    camera.findTargets();
    camera.showFrame();
