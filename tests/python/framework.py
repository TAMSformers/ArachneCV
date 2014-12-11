#
# Arachne CV
#
# Written by David Hashe and Jacob Brunson
#
#
#
#
# Currently unfinished. Don't actually run this one.
#

import ArachneCV as acv

camera0 = acv.Camera(0, [-1, 0, 2], 45, 90, 50);
camera1 = acv.Camera(0, [1, 0, 2], 45, 90, 50);

targets = acv.Targets()

while (True):
    camera0.getFrame()
    camera1.getFrame()

    camera0.warpPerspective()
    camera1.warpPerspective()

    camera0.findTargets()
    camera1.findTargets()

    targets.merge(camera0.targets)
    targets.merge(camera1.targets)

