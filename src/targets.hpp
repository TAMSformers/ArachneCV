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
 * A ball, robot, etc.
 */
typedef struct
{
  /**
   * Identity of target. Usually either robot or ball.
   */
  std::string type;

  /**
   * Either red or blue.
   */
  std::string color;

  /**
   * Coordinates in 3D space relative to the center of the robot.
   */
  double coords[3];

  /**
   * Used within image_analysis to mark false positives.
   */
  bool is_real = true;
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
