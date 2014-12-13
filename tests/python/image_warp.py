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

camera = acv.Camera(0, [0, 0, 4], 45, 90, 30);

while (True):
    camera.getFrameFromImage("../media/image.jpg");
    camera.warpPerspective();
    camera.findTargets();
    camera.showFrame();
