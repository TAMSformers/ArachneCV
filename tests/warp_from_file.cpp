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
  int cam_coords[3];
  acv::Camera camera1(0, cam_coords, 45);
  while (true)
  {
    camera1.getFrameFromImage("image.jpg");
    camera1.warpPerspective();
    camera1.findBalls();
    camera1.showFrame();
  }
  return 0;
}
