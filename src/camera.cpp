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
 * This file implements the camera framework.
 */

#include <vector>
#include <string>
#include <cmath>
#include <cstdio>

#include <opencv2/opencv.hpp>
#include <opencv2/gpu/gpu.hpp>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>

#include "camera.hpp"
#include "targets.hpp"
#include "analysis/analysis.hpp"
#include "analysis/analysis_helper.hpp"

namespace acv {

/* Helper functions */
void reorient_targets(std::vector<Target> &r_targets, double coords[3], int rotation);
void clear_targets(std::vector<Target> &r_targets);
cv::Mat warp_perspective(cv::Mat frame, int m_declination);

/****************************************************************************
 *
 * class Camera
 *
 ****************************************************************************/

void Camera::getNext()
{
  /* Dummy function */
}

void Camera::findTargets()
{
  /* Dummy function */
}

std::vector<Target> Camera::getTargets()
{
  return m_targets;
}

/****************************************************************************
 *
 * class WarpCamera
 *
 ****************************************************************************/

WarpCamera::WarpCamera(int cam_num, double coords[3], int declination, int rotation, int hfov, int vfov)
{
  assert(0 < declination < 90);

  m_cam_num = cam_num;
  m_coords[0] = coords[0];
  m_coords[1] = coords[1];
  m_coords[2] = coords[2];
  m_declination = declination;
  m_effective_height = coords[2] * tan((90 - declination) * 3.1415 / 180);
  m_hfov = hfov;
  m_vfov = vfov;
  m_rotation = rotation;

  /* initialize capture */
  m_capture = cam_num;
  if (!m_capture.isOpened())
    exit(1);
}

void WarpCamera::process()
{
  /* Convert and blur frame */
  /* Do we have a compatible GPU? */
  if (cv::gpu::getCudaEnabledDeviceCount()) {
    cv::gpu::GpuMat gpu_frame(m_frame);
    cv::gpu::GpuMat gpu_out;

    cv::gpu::cvtColor(gpu_frame, gpu_frame, CV_BGR2HSV);
    cv::gpu::bilateralFilter(gpu_frame, gpu_out, 3, 3, 3);

    gpu_out.download(m_blurred_frame);
  } else {
    /* Fallback to CPU */
    cv::Mat hsv;
    cv::cvtColor(m_frame, hsv, CV_BGR2HSV);
    cv::bilateralFilter(hsv, m_blurred_frame, 4, 3, 3);
  }

  m_warped_blurred_frame = warp_perspective(m_blurred_frame, m_declination);
}

void WarpCamera::getNext()
{
  /* Retrieve frame */
  m_capture >> m_frame;

  process();
}

void WarpCamera::showFrame()
{
  char frame_name[10];
  sprintf(frame_name, "Camera %d", m_cam_num);
  cv::imshow(frame_name, m_frame);
  cv::waitKey(30);
}

void WarpCamera::showBlurredFrame()
{
  char frame_name[10];
  sprintf(frame_name, "Camera %d", m_cam_num);
  cv::imshow(frame_name, m_blurred_frame);
  cv::waitKey(30);
}

void WarpCamera::showWarpedBlurredFrame()
{
  char frame_name[10];
  sprintf(frame_name, "Camera %d", m_cam_num);
  cv::imshow(frame_name, m_warped_blurred_frame);
  cv::waitKey(30);
}


void WarpCamera::getNextFromImage(std::string image)
{
  /* Clear targets from last frame */
  clear_targets(m_targets);

  m_frame = cv::imread(image, CV_LOAD_IMAGE_COLOR);

  process();
}

void WarpCamera::findTargets()
{
  std::vector<Target> bins;
  find_bins(m_blurred_frame, m_warped_blurred_frame, cv::Mat(), bins, m_effective_height, 0.0, m_hfov, m_vfov);

  std::vector<Target> noodles;
  find_noodles(m_blurred_frame, m_warped_blurred_frame, cv::Mat(), noodles, m_effective_height, 0.0, m_hfov, m_vfov);

  std::vector<Target> scoring_zones;
  find_scoring_zones(m_blurred_frame, m_warped_blurred_frame, cv::Mat(), scoring_zones, m_effective_height, 0.0, m_hfov, m_vfov);

  std::vector<Target> yellow_totes;
  find_yellow_totes(m_blurred_frame, m_warped_blurred_frame, cv::Mat(), yellow_totes, m_effective_height, 0.0, m_hfov, m_vfov);

  std::vector<Target> gray_totes;
  find_gray_totes(m_blurred_frame, m_warped_blurred_frame, cv::Mat(), gray_totes, m_effective_height, 0.0, m_hfov, m_vfov);

  /* Convert target coordinates (which are with respect to the camera) to be with respect to the robot. */
  reorient_targets(bins, m_coords, m_rotation);
  reorient_targets(noodles, m_coords, m_rotation);
  reorient_targets(scoring_zones, m_coords, m_rotation);
  reorient_targets(yellow_totes, m_coords, m_rotation);
  reorient_targets(gray_totes, m_coords, m_rotation);

  /* Add converted targets to array */
  for (int i = 0; i < bins.size(); i++) {
    m_targets.push_back(bins[i]);
  }
  for (int i = 0; i < noodles.size(); i++) {
    m_targets.push_back(noodles[i]);
  }
  for (int i = 0; i < scoring_zones.size(); i++) {
    m_targets.push_back(scoring_zones[i]);
  }
  for (int i = 0; i < yellow_totes.size(); i++) {
    m_targets.push_back(yellow_totes[i]);
  }
  for (int i = 0; i < gray_totes.size(); i++) {
    m_targets.push_back(gray_totes[i]);
  }
}


/****************************************************************************
 *
 * class DepthCamera
 *
 ****************************************************************************/

DepthCamera::DepthCamera(int cam_num, double coords[3], int declination, int rotation, int hfov, int vfov)
{
  m_cam_num = cam_num;
  m_coords[0] = coords[0];
  m_coords[1] = coords[1];
  m_coords[2] = coords[2];
  m_depth_correction = sin((90 - declination) * 3.1415 / 180);
  std::cout << m_depth_correction << std::endl;
  m_effective_height = coords[2] * tan((90 - declination) * 3.1415 / 180);
  m_hfov = hfov;
  m_vfov = vfov;
  m_rotation = rotation;

  libfreenect2::Freenect2 freenect2;
  m_kinect = freenect2.openDefaultDevice();
  if(!m_kinect) {
    exit(1);
  }
}

void DepthCamera::process()
{
  // TODO Adjust depth

  m_color = warp_perspective(m_color, m_declination);
  m_depth = warp_perspective(m_depth, m_declination);
}

void DepthCamera::getNext()
{
  libfreenect2::Freenect2 freenect2;
  m_kinect = freenect2.openDefaultDevice();
  libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
  libfreenect2::FrameMap frames;

  m_kinect->setColorFrameListener(&listener);
  m_kinect->setIrAndDepthFrameListener(&listener);
  m_kinect->start();

  listener.waitForNewFrame(frames);
  libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
  libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];

