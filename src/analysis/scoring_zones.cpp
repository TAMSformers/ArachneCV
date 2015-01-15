/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * Scoring zone-finding algorithms
 *
 */

#include <opencv2/opencv.hpp>
#include <cmath>

#include "analysis.hpp"
#include "analysis_helper.hpp"
#include "../targets.hpp"

namespace acv {

void find_scoring_zones(cv::Mat frame, cv::Mat warp, cv::Mat depth, std::vector<Target> &r_targets, double effective_height, double depth_correction, int hfov, int vfov)
{
  /* area sizes at camera position */
  float pix_per_ft_x = fov_to_pix_per_ft(warp.size().width, effective_height, hfov, vfov);
  float pix_per_ft_y = fov_to_pix_per_ft(warp.size().height, effective_height, hfov, vfov);

  /* white bounds for inRange (in HSV color space) */
  int lower_bound[3] = {0, 0, 220};
  int upper_bound[3] = {255, 60, 255};

  /* detect color fields */
  std::vector<std::vector<cv::Point>> all_contours;
  color_fields_in_range(all_contours, warp, lower_bound, upper_bound);

  /* filter out color fields below size threshold */
  std::vector<std::vector<cv::Point>> contours;
  filter_color_fields(contours, all_contours, pix_per_ft_x, pix_per_ft_y, 4);

  /* determine geometric characteristics of color fields above size threshold */

  /* determine angles */
  std::vector<double> angles;

  for (int i = 0; i < contours.size(); i++) {

    /* find two farthest apart points in contour */
    cv::Point P1;
    cv::Point P2;
    double max_distance = 0;
    for (int j = 0; j < contours[i].size(); j++) {
      for (int k = 0; k < contours[i].size(); k++) {
        if (sqrt(pow(contours[i][k].x - contours[i][j].x, 2) +
                 pow(contours[i][k].y - contours[i][j].y, 2)) >
            max_distance) {
          P1 = contours[i][j];
          P2 = contours[i][k];
          max_distance = sqrt(pow(contours[i][k].x - contours[i][j].x, 2) +
                              pow(contours[i][k].y - contours[i][j].y, 2));
        }
      }
    }

    /* determine angle noodle is lying at relative to the camera. 0 degrees
       is horizontal facing right, 90 degrees is vertical, 180 is horizontal
       facing left */
    int x_distance = P2.x - P1.x;
    int y_distance = P2.y - P1.y;
    double angle = atan(abs(y_distance) / abs(x_distance)) * 180 / 3.1415;
    if ((x_distance > 0) && (y_distance < 0) ||
        (x_distance < 0) && (y_distance > 0)) {
      angle = 180 - angle;
    }
    angles.push_back(angle);
  }

  /* determine centers */
  std::vector<cv::Vec3f> centers;
  find_centers_of_color_fields(centers, contours);

  /* not used so empty */
  std::vector<std::string> orientations;

  add_color_fields_as_targets(r_targets, centers, "scoring zone", orientations, angles);

  int debug_color[3] = {255, 255, 255};

  /* determine distance to each target*/
  if (!depth.empty()) {
    pix_to_ft_depth(r_targets, depth, depth_correction, pix_per_ft_x, pix_per_ft_y);
    annotate_frame(r_targets, warp, debug_color, pix_per_ft_x, pix_per_ft_y);
  } else {
    // Requires centers to be on warp.
    pix_to_ft_warp(r_targets, warp.size(), pix_per_ft_x, pix_per_ft_y);
    annotate_frame(r_targets, warp, debug_color, pix_per_ft_x, pix_per_ft_y);
  }

}

}
