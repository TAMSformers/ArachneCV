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
 * Depth tote-finding algorithms
 */

#include <opencv2/opencv.hpp>
#include <cmath>

#include "analysis.hpp"
#include "analysis_helper.hpp"
#include "../targets.hpp"

namespace acv {

// Takes color image and set of points provided by Python, outputs relative
// position and orientation
void find_totes_depth(cv::Mat frame, cv::Mat depth, std::vector<Target> &r_targets, std::vector<cv::Point> points, double effective_height, double depth_correction, int hfov, int vfov)
{
  // TODO Separate image into depth layers and discard bottom layer.
  /*for (int i = 0; i < depth.size().width; i++) {
    for (int j = 0; j < depth.size().height; j++) {
      uchar tmp = int(depth.at<uchar>(i,j) - round((depth.size().height-j)*(CONSTANT/depth.size().height)))
      if tmp >= 220:
        depth[y, x] = 0
      elif tmp >= 208 and tmp < 220:
        depth[y, x] = 85
      elif tmp >= 180 and tmp < 208:
        depth[y, x] = 170
      else:
       depth[y, x] = 255
    }
  }
    return depth*/


  // TODO AND depth with color.
  // TODO Apply watershed to color.
  // TODO Get level for each tote.
  // TODO Get orientation for each tote. <-- Both might go into orientation.

  int height = depth.size().height;
  int width = depth.size().width;
  cv::Mat markers(height, width, CV_32F);
  //cv::Mat markers_vis(markers);
  for (int i = 0; i < points.size(); i++) {
      cv::Scalar color(i, i*100, i);
      //cv::circle(markers_vis, points[i], 1, color, 20);
      cv::circle(markers, points[i], 1, color, 20);
  }
  cv::watershed(frame, markers);
}

}
