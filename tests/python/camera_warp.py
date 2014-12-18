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

camera = acv.WarpCamera(0, [1, 1, 1], 45, 90, 30)

while (True):
    camera.getFrame()
    camera.warpPerspective()
    camera.findTargets()
    camera.showFrame()

    print acv.mergeTargets(camera.getTargets(), [])
