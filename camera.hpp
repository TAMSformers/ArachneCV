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

#ifndef __ARACHNECV_CAMERA_ACV__
#define __ARACHNECV_CAMERA_ACV__

#include <vector>
#include <cmath>

#include <opencv2/opencv.hpp>

//#include "ArachneCVConfig.h"

namespace acv{

/** \brief internal representation of a camera
 *
 */
class acvCamera
{
  private:
    cv::Size cam_size; /*< camera resolution */
    int cam_coords[ 2 ]; /*< physical location */
    int cam_angle; /*< angle of declination, needed for warping */

    cv::VideoCapture capture; /*< video stream from camera */
    cv::Mat frame; /*< current frame from video stream */

  public:
    acvCamera( int cam_num_in, int cam_size_in[ 2 ], int cam_coords_in[ 2 ], int cam_angle_in ); /**< contructor */
    void acvGetFrame( ); /**< retrieve next frame, should be run in a loop */
    void acvWarpPerspective( ); /**< warp image to generate overhead view from angled camera */
};

}

#endif
