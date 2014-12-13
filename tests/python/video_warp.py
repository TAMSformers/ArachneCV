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

camera = acv.Camera("../media/video0.mp4", [1, 1, 1], 30, 90, 50);

while (True):
    camera.getFrame();
#    camera.warpPerspective();
    camera.findTargets();
    camera.showFrame();
