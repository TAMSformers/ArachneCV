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
  acv::WarpCamera camera0(0, cam_coords, 45, 90, 50);

  std::vector<acv::Target> targets;
  std::vector<acv::Target> empty;

  while (true)
  {
    camera0.getFrame();
    camera0.warpPerspective();
    camera0.findTargets();
    camera0.showFrame();

    targets = acv::mergeTargets(camera0.getTargets(), empty);
  }
  return 0;
}
