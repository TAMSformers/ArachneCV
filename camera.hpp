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

#include "ArachneCVConfig.h"
#include "image_analysis.hpp"
#include "targets.hpp"

namespace acv {

/** \brief internal representation of a camera
 *
 */
class Camera
{
  private:
    int cam_num; /*< camera number */
    double cam_coords[3]; /*< physical location */
    int cam_angle; /*< angle of declination, needed for warping */
    double cam_distance; /*< length of field of view captured by the camera from overhead */
    int cam_orientation; /*< direction camera faces, measured counter-clockwise from front */

    cv::VideoCapture capture; /*< video stream from camera */
    cv::Mat frame; /*< current frame from video stream */
    cv::Mat warped;

  public:
    Camera(int cam_num_in, double cam_coords_in[3], int cam_angle_in, int orientation, int pix_per_ft); /**< contructor */
    void getFrame(); /**< retrieve next frame, should be run in a loop */
    void getFrameFromImage(std::string image); /**< load next frame from specified image instead of camera. useful for testing */
    void warpPerspective(); /**< warp image to generate overhead view from angled camera */
    void findTargets();
    void showFrame(); /**< create a window if not opened and show the current frame */
    Target targets[16];
};

}

#endif
