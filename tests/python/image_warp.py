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

if len(sys.argv) != 2:
    print "Usage: image_warp.py [input file]"
    quit()

camera = acv.WarpCamera(0, [0, 0, 5], 45, 90, 54, 38)

while (True):
    camera.getFrameFromImage(sys.argv[1])
    camera.warpPerspective()
    camera.findTargets()
    camera.showFrame()

    print acv.mergeTargets(camera.getTargets(), [])
