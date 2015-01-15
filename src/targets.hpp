/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * This file implements object locations.
 *
 */

#ifndef __ARACHNECV_TARGETS__
#define __ARACHNECV_TARGETS__

#include <string>

#include <opencv2/opencv.hpp>

namespace acv {

/**
 * Anything we need to track.
 */
typedef struct
{
  /**
   * Identity of target.
   */
  std::string type;

  /**
   * Coordinates in 3D space relative to the center of the robot.
   */
  //cv::Vec3f coords;
  double coords[3];

  /**
   * Angle in degrees from horizontal from front of robot.
   */
  double angle;

  /**
   * Orientation;
   */
  std::string orientation;
} Target;

/**
 * Return union of two input sets, with duplicates and near duplicates removed.
 *
 * @param[in] targets_1 First set of targets to be merged.
 * @param[in] targets_2 Second set of targets to be merged.
 */
std::vector<Target> mergeTargets(std::vector<Target> targets_1,
                          std::vector<Target> targets_2);

}

#endif
