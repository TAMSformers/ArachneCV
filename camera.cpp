/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * This file implements the camera frameframework.
 *
 */

#include <vector>
#include <string>
#include <cmath>

#include <opencv2/opencv.hpp>

#include "camera.hpp"
#include "image_analysis.hpp"
#include "targets.hpp"

#include "ArachneCVConfig.h"

namespace acv {

Camera::Camera(int cam_num_in, double cam_coords_in[3], int cam_angle_in, int orientation, int pix_per_ft_in)
{
  assert(0 < cam_angle < 90);
  cam_num = cam_num_in;
  cam_coords[0] = cam_coords_in[0];
  cam_coords[1] = cam_coords_in[1];
  cam_coords[2] = cam_coords_in[2];
  cam_angle = cam_angle_in;
  cam_distance = cam_coords[2] * tan((90 - cam_angle) * 3.1415 / 180) * pix_per_ft_in;
  pix_per_ft = pix_per_ft_in;
  cam_orientation = orientation;

  capture = cam_num_in;
  if (!capture.isOpened())
    exit(1);
}

Camera::Camera(std::string file_name_in, double cam_coords_in[3], int cam_angle_in, int orientation, int pix_per_ft_in)
{
  assert(0 < cam_angle < 90);
  cam_num = -1;
  cam_coords[0] = cam_coords_in[0];
  cam_coords[1] = cam_coords_in[1];
  cam_coords[2] = cam_coords_in[2];
  cam_angle = cam_angle_in;
  cam_distance = cam_coords[2] * tan((90 - cam_angle) * 3.1415 / 180) * pix_per_ft_in;
  pix_per_ft = pix_per_ft_in;
  cam_orientation = orientation;

  capture = file_name_in;
  if (!capture.isOpened())
    exit(1);
}

void Camera::getFrame()
{
  /* Clear targets from last frame */
  while (targets.size()) {
    targets.pop_back();
  }

  capture >> frame;
}

void Camera::getFrameFromImage(std::string image)
{
  /* Clear targets from last frame */
  while (targets.size()) {
    targets.pop_back();
  }

  frame = cv::imread(image, CV_LOAD_IMAGE_UNCHANGED);
}

void Camera::warpPerspective()
{
  /* get camera size */
  cv::Size cam_size = frame.size();

  /* point arrays to generate transform matrix */
  std::vector<cv::Point2f> src_pts(4);
  std::vector<cv::Point2f> dst_pts(4);

  /* initialize source array */
  cv::Point2f S0(0.0, 0.0);
  cv::Point2f S1((float)cam_size.width, 0.0);
  cv::Point2f S2((float)cam_size.width, (float)cam_size.height);
  cv::Point2f S3(0.0, (float)cam_size.height);
  src_pts[0] = S0;
  src_pts[1] = S1;
  src_pts[2] = S2;
  src_pts[3] = S3;

  /* find transform from reference angle */
//  if( cam_angle = 45 )
//  {
//    cv::Point2f S0( 75.0, 200 );
//    cv::Point2f S1( cam_size.width - 75.0, 200 );
//    cv::Point2f S2((float)cam_size.width, (float)cam_size.height );
//    cv::Point2f S3( 0.0 , (float)cam_size.height );
//    src_pts[ 0 ] = S0;
//    src_pts[ 1 ] = S1;
//    src_pts[ 2 ] = S2;
//    src_pts[ 3 ] = S3;
//  } else {
    /* hard-coded to 45 for now, exit if different */
//    exit(1);
//  }
  float warp_factor = 90.0 / cam_angle;
  cv::Size dsize(cam_size.width * warp_factor, cam_size.height);
  cv::Point2f D0(0.0, 0.0);
  cv::Point2f D1((float)dsize.width, 0.0);
  cv::Point2f D2(((float)dsize.width / 2) + ((float)cam_size.width / 2), (float)dsize.height);
  cv::Point2f D3(((float)dsize.width / 2) - ((float)cam_size.width / 2), (float)dsize.height);
  dst_pts[0] = D0;
  dst_pts[1] = D1;
  dst_pts[2] = D2;
  dst_pts[3] = D3;

  /* generate transform matrix */
  cv::Mat M = cv::getPerspectiveTransform(src_pts, dst_pts);
  M.convertTo(M, CV_32F);

  /* apply transform matrix */
  cv::warpPerspective(frame, frame, M, dsize);
}

void Camera::showFrame()
{
  char frame_name[10];
  sprintf(frame_name, "Camera %d", cam_num);
  cv::imshow(frame_name, frame);
  cv::waitKey(30);
}

void Camera::findTargets()
{
  std::string colors[2] = {"red", "blue"};

  for (int i = 0; i < 2; i++) {
    std::vector<Target> merge_targets;
    findTargetsInFrame(frame, merge_targets, colors[i], cam_distance, pix_per_ft);

    /* Convert merge_targets coordinates (which are with respect to the camera) to be with respect to the robot. */
    Target tmp_target;
    for (int i = 0; i < merge_targets.size(); i++) {
//        printf("%f %f %f\n", (float)merge_targets[i].coords[0], (float)merge_targets[i].coords[1], (float)merge_targets[i].coords[2]);
      tmp_target.coords[0] = cos(cam_orientation * 3.141592 / 180) * merge_targets[i].coords[0] - sin(cam_orientation * 3.141592 / 180) * merge_targets[i].coords[1];
      tmp_target.coords[1] = sin(cam_orientation * 3.141592 / 180) * merge_targets[i].coords[0] + cos(cam_orientation * 3.141592 / 180) * merge_targets[i].coords[1];
      merge_targets[i].coords[0] = tmp_target.coords[0];
      merge_targets[i].coords[1] = tmp_target.coords[1];
//        printf("%f %f %f\n", (float)merge_targets[i].coords[0], (float)merge_targets[i].coords[1], (float)merge_targets[i].coords[2]);
    }

    /* Add converted targets to array */
    for (int i = 0; i < merge_targets.size(); i++) {
      targets.push_back(merge_targets[i]);
    }
  }
}

std::vector<Target> Camera::getTargets()
{
  return targets;
}

}

