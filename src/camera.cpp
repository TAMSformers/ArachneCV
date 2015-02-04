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

#include <opencv2/opencv.hpp>
#include <opencv2/gpu/gpu.hpp>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/threading.h>

#include "camera.hpp"
#include "targets.hpp"
#include "analysis/analysis.hpp"
#include "analysis/analysis_helper.hpp"

namespace acv {

/* Helper functions */
void reorient_targets(std::vector<Target> &r_targets, double coords[3], int rotation);
void clear_targets(std::vector<Target> &r_targets);

/****************************************************************************
 *
 * class Camera
 *
 ****************************************************************************/

void Camera::getFrame()
{
  /* Clear targets from last frame */
  clear_targets(m_targets);

  m_capture >> m_frame;
}

void Camera::showFrame()
{
  //char frame_name[10];
  //sprintf(frame_name, "Camera %d", m_cam_num);
  //cv::imshow(frame_name, m_frame);
  
  std::cout << "Debugging is fun!!!1!!1!!!1!" << std::endl;

  char frame_name[10];
  sprintf(frame_name, "Camera %d", m_cam_num);
  cv::imshow(frame_name, m_frame_blur);

  char warp_name[10];
  sprintf(warp_name, "Warp %d", m_cam_num);
  cv::imshow(warp_name, m_warp_blur);

  cv::waitKey(30);
}

void Camera::cvtAndBlur()
{
  /* Do we have a compatible GPU? */
  if (cv::gpu::getCudaEnabledDeviceCount()) {
    cv::gpu::GpuMat gpu_frame(m_frame);
    cv::gpu::GpuMat gpu_out;

    cv::gpu::cvtColor(gpu_frame, gpu_frame, CV_BGR2HSV);
    cv::gpu::bilateralFilter(gpu_frame, gpu_out, 3, 3, 3);

    gpu_out.download(m_frame_blur);
  } else {
    /* Fallback to CPU */
    cv::Mat hsv;
    cv::cvtColor(m_frame, hsv, CV_BGR2HSV);
    cv::bilateralFilter(hsv, m_frame_blur, 4, 3, 3);
  }
}

/*void annotateFrame()
{
  int pix_per_ft_x = fov_to_pix_per_ft(m_frame.size().width, m_effective_height, m_hfov, m_vfov);
  int pix_per_ft_y = fov_to_pix_per_ft(m_frame.size().height, m_effective_height, m_hfov, m_vfov);

   Annotate frame with identified targets (debugging) 
  for (int i = 0; i < m_targets.size(); i++) {
    float pixels_x = pix_per_ft_x * m_targets[i].coords[0] + (m_frame.size().width / 2);
    float pixels_y = m_frame.size().height - pix_per_ft_y * m_targets[i].coords[1];

    cv::Point center(cvRound(pixels_x), cvRound(pixels_y));
    if (m_targets[i].type == "bin") {
      cv::Scalar color(0, 0, 255);
      cv::circle(m_frame, center, 3, color, -1, 8, 0);
      cv::circle(m_frame, center, 50, color, 3, 8, 0);
    } else if (m_targets[i].type == "noodle") {
      cv::Scalar color(255, 0, 0);
      cv::circle(m_frame, center, 3, color, -1, 8, 0);
      cv::circle(m_frame, center, 50, color, 3, 8, 0);
    } else if (m_targets[i].type == "scoring zone") {
      cv::Scalar color(255, 255, 255);
      cv::circle(m_frame, center, 3, color, -1, 8, 0);
      cv::circle(m_frame, center, 50, color, 3, 8, 0);
    }
  }
}*/


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

void WarpCamera::getFrameFromImage(std::string image)
{
  /* Clear targets from last frame */
  clear_targets(m_targets);

  m_frame = cv::imread(image, CV_LOAD_IMAGE_COLOR);
}

void WarpCamera::warpPerspective()
{
  /* get camera size */
  cv::Size cam_size = m_frame_blur.size();

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
  float warp_factor = 90.0 / m_declination;
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
  cv::warpPerspective(m_frame_blur, m_warp_blur, M, dsize);
}

void WarpCamera::findTargets()
{
  std::vector<Target> bins;
  find_bins(m_frame_blur, m_warp_blur, cv::Mat(), bins, m_effective_height, 0.0, m_hfov, m_vfov);

  std::vector<Target> noodles;
  find_noodles(m_frame_blur, m_warp_blur, cv::Mat(), noodles, m_effective_height, 0.0, m_hfov, m_vfov);

  std::vector<Target> scoring_zones;
  find_scoring_zones(m_frame_blur, m_warp_blur, cv::Mat(), scoring_zones, m_effective_height, 0.0, m_hfov, m_vfov);

  std::vector<Target> yellow_totes;
  find_yellow_totes(m_frame_blur, m_warp_blur, cv::Mat(), yellow_totes, m_effective_height, 0.0, m_hfov, m_vfov);

  std::vector<Target> gray_totes;
  //find_gray_totes(m_frame_blur, m_warp_blur, cv::Mat(), gray_totes, m_effective_height, 0.0, m_hfov, m_vfov);

  /* Debugging */
  /*int bins_color[3] = {0, 0, 255};
  int noodles_color[3] = {255, 0, 0};
  int scoring_color[3] = {255, 255, 255};
  float pix_per_ft_x = fov_to_pix_per_ft(m_frame.size().width, m_effective_height, m_hfov, m_vfov);
  float pix_per_ft_y = fov_to_pix_per_ft(m_frame.size().height, m_effective_height, m_hfov, m_vfov);
  annotate_frame(bins, m_frame, bins_color, pix_per_ft_x, pix_per_ft_y);
  annotate_frame(noodles, m_warp_blur, noodles_color, pix_per_ft_x, pix_per_ft_y);
  annotate_frame(scoring_zones, m_warp_blur, scoring_color, pix_per_ft_x, pix_per_ft_y);*/
  /* \Debugging */

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
  m_effective_height = coords[2] * tan((90 - declination) * 3.1415 / 180);
  m_hfov = hfov;
  m_vfov = vfov;
  m_rotation = rotation;

  /* initialize capture */
  //m_capture = cam_num;
  //if (!m_capture.isOpened())
  //  exit(1);
  
  m_capture = m_freenect2.openDefaultDevice();
    
  if (m_capture == 0) {
    exit(1);
  }

  m_listener = new libfreenect2::SyncMultiFrameListener(
       libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
  
  //m_listener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);

  m_capture->setColorFrameListener(m_listener);
  m_capture->setIrAndDepthFrameListener(m_listener);
  m_capture->start();
}

void DepthCamera::getFrame()
{
  /* Clear targets from last frame */
  clear_targets(m_targets);

  //m_capture.grab();
  //m_capture.retrieve(m_depth, CV_CAP_OPENNI_DEPTH_MAP);
  //m_capture.retrieve(m_frame, CV_CAP_OPENNI_BGR_IMAGE);

  std::cout << "Debugging is radical!!!1!!1!!!1!" << std::endl;

  m_listener->waitForNewFrame(m_frames);

  std::cout << "Debugging is sweet!!!1!!1!!!1!" << std::endl;

  libfreenect2::Frame *rgb = m_frames[libfreenect2::Frame::Color];
  libfreenect2::Frame *depth = m_frames[libfreenect2::Frame::Depth];

  m_frame = cv::Mat(rgb->height, rgb->width, CV_8UC3, rgb->data);
  m_depth = cv::Mat(depth->height, depth->width, CV_32FC1, depth->data) / 4500.0f;

  m_listener->release(m_frames);

  std::cout << "Debugging is awesome!!!1!!1!!!1!" << std::endl;
}

void DepthCamera::findTargets()
{
  std::vector<Target> bins;
  find_bins(m_frame_blur, m_warp_blur, m_depth, bins, m_effective_height, m_depth_correction, m_hfov, m_vfov);

  std::vector<Target> noodles;
  find_noodles(m_frame_blur, m_warp_blur, m_depth, noodles, m_effective_height, m_depth_correction, m_hfov, m_vfov);

  std::vector<Target> scoring_zones;
  find_scoring_zones(m_frame_blur, m_warp_blur, m_depth, scoring_zones, m_effective_height, m_depth_correction, m_hfov, m_vfov);

  std::vector<Target> yellow_totes;
  find_yellow_totes(m_frame_blur, m_warp_blur, m_depth, yellow_totes, m_effective_height, m_depth_correction, m_hfov, m_vfov);

  std::vector<Target> gray_totes;
  //find_gray_totes(m_frame_blur, m_warp_blur, m_depth, gray_totes, m_effective_height, m_depth_correction, m_hfov, m_vfov);

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

void DepthCamera::warpPerspective()
{
  /* get camera size */
  cv::Size cam_size = m_frame_blur.size();

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
  float warp_factor = 90.0 / m_declination;
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
  cv::warpPerspective(m_frame_blur, m_warp_blur, M, dsize);
  cv::warpPerspective(m_depth, m_depth, M, dsize);
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

}

