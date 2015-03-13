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

#ifndef __ARACHNECV_CAMERA__
#define __ARACHNECV_CAMERA__

#include <vector>
#include <cmath>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/gpu/gpu.hpp>

#include <libfreenect2/libfreenect2.hpp>

#include "analysis/analysis.hpp"
#include "targets.hpp"

namespace acv {

/**
 * Base class for WarpCamera and DepthCamera. Wraps an opencv VideoCapture
 * object with additional functionality.
 */
class Camera
{
  protected:

    /**
     * Camera number. An internal webcam, if present, is usually 0.
     */
    int m_cam_num;

    /**
     * Physical location of the Camera in 3D space. It is a good idea to make
     * this relative to the center of the robot.
     */
    double m_coords[3];

    /**
     * The horizontal direction the Camera faces, measured counter-clockwise
     * from the front in degrees.
     */
    int m_rotation;

    /**
     * Angle of declination. Used to determine how far to warp.
     */
    int m_declination; 

    /**
     * Horizontal field of view in degrees.
     */
    int m_hfov;

    /**
     * Vertical field of view in degrees.
     */
    int m_vfov;

    /**
     * When the image is warped, the camera appears to be at a lower
     * height. This is important for image analysis calculations.
     */
    double m_effective_height;

    /**
     * Vector of all targets (bins, totes, etc.) found in current frame.
     */
    std::vector<Target> m_targets;

  public:

    /**
     * Retrieve next image from stream.
     */
    void getNext();

    /**
     * Process current frame to locate targets (bins, totes, etc.).
     */
    void findTargets();

    /**
     * Retrieve targets stored internally by findTargets().
     */
    std::vector<Target> getTargets();
};

/**
 * Downward facing single-channel camera.
 */
class WarpCamera : public Camera
{
  protected:

    /**
     * OpenCV video stream.
     */
    cv::VideoCapture m_capture;

    /**
     * Current unaltered frame from video stream.
     */
    cv::Mat m_frame;

    /**
     * Current blurred frame from video stream.
     */
    cv::Mat m_blurred_frame;

    /**
     * Current blurred and warped frame from video stream.
     */
    cv::Mat m_warped_blurred_frame;

    /**
     * Apply image preprocessing.
     */
    void process();

  public:

    /**
     * Constructor for use with a video camera.
     * @param[in] cam_num     Camera device number.
     * @param[in] coords      3D location relative to the robot's center.
     * @param[in] declination Angle downward in degrees.
     * @param[in] rotation Horizontal angle counterclockwise from the front in degrees.
     * @param[in] hfov        Horizontal field of view in degrees.
     * @param[in] vfov        Vertical field of view in degrees.
     */
    WarpCamera(int cam_num, double coords[3], int declination, int rotation, int hfov, int vfov);

    /**
     * Retrieve next image from stream.
     */
    void getNext();

    /**
     * Display current unaltered frame.
     */
    void showFrame();

    /**
     * Display current blurred frame.
     */
    void showBlurredFrame();

    /**
     * Display current warped, blurred frame.
     */
    void showWarpedBlurredFrame();

    /**
     * Retrieve next frame from an image, ignoring the stream. Useful for
     * debugging.
     *
     * @param[in] image Image file path. Relative or absolute.
     */
    void getNextFromImage(std::string image);

    /**
     * Process current frame to locate targets (bins, totes, etc.).
     */
    void findTargets();
};

/**
 * Kinect Camera
 */
class DepthCamera : public Camera
{
  protected:

    /**
     * OpenNI video stream.
     */
    //libfreenect2::Freenect2Device *m_kinect;
    cv::VideoCapture m_kinect;

    /**
     * Current color frame from video stream.
     */
    cv::Mat m_color;

    /**
     * Current depth frame from video stream.
     */
    cv::Mat m_depth;

    /**
     * Correction for depth map due to camera declination.
     */
    double m_depth_correction;

    /**
     * Apply image preprocessing.
     */
    void process();

  public:

    /**
     * Constructor for use with a video camera.
     * @param[in] cam_num     Camera device number.
     * @param[in] coords      3D location relative to the robot's center.
     * @param[in] declination Angle downward in degrees.
     * @param[in] rotation Horizontal angle counterclockwise from the front in degrees.
     * @param[in] hfov        Horizontal field of view in degrees.
     * @param[in] vfov        Vertical field of view in degrees.
     */
    DepthCamera(int cam_num, double coords[3], int declination, int rotation, int hfov, int vfov);

    /**
     * Retrieve next frame from stream.
     */
    void getNext();

    /**
     * Process current frame to locate targets (bins, totes, etc.).
     *
     * @param[in] points      Human-provided points to distinguish totes.
     */
    void findTargets(std::vector<cv::Point> points);

    /**
     * Display current color frame.
     */
    void showColor();

    /**
     * Display current depth frame.
     */
    void showDepth();

    /**
     * Retrieve next frame from an image, ignoring the stream. Useful for
     * debugging.
     *
     * @param[in] image Image file path. Relative or absolute.
     */
    void getNextFromImage(std::string color, std::string depth);

};

}

#endif
