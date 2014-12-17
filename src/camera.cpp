/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * This file implements the camera framework.
 *
 */

#include <vector>
#include <string>
#include <cmath>

#include <opencv2/opencv.hpp>

#include "camera.hpp"
#include "image_analysis.hpp"
#include "targets.hpp"

namespace acv {

/****************************************************************************
 *
 * class Camera
 *
 ****************************************************************************/

void Camera::getFrame()
{
  /* Clear targets from last frame */
  while (m_targets.size()) {
    m_targets.pop_back();
  }

  m_capture >> m_frame;
}

void Camera::getFrameFromImage(std::string image)
{
  /* Clear targets from last frame */
  while (m_targets.size()) {
    m_targets.pop_back();
  }

  m_frame = cv::imread(image, CV_LOAD_IMAGE_UNCHANGED);
}


void Camera::showFrame()
{
  char frame_name[10];
  sprintf(frame_name, "Camera %d", m_cam_num);
  cv::imshow(frame_name, m_frame);
  cv::waitKey(30);
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

WarpCamera::WarpCamera(int cam_num, double coords[3], int declination, int orientation, int pix_per_ft)
{
  assert(0 < declination < 90);

  m_cam_num = cam_num;
  m_coords[0] = coords[0];
  m_coords[1] = coords[1];
  m_coords[2] = coords[2];
  m_declination = declination;
  m_distance = coords[2] * tan((90 - declination) * 3.1415 / 180) * pix_per_ft;
  m_pix_per_ft = pix_per_ft;
  m_orientation = orientation;

  /* initialize capture */
  m_capture = cam_num;
  if (!m_capture.isOpened())
    exit(1);
}

WarpCamera::WarpCamera(std::string input_file, double coords[3], int declination, int orientation, int pix_per_ft)
{
  assert(0 < declination < 90);

  m_cam_num = -1;
  m_coords[0] = coords[0];
  m_coords[1] = coords[1];
  m_coords[2] = coords[2];
  m_declination = declination;
  m_distance = coords[2] * tan((90 - declination) * 3.1415 / 180) * pix_per_ft;
  m_pix_per_ft = pix_per_ft;
  m_orientation = orientation;

  /* initialize capture */
  m_capture = input_file;
  if (!m_capture.isOpened())
    exit(1);
}

void WarpCamera::warpPerspective()
{
  /* get camera size */
  cv::Size cam_size = m_frame.size();

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
  cv::warpPerspective(m_frame, m_frame, M, dsize);
}

void WarpCamera::findTargets()
{
  std::string colors[2] = {"red", "blue"};

  for (int i = 0; i < 2; i++) {
    std::vector<Target> merge_targets;
    findTargetsInFrame(m_frame, merge_targets, colors[i], m_distance, m_pix_per_ft);

    /* Convert merge_targets coordinates (which are with respect to the camera) to be with respect to the robot. */
    Target tmp_target;
    for (int i = 0; i < merge_targets.size(); i++) {
//        printf("%f %f %f\n", (float)merge_targets[i].coords[0], (float)merge_targets[i].coords[1], (float)merge_targets[i].coords[2]);
      tmp_target.coords[0] = cos(m_orientation * 3.141592 / 180) * merge_targets[i].coords[0] - sin(m_orientation * 3.141592 / 180) * merge_targets[i].coords[1];
      tmp_target.coords[1] = sin(m_orientation * 3.141592 / 180) * merge_targets[i].coords[0] + cos(m_orientation * 3.141592 / 180) * merge_targets[i].coords[1];
      merge_targets[i].coords[0] = tmp_target.coords[0];
      merge_targets[i].coords[1] = tmp_target.coords[1];
//        printf("%f %f %f\n", (float)merge_targets[i].coords[0], (float)merge_targets[i].coords[1], (float)merge_targets[i].coords[2]);
    }

    /* Add converted targets to array */
    for (int i = 0; i < merge_targets.size(); i++) {
      m_targets.push_back(merge_targets[i]);
    }
  }
}

/****************************************************************************
 *
 * class DepthCamera
 *
 ****************************************************************************/

DepthCamera::DepthCamera(int cam_num, double coords[3], int orientation)
{
  m_cam_num = cam_num;
  m_coords[0] = coords[0];
  m_coords[1] = coords[1];
  m_coords[2] = coords[2];
  m_orientation = orientation;

  /* initialize capture */
  m_capture = cam_num;
  if (!m_capture.isOpened())
    exit(1);
}

DepthCamera::DepthCamera(std::string input_file, double coords[3], int orientation)
{
  m_cam_num = -1;
  m_coords[0] = coords[0];
  m_coords[1] = coords[1];
  m_coords[2] = coords[2];
  m_orientation = orientation;

  /* initialize capture */
  m_capture = input_file;
  if (!m_capture.isOpened())
    exit(1);
}

}

