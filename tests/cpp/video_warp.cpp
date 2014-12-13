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
  acv::Camera camera0("../media/video0.mp4", cam_coords, 45, 90, 50);
  while (true)
  {
    camera0.getFrame();
    camera0.warpPerspective();
    camera0.findTargets();
    camera0.showFrame();
  }
  return 0;
}
