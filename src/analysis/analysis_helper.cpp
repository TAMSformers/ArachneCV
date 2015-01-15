/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * Object-finding algorithms
 *
 */

#include <opencv2/opencv.hpp>
#include <cmath>

#include "analysis_helper.hpp"
#include "targets.hpp"

namespace acv {

int fov_to_pix_per_ft(int frame_dim, double effective_height, int hfov, int vfov)
{
  return frame_dim / (2 * effective_height * tan((hfov / 2) * 3.1415 / 180));
}

void color_fields_in_range(std::vector<std::vector<cv::Point>> &r_contours, cv::Mat frame, int lower_bound[3], int upper_bound[3])
{
  std::vector<int> lower_bound_vec(lower_bound, lower_bound + 3);
  std::vector<int> upper_bound_vec(upper_bound, upper_bound + 3);

  /* create mask */
  cv::Mat mask;
  cv::inRange(frame, lower_bound_vec, upper_bound_vec, mask);

  cv::findContours(mask, r_contours, cv::noArray(), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
}

void filter_color_fields(std::vector<std::vector<cv::Point>> &r_contours, std::vector<std::vector<cv::Point>> &r_all_contours, float pix_per_ft_x, float pix_per_ft_y, float threshold_ft)
{
  for (int i = 0; i < r_all_contours.size(); i++) {
    if (cv::contourArea(r_all_contours[i]) / (pix_per_ft_x * pix_per_ft_y) >
        threshold_ft) {
      r_contours.push_back(r_all_contours[i]);
    }
  }
}

void find_centers_of_color_fields(std::vector<cv::Vec3f> &r_centers, std::vector<std::vector<cv::Point>> &r_contours)
{
  /*for (int i = 0; i < r_contours.size(); i++) {
    double center[3];
    cv::Rect br = cv::boundingRect(r_contours[i]);
    center[0] = br.x + br.width / 2;
    center[1] = br.y + br.height / 2;
    coords.push_back(center);
  }*/
  std::vector<cv::Moments> moments;

  for (int i = 0; i < r_contours.size(); i++) {
    cv::Moments moment = cv::moments(r_contours[i], false);
    moments.push_back(moment);
  }

  for (int i = 0; i < r_contours.size(); i++) {
    cv::Vec3f center;
    center[0] = moments[i].m10 / moments[i].m00;
    center[1] = moments[i].m01 / moments[i].m00;
    r_centers.push_back(center);
  }
}

void add_color_fields_as_targets(std::vector<Target> &r_targets, std::vector<cv::Vec3f> coords, std::string type, std::vector<std::string> orientations, std::vector<double> angles)
{
  for (int i = 0; i < coords.size(); i++) {
    Target target;
    target.type = type;
    target.coords[0] = coords[i][0];
    target.coords[1] = coords[i][1];
    target.coords[2] = coords[i][2];
    if (orientations.size()) {
      target.orientation = orientations[i];
    } else {
      target.orientation = "";
    }
    if (angles.size()) {
      target.angle = angles[i];
    } else {
      target.angle = -1;
    }
    r_targets.push_back(target);
  }
}

void pix_to_ft_warp(std::vector<Target> &r_targets, cv::Size warp_size, int pix_per_ft_x, int pix_per_ft_y)
{
  for (int i = 0; i < r_targets.size(); i++) {
    r_targets[i].coords[0] = (r_targets[i].coords[0] - (warp_size.width / 2)) / pix_per_ft_x;
    r_targets[i].coords[1] = (warp_size.height - r_targets[i].coords[1]) / pix_per_ft_y;
  }
}

void pix_to_ft_depth(std::vector<Target> &r_targets, cv::Mat depth, double depth_correction, int pix_per_ft_x, int pix_per_ft_y)
{
  for (int i = 0; i < r_targets.size(); i++) {
    double tmp_coords[3];
    tmp_coords[0] = r_targets[i].coords[0];
    tmp_coords[1] = r_targets[i].coords[1];

    r_targets[i].coords[0] = (r_targets[i].coords[0] - (depth.size().width / 2)) / pix_per_ft_x;
    r_targets[i].coords[1] = (depth.at<uchar>(tmp_coords[0],tmp_coords[1]) / 305.0) * depth_correction;
  }
}

void annotate_frame(std::vector<Target> targets, cv::Mat frame, int bgr_color[3], float pix_per_ft_x, float pix_per_ft_y)
{
  /* Annotate frame with identified targets (debugging) */
  for (int i = 0; i < targets.size(); i++) {
    float pixels_x = pix_per_ft_x * targets[i].coords[0] + (frame.size().width / 2);
    float pixels_y = frame.size().height - pix_per_ft_y * targets[i].coords[1];

    cv::Point center(cvRound(pixels_x), cvRound(pixels_y));
    cv::Scalar color(bgr_color[0], bgr_color[1], bgr_color[2]);
    cv::circle(frame, center, 3, color, -1, 8, 0);
    cv::circle(frame, center, 50, color, 3, 8, 0);
  }
}

}
