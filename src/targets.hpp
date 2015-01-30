/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 * This is ArachneCV, a computer vision library for the FIRST        *
 * Robotics Competition. Source hosted at                            *
 * https://github.com/tamsformers/ArachneCV                          *
 *                                                                   *
 * Copyright 2014, 2015 by David Hashe and Jacob Brunson.            *
 *                                                                   *
 * This program is free software; you can redistribute it and/or     *
 * modify it under the terms of the GNU General Public License as    *
 * published by the Free Software Foundation - version 3 or          *
 * (at your option) any later version.                               *
 *                                                                   *
 * This program is distributed in the hope that it will be useful,   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     *
 * GNU General Public License in file COPYING for more details.      *
 *                                                                   *
 * You should have received a copy of the GNU General Public         *
 * License along with this program; if not, write to the Free        *
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,       *
 * Boston, MA 02111, USA.                                            *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * This file implements object locations.
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
