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

  double cam_coords[3] = {0, 0, 5};
  acv::WarpCamera camera(0, cam_coords, 45, 0, 54, 38);

  std::vector<acv::Target> targets;
  std::vector<acv::Target> empty;

  camera.getNextFromImage(argv[1]);
  camera.findTargets();
  camera.showWarpedBlurredFrame();

  //targets = acv::mergeTargets(camera.getTargets(), empty);

  cv::waitKey(0);

  return 0;
}
