/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * Main test. Temporary; we need a testing suite.
 *
 */

#include <ArachneCV/ArachneCV.hpp>

int main()
{
  double cam_coords[3] = {0, 0, 1};
  acv::Camera camera(0, cam_coords, 45, 90, 50);
  acv::Targets targets();
  while (true)
  {
    camera.getFrameFromImage("../media/image.jpg");
    camera.warpPerspective();
    camera.findTargets();
    camera.showFrame();

    targets.merge(camera.targets);
    targets.clear();
  }
  return 0;
}
