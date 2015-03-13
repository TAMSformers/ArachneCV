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

int main()
{
  double cam_coords[3] = {1, 1, 1};
  acv::WarpCamera camera0(0, cam_coords, 45, 90, 54, 38);
  acv::WarpCamera camera1(1, cam_coords, 45, 90, 54, 38);

  std::vector<acv::Target> targets;
  std::vector<acv::Target> empty;

  camera0.getNext();
  camera1.getNext();

  camera0.findTargets();
  camera1.findTargets();

  camera0.showWarpedBlurredFrame();
  camera1.showWarpedBlurredFrame();

  //targets = acv::mergeTargets(camera0.getTargets(), empty);

  cv::waitKey(0);

  return 0;
}
