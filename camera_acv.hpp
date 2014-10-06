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

class acvCamera
{
  private:
    cv::Size cam_size;
    int cam_coords[ 2 ];
    int cam_angle;

    cv::VideoCapture capture;
    cv::Mat frame;

  public:
    acvCamera( int cam_num_in, int cam_size_in[ 2 ], int cam_coords_in[ 2 ], int cam_angle_in );
    void acvGetFrame( );
    void acvWarpPerspective( );
};

}

#endif
