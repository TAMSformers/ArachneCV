/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * Currently unfinished. Don't actually run this one.
 *
 */

#include <ArachneCV.hpp>

int main()
{
  double cam_coords0[3] = {-1, 0, 2};
  double cam_coords1[3] = {1, 0, 2};

  acv::Camera camera0(0, cam_coords0, 45, 90, 50);
  acv::Camera camera1(1, cam_coords1, 45, 90, 50);

  acv::Targets targets;

  while (true) {
    camera0.getFrame();
    camera1.getFrame();

    // Required for findBalls and findRobots to return correct value, so merge into getFrame?
    camera0.warpPerspective();
    camera1.warpPerspective();

    camera0.findTargets();
    camera1.findTargets();

    targets.merge(camera0.targets);
    targets.merge(camera1.targets);

    targets.clear();
  }

  return 0;
}



