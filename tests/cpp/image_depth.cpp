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
  if (argc != 3) {
    std::cout << "Usage: image_depth [color file] [depth file]" << std::endl;
    return 0;
  }

  double cam_coords[3] = {0, 0, 15};
  acv::DepthCamera camera(0, cam_coords, 30, 0, 54, 38);

  std::vector<acv::Target> targets;
  std::vector<acv::Target> empty;

  std::vector<cv::Point> points;
  points.push_back(cv::Point(320, 160));
  points.push_back(cv::Point(320, 300));

  camera.getNextFromImage(argv[1], argv[2]);
  camera.findTargets(points);
  camera.showColor();
  camera.showDepth();

  //targets = acv::mergeTargets(camera.getTargets(), empty);

  cv::waitKey(0);

  return 0;
}
