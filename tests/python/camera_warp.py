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

camera = acv.Camera(1, [1, 1, 1], 45, 90, 50);
targets = acv.Targets()

while (True):
    camera.getFrame()
    camera.warpPerspective()
    camera.findTargets()
    camera.showFrame()

    targets.merge(camera.targets)
    targets.clear()
