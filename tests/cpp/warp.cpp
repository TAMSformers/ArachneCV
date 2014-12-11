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
  acv::Camera camera1(0, cam_coords, 45, 90, 50);
  while (true)
  {
    camera1.getFrame();
    camera1.warpPerspective();
    camera1.findTargets();
    camera1.showFrame();
  }
  return 0;
}
