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

namespace acv{

/** \brief internal representation of a camera
 *
 */
class Camera
{
  private:
    int cam_num; /*< camera number */
    int cam_coords[ 2 ]; /*< physical location */
    int cam_angle; /*< angle of declination, needed for warping */

    cv::VideoCapture capture; /*< video stream from camera */
    cv::Mat frame; /*< current frame from video stream */
    cv::Mat warped;

  public:
    Camera( int cam_num_in, int cam_coords_in[ 2 ], int cam_angle_in ); /**< contructor */
    void GetFrame( ); /**< retrieve next frame, should be run in a loop */
    void GetFrameFromImage( std::string image ); /**< load next frame from specified image instead of camera. useful for testing */
    void WarpPerspective( ); /**< warp image to generate overhead view from angled camera */
    void FindBalls( );
    void FindRobots( );
    void ClearTargets( );
    void ShowFrame( ); /**< create a window if not opened and show the current frame */
    Target targets[ 16 ];
};

}

#endif
