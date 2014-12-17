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

#include "../../src/ArachneCV.hpp"

int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cout << "Usage: image_warp [input file]" << std::endl;
    return 0;
  }

  double cam_coords[3] = {0, 0, 1};
  acv::WarpCamera camera(0, cam_coords, 45, 90, 50);
  acv::Targets targets;
  while (true)
  {
    camera.getFrameFromImage(argv[1]);
    camera.warpPerspective();
    camera.findTargets();
    camera.showFrame();

    targets.merge(camera.getTargets());
    targets.clear();
  }
  return 0;
}
