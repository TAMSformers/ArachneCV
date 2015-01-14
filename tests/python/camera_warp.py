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

camera = acv.WarpCamera(0, [1, 1, 1], 45, 90, 54, 38)

while (True):
    camera.getFrame()
    camera.cvtAndBlur()
    camera.warpPerspective()
    camera.findTargets()
    camera.showFrame()

    print acv.mergeTargets(camera.getTargets(), [])
