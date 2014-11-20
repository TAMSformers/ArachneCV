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

camera0 = acv.Camera($params)
camera1 = acv.Camera($params)

targets = acv.Targets()

while (True):
    camera0.getFrame()
    camera1.getFrame()

    camera0.warpPerspective()
    camera1.warpPerspective()

    camera0.findBalls()
    camera1.findBalls()

    camera0.findRobots()
    camera1.findRobots()

    targets.merge(camera0.targets)
    targets.merge(camera1.targets)

