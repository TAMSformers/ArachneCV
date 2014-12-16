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
  protected:
    int m_cam_num; /*< camera number, -1 if video stream */
    double m_coords[3]; /*< physical location */
    int m_declination; /*< angle of declination, needed for warping */
    int m_orientation; /*< direction camera faces, measured counter-clockwise from front */
    int m_pix_per_ft; /*< Number of pixels equaling a foot when warped. */

    double m_distance; /*< length of field of view captured by the camera from overhead */
    std::vector<Target> m_targets; /* holds all targets found in current frame */
    cv::VideoCapture m_capture; /*< video stream from camera */
    cv::Mat m_frame; /*< current frame from video stream */

  public:
    Camera(int cam_num, double coords[3], int declination, int orientation, int pix_per_ft); /**< webcam constructor */
    Camera(std::string input_file, double coords[3], int declination, int orientation, int pix_per_ft); /**< video file constructor */
    void getFrame(); /**< retrieve next frame, should be run in a loop */
    void getFrameFromImage(std::string image); /**< load next frame from specified image instead of camera. useful for testing */
    void warpPerspective(); /**< warp image to generate overhead view from angled camera */
    void findTargets(); /**< processes image to locate targets and stores them internally */
    void showFrame(); /**< create a window if not opened and show the current frame */
    std::vector<Target> getTargets(); /**< retrieves targets internally stored by findTargets */
};

class WarpCamera : protected Camera {

};

class DepthCamera : protected Camera { 

};

}

#endif
