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
import sys

if len(sys.argv) != 2:
    print "Usage: video_warp.py [input file]"
    quit()

camera = acv.WarpCamera(sys.argv[1], [1, 1, 25], 45, 90, 50)
targets = acv.Targets()

while (True):
    camera.getFrame()
    camera.warpPerspective()
    camera.findTargets()
    camera.showFrame()

    targets.merge(camera.getTargets())
    targets.clear()
