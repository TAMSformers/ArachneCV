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

#ifndef __ARACHNECV_CAMERA__
#define __ARACHNECV_CAMERA__

#include <vector>
#include <cmath>
#include <string>

#include <opencv2/opencv.hpp>

#include "image_analysis.hpp"
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
     * Camera number. An internal webcam, if present, is usually 0. Set to -1
     * if Camera is initialized with a video file.
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
     * Vector of all targets (balls, robots, etc.) found in current frame.
     */
    std::vector<Target> m_targets;

    /**
     * Video stream from camera or file.
     */
    cv::VideoCapture m_capture;

    /**
     * Current frame from video stream.
     */
    cv::Mat m_frame;

  public:

    /**
     * Retrieve next frame from stream.
     */
    void getFrame();

    /**
     * Creates a window if not opened and shows the current frame.
     */
    void showFrame();

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
     * Effective height of the camera view when warped.
     */
    double m_effective_height;

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
     * Constructor for use with a video file.
     *
     * @param[in] input_file  Video file path. Relative or absolute.
     * @param[in] coords      3D location relative to the robot's center.
     * @param[in] declination Angle downward in degrees.
     * @param[in] rotation Horizontal angle counterclockwise from the front in degrees.
     * @param[in] hfov        Horizontal field of view in degrees.
     * @param[in] vfov        Vertical field of view in degrees.
     */
    WarpCamera(std::string input_file, double coords[3], int declination, int rotation, int hfov, int vfov);

    /**
     * Retrieve next frame from an image, ignoring the stream. Useful for
     * debugging.
     *
     * @param[in] image Image file path. Relative or absolute.
     */
    void getFrameFromImage(std::string image);

    /**
     * Warp current frame to generate overhead view.
     */
    void warpPerspective();

    /**
     * Process current frame to locate targets (balls, robots, etc.).
     * Stores results internally.
     */
    void findTargets();
};

/**
 * OpenNI aware dual-channel camera.
 */
class DepthCamera : public Camera
{
  protected:

    /**
     * Holds depth data for current frame.
     */
     cv::Mat m_frame_depth;

    /**
     * Horizontal field of view in degrees.
     */
    int m_hfov;

    /**
     * Vertical field of view in degrees.
     */
    int m_vfov;

  public:

    /**
     * Constructor for use with a video camera.
     * @param[in] cam_num     Camera device number.
     * @param[in] coords      3D location relative to the robot's center.
     * @param[in] rotation Horizontal angle counterclockwise from the front in degrees.
     * @param[in] hfov        Horizontal field of view in degrees.
     * @param[in] vfov        Vertical field of view in degrees.
     */
    DepthCamera(int cam_num, double coords[3], int rotation, int hfov, int vfov);

    /**
     * Constructor for use with a video file.
     *
     * @param[in] input_file  Video file path. Relative or absolute.
     * @param[in] coords      3D location relative to the robot's center.
     * @param[in] rotation Horizontal angle counterclockwise from the front in degrees.
     * @param[in] hfov        Horizontal field of view in degrees.
     * @param[in] vfov        Vertical field of view in degrees.
     */
    DepthCamera(std::string input_file, double coords[3], int rotation, int hfov, int vfov);

    /**
     * Retrieve next frame from stream.
     */
    void getFrame();

    /**
     * Process current frame to locate targets (balls, robots, etc.).
     * Stores results internally.
     */
    void findTargets();
};

}

#endif
