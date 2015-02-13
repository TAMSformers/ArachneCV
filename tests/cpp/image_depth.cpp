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
    std::cout << "Usage: image_depth [input file]" << std::endl;
    return 0;
  }

  double cam_coords[3] = {0, 0, 15};
  acv::DepthCamera camera(0, cam_coords, 30, 0, 54, 38);

  std::vector<acv::Target> targets;
  std::vector<acv::Target> empty;

  std::vector<cv::Point> points;
  points.push_back(cv::Point(320, 160));
  points.push_back(cv::Point(320, 300));

  //for (int i = 0; i < 50; i++)
  while (true)
  {
    camera.getFrameFromImage(argv[1]);
    camera.watershed(points);
    camera.showFrame();

    //targets = acv::mergeTargets(camera.getTargets(), empty);
  }
  return 0;
}
