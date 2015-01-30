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
 * Object-finding algorithms
 */

#ifndef __ARACHNECV_IMAGE_ANALYSIS__
#define __ARACHNECV_IMAGE_ANALYSIS__

#include <opencv2/opencv.hpp>

#include "../targets.hpp"

namespace acv {

void find_bins(cv::Mat frame, cv::Mat warp, cv::Mat depth,
               std::vector<Target> &r_targets, double effective_height,
               double depth_correction, int hfov, int vfov);

void find_noodles(cv::Mat frame, cv::Mat warp, cv::Mat depth,
               std::vector<Target> &r_targets, double effective_height,
               double depth_correction, int hfov, int vfov);

void find_scoring_zones(cv::Mat frame, cv::Mat warp, cv::Mat depth,
               std::vector<Target> &r_targets, double effective_height,
               double depth_correction, int hfov, int vfov);

void find_yellow_totes(cv::Mat frame, cv::Mat warp, cv::Mat depth,
               std::vector<Target> &r_targets, double effective_height,
               double depth_correction, int hfov, int vfov);

void find_gray_totes(cv::Mat frame, cv::Mat warp, cv::Mat depth,
               std::vector<Target> &r_targets, double effective_height,
               double depth_correction, int hfov, int vfov);

}

#endif
