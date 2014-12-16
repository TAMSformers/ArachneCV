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
import sys

camera = acv.Camera(0, [0, 0, 4], 45, 90, 30)
targets = acv.Targets()

while (True):
    camera.getFrameFromImage(sys.argv[1])
    camera.warpPerspective()
    camera.findTargets()
    camera.showFrame()

    targets.merge(camera.getTargets())
    targets.clear()
