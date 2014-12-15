#
# Arachne CV
#
# Written by David Hashe and Jacob Brunson
#
#
#
#
# Running multiple webcams might require some hardware configuration to avoid
# 'device full' errors.
#

import ArachneCV as acv

camera0 = acv.Camera(0, [-1, 0, 2], 45, 90, 50);
camera1 = acv.Camera(1, [1, 0, 2], 45, 90, 50);

targets = acv.Targets()

while (True):
    camera0.getFrame()
    camera1.getFrame()

    camera0.warpPerspective()
    camera1.warpPerspective()

    camera0.findTargets()
    camera1.findTargets()

    camera0.showFrame()
    camera1.showFrame()

    targets.merge(camera0.getTargets())
    targets.merge(camera1.getTargets())

    targets.clear()
