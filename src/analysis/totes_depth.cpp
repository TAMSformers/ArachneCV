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
#include <string>

#include "analysis.hpp"
#include "analysis_helper.hpp"
#include "../targets.hpp"

namespace acv {

// Takes color image and set of points provided by Python, outputs relative
// position and orientation
void find_totes_depth(cv::Mat color, cv::Mat depth, std::vector<Target> &r_targets, std::vector<cv::Point> points, double effective_height, double depth_correction, int hfov, int vfov)
{
  /* Separate image into depth layers and discard bottom layer */
  cv::Mat levels(depth);
  levels.convertTo(levels, CV_8UC1);
  for (int i = 0; i < levels.size().width; i++) {
    for (int j = 0; j < levels.size().height; j++) {
      uchar tmp = int(levels.at<uchar>(i,j) - round((depth.size().height-j)*(depth_correction/depth.size().height)));
      if (tmp >= 220) {
        levels.at<uchar>(i,j) = 0;
      } else if (tmp >= 208 && tmp < 220) {
        levels.at<uchar>(i,j) = 1;
      } else if (tmp >= 180 && tmp < 208) {
        levels.at<uchar>(i,j) = 2;
      } else {
        levels.at<uchar>(i,j) = 3;
      }
    }
  }
  /* Designate one point in the background at level 0 as a point so that the algorithm works */
  bool done = false;
  for (int i = 0; i < levels.size().width && !done; i++) {
    for (int j = 0; j < levels.size().height && !done; j++) {
      if (levels.at<uchar>(i,j) == 0) {
        points.push_back(cv::Point(i,j));
        done = true;
      }
    }
  }

  /* Discard bottom level of color image */
  cv::Mat processed;
  cv::bitwise_and(color, color, processed, levels);

  /* Apply watershed to color */
  int height = depth.size().height;
  int width = depth.size().width;
  cv::Mat markers(height, width, CV_32F);
  for (int i = 0; i < points.size(); i++) {
      cv::Scalar color(i, i*100, i);
      cv::circle(markers, points[i], 1, color, 20);
  }
  cv::watershed(color, markers);

  for (int i = 0; i < points.size()-1 /* all but background point */; i++) {
    acv::Target target;
    target.type = "kinect tote";
    // Set level (orientation) for tote.
    target.orientation = std::to_string(levels.at<uchar>(points[i].x,points[i].y));
    // Set angle for each tote.
    /* TODO Build std::vector<cv::Point> containing all points within tote */
    
    //cv::Rect min_rect = cv::minAreaRect(
    /* TODO Take bottom left and right points and get angle of tote from trig */

    // TODO Set coords with:
    // int fov_to_pix_per_ft(int frame_dim, double effective_height, int hfov, int vfov);
  }
}

}
