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
 * Bin-finding algorithms
 */

#include <opencv2/opencv.hpp>
#include <cmath>

#include "analysis.hpp"
#include "analysis_helper.hpp"
#include "../targets.hpp"

namespace acv {

void find_bins(cv::Mat frame, cv::Mat warp, cv::Mat depth, std::vector<Target> &r_targets, double effective_height, double depth_correction, int hfov, int vfov)
{
  /* dark green bounds for inRange (in HSV color space) */
  int lower_bound[3] = {50, 0, 0};
  int upper_bound[3] = {100, 80, 80};

  /* area sizes at camera position */
  float pix_per_ft_x = (warp.size().width /
                   (2 * effective_height * tan((hfov / 2) * 3.1415 / 180)));
  float pix_per_ft_y = (warp.size().height /
                   (2 * effective_height * tan((vfov / 2) * 3.1415 / 180)));

  /* detect color fields */
  std::vector<std::vector<cv::Point>> all_contours;
  color_fields_in_range(all_contours, warp, lower_bound, upper_bound);

  /* screen for color fields above size threshold */
  std::vector<std::vector<cv::Point>> contours;
  for (int i = 0; i < all_contours.size(); i++) {
    if (cv::contourArea(all_contours[i]) / (pix_per_ft_x * pix_per_ft_y) > .1) {
      contours.push_back(all_contours[i]);
    }
  }

  /* determine geometric characteristics of color fields above size threshold */
  std::vector<cv::Vec3f> centers;
  std::vector<float> heights;
  std::vector<float> widths;

  for (int i = 0; i < contours.size(); i++) {
    cv::Rect br = cv::boundingRect(contours[i]);
    cv::Vec3f center = cv::Vec3f(br.x + br.width / 2, br.y + br.height / 2, 0);
    centers.push_back(center);
    heights.push_back(br.height);
    widths.push_back(br.width);
  }

  /* determine orientation of color fields */
  std::vector<std::string> orientations;
  for (int i = 0; i < contours.size(); i++) {
    //std::cout << heights[i] << std::endl;
    //std::cout << widths[i] << std::endl;
    /* Algorithm won't work with high declination. */
    if (heights[i] > (widths[i] * (1.0 + 0.3 * depth_correction) / ((float)warp.size().width / frame.size().width))) {
      orientations.push_back("up");
    } else {
      orientations.push_back("down");
    }
  }

  /* not used, so empty */
  std::vector<double> angles;

  /* add color fields as targets */
  add_color_fields_as_targets(r_targets, centers, "bin", orientations, angles);

  int debug_color[3] = {0, 0, 255};

  /* determine distance to each target*/
  // Requires centers to be on warp.
  pix_to_ft_warp(r_targets, warp.size(), pix_per_ft_x, pix_per_ft_y);
  annotate_frame(r_targets, warp, debug_color, pix_per_ft_x, pix_per_ft_y);

}

}
