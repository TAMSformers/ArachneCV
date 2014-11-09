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
  int cam_coords[3];
  acv::Camera camera1(1, cam_coords, 45);
  while (true)
  {
    camera1.getFrame();
    camera1.warpPerspective();
    camera1.findBalls();
    camera1.showFrame();
  }
  return 0;
}
