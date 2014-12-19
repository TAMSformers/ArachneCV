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

#include "../../src/ArachneCV.hpp"

int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cout << "Usage: video_warp [input file]" << std::endl;
    return 0;
  }

  double cam_coords[3] = {0, 0, 25};
  acv::WarpCamera camera(argv[1], cam_coords, 45, 90, 54, 38);

  std::vector<acv::Target> targets;
  std::vector<acv::Target> empty;

  while (true)
  {
    camera.getFrame();
    camera.warpPerspective();
    camera.findTargets();
    camera.showFrame();

    targets = acv::mergeTargets(camera.getTargets(), empty);
  }
  return 0;
}
