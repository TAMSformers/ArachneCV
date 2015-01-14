/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * Tote-finding algorithms
 *
 */

#include <opencv2/opencv.hpp>
#include <cmath>

#include "analysis.hpp"
#include "analysis_helper.hpp"
#include "targets.hpp"

namespace acv {

// TODO All of this is broken.
void find_totes(cv::Mat frame, cv::Mat warp, cv::Mat depth, std::vector<Target> &r_targets, double effective_height, int hfov, int vfov);

void distance_totes_color(std::vector<Target> &r_targets, cv::Mat frame, float pix_per_ft_x, float pix_per_ft_y, int lower_bound[3], int upper_bound[3]);

void distance_totes_depth(std::vector<Target> &r_targets, cv::Mat depth, int hfov, int vfov);


void find_yellow_totes(cv::Mat frame, cv::Mat warp, cv::Mat depth, std::vector<Target> &r_targets, double effective_height, int hfov, int vfov)
{
  /* yellow bounds for inRange (in HSV color space) */
  int lower_bound[3] = {30, 120, 120};
  int upper_bound[3] = {70, 255, 255};

  find_totes(frame, warp, depth, r_targets, effective_height, hfov, vfov, lower_bound, upper_bound);
}

void find_gray_totes(cv::Mat frame, cv::Mat warp, cv::Mat depth, std::vector<Target> &r_targets, double effective_height, int hfov, int vfov)
{
  /* gray bounds for inRange (in HSV color space) */
  int lower_bound[3] = {30, 120, 120};
  int upper_bound[3] = {70, 255, 255};

  find_totes(frame, warp, depth, r_targets, effective_height, hfov, vfov, lower_bound, upper_bound);
}

void find_totes(cv::Mat frame, cv::Mat warp, cv::Mat depth, std::vector<Target> &r_targets, double effective_height, int hfov, int vfov, int lower_bound[3], int upper_bound[3])
{
  /* area sizes at camera position */
  float pix_per_ft_x = fov_to_pix_per_ft(warp.size().width, effective_height, hfov, vfov);
  float pix_per_ft_y = fov_to_pix_per_ft(warp.size().height, effective_height, hfov, vfov);

  /* detect color fields */
  std::vector<std::vector<cv::Point>> all_contours;
  color_fields_in_range(all_contours, warp, lower_bound, upper_bound);

  /* filter out color fields below size threshold */
  std::vector<std::vector<cv::Point>> contours;
  filter_color_fields(contours, all_contours, pix_per_ft_x, pix_per_ft_y, 0.1);

  /* determine geometric characteristics of color fields above size threshold */

  /* determine angles */
  std::vector<double> angles;

  for (int i = 0; i < contours.size(); i++) {

    /* find two farthest apart points in contour */
    cv::Point P1;
    cv::Point P2;
    double max_distance = 0;
    for (int j = 0; i < contours[i].size(); x++) {
      for (int k = 0; y < contours[i].size(); y++) {
        if (sqrt(pow(contours[i][k].x - contours[i][j].x, 2) +
                 pow(contours[i][k].y - contours[i][j].y, 2)) >
            max_distance) {
          P1 = contours[i][j];
          P2 = contours[i][k];
          max_distance = sqrt(pow(contours[i][k].x - contours[i][j].x, 2) +
                              pow(contours[i][k].y - contours[i][j].y, 2);
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

  add_color_fields_as_targets(r_targets, centers, std::vector<std::string>(), angles);

  /* determine distance to each target*/
  if (!depth.empty()) {
    distance_totes_depth(r_targets, depth, hfov, vfov);
  } else {
    distance_totes_color(r_targets, warp, pix_per_ft_x, pix_per_ft_y);
  }
}

void distance_totes_color(std::vector<Target> &r_targets, cv::Mat frame, float pix_per_ft_x, float pix_per_ft_y)
{
  pix_to_ft_warp(r_targets, warp, pix_per_ft_x, pix_per_ft_y);
}

void distance_totes_depth(std::vector<Target> &r_targets, cv::Mat depth, int hfov, int vfov)
{
  pix_to_ft_depth(r_targets, depth, hfov, vfov);
}

}
