/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * Find balls and display.
 *
 */

#include <ArachneCV/ArachneCV.hpp>

int main()
{
  double cam_coords[3] = {1, 1, 1};
  acv::Camera camera("../media/video0.mp4", cam_coords, 45, 90, 50);
  acv::Targets targets();
  while (true)
  {
    camera.getFrame();
    camera.warpPerspective();
    camera.findTargets();
    camera.showFrame();

    targets.merge(camera.targets);
    targets.clear();
  }
  return 0;
}
