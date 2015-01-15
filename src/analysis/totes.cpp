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

void find_totes(cv::Mat frame, cv::Mat warp, cv::Mat depth, std::vector<Target> &r_targets, double effective_height, double depth_correction, int hfov, int vfov, int lower_bound[3], int upper_bound[3], std::string type)
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

  /* determine orientations */
  std::vector<std::string> orientations;

  /* determine angles */
  std::vector<double> angles;

  /* determine centers */
  std::vector<cv::Vec3f> centers;
  find_centers_of_color_fields(centers, contours);

  add_color_fields_as_targets(r_targets, centers, type, orientations, angles);

  /* determine distance to each target*/
  if (!depth.empty()) {
    pix_to_ft_depth(r_targets, depth, depth_correction, pix_per_ft_x, pix_per_ft_y);
  } else {
    pix_to_ft_warp(r_targets, warp.size(), pix_per_ft_x, pix_per_ft_y);
  }
}

void find_yellow_totes(cv::Mat frame, cv::Mat warp, cv::Mat depth, std::vector<Target> &r_targets, double effective_height, double depth_correction, int hfov, int vfov)
{
  /* yellow bounds for inRange (in HSV color space) */
  int lower_bound[3] = {30, 120, 120};
  int upper_bound[3] = {70, 255, 255};
  std::string type = "yellow tote";

  find_totes(frame, warp, depth, r_targets, effective_height, depth_correction, hfov, vfov, lower_bound, upper_bound, type);
}

void find_gray_totes(cv::Mat frame, cv::Mat warp, cv::Mat depth, std::vector<Target> &r_targets, double effective_height, double depth_correction, int hfov, int vfov)
{
  /* gray bounds for inRange (in HSV color space) */
  int lower_bound[3] = {0, 0, 40};
  int upper_bound[3] = {18, 80, 180};
  std::string type = "gray tote";

  find_totes(frame, warp, depth, r_targets, effective_height, depth_correction, hfov, vfov, lower_bound, upper_bound, type);
}

}
