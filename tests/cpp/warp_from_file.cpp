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
  acv::Camera camera1(0, cam_coords, 45, 90, 50);
  while (true)
  {
    camera1.getFrameFromImage("../image.jpg");
    camera1.warpPerspective();
    camera1.findBalls();
    camera1.showFrame();
  }
  return 0;
}