  m_color = cv::Mat(rgb->height, rgb->width, CV_8UC3, rgb->data);
  m_depth = cv::Mat(depth->height, depth->width, CV_32FC1, depth->data) / 4500.0f;

  listener.release(frames);

  process();
}

void DepthCamera::showColor()
{
  char frame_name[10];
  sprintf(frame_name, "Camera %d", m_cam_num);
  cv::imshow(frame_name, m_color);

  cv::waitKey(30);
}

void DepthCamera::showDepth()
{
  char frame_name[10];
  sprintf(frame_name, "Camera %d", m_cam_num);
  cv::imshow(frame_name, m_depth);

  cv::waitKey(30);
}

void DepthCamera::getNextFromImage(std::string color, std::string depth)
{
  /* Clear targets from last frame */
  clear_targets(m_targets);

  m_color = cv::imread(color, CV_LOAD_IMAGE_COLOR);
  m_depth = cv::imread(depth, CV_LOAD_IMAGE_COLOR);

  process();
}

void DepthCamera::findTargets(std::vector<cv::Point> points)
{
  std::vector<Target> totes;
  find_totes_depth(m_color, m_depth, totes, points, m_effective_height, m_hfov, m_vfov);

  /* Convert target coordinates (which are with respect to the camera) to be with respect to the robot. */
  //reorient_targets(bins, m_coords, m_rotation);
  //reorient_targets(noodles, m_coords, m_rotation);
  //reorient_targets(scoring_zones, m_coords, m_rotation);
  //reorient_targets(yellow_totes, m_coords, m_rotation);
  //reorient_targets(gray_totes, m_coords, m_rotation);

  /* Add converted targets to array */
  for (int i = 0; i < totes.size(); i++) {
    m_targets.push_back(totes[i]);
  }
}

/****************************************************************************
 *
 * Helper functions
 *
 ****************************************************************************/

void reorient_targets(std::vector<Target> &r_targets, double coords[3], int rotation)
{
  Target tmp_target;

  for (int i = 0; i < r_targets.size(); i++) {
    /* Rotate target */
//      printf("%f %f %f\n", (float)r_targets[i].coords[0],
//            (float)r_targets[i].coords[1], (float)r_targets[i].coords[2]);
    tmp_target.coords[0] =
               cos(rotation * 3.141592 / 180) * r_targets[i].coords[0] -
               sin(rotation * 3.141592 / 180) * r_targets[i].coords[1];
    tmp_target.coords[1] =
               sin(rotation * 3.141592 / 180) * r_targets[i].coords[0] +
               cos(rotation * 3.141592 / 180) * r_targets[i].coords[1];
    r_targets[i].coords[0] = tmp_target.coords[0];
    r_targets[i].coords[1] = tmp_target.coords[1];
//        printf("%f %f %f\n", (float)r_targets[i].coords[0],
//              (float)r_targets[i].coords[1], (float)r_targets[i].coords[2]);

    /* Translate / slide target */
    r_targets[i].coords[0] += coords[0];
    r_targets[i].coords[1] += coords[1];
  }
}

void clear_targets(std::vector<Target> &r_targets)
{
  while (r_targets.size()) {
    r_targets.pop_back();
  }
}

cv::Mat warp_perspective(cv::Mat frame, int declination)
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
  float warp_factor = 90.0 / declination;
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
  cv::Mat warped_frame;
  cv::warpPerspective(frame, warped_frame, M, dsize);

  return warped_frame;
}

}

