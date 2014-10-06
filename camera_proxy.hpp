/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * This proxy file wraps the camera framework.
 *
 */

#ifndef __ARACHNECV_CAMERA_PROXY__
#define __ARACHNECV_CAMERA_PROXY__

#include "camera_acv.hpp"

//#include "ArachneCVConfig.h"

namespace acv{

class Camera
{
  private:
    int cam_num;
    int cam_size[ 2 ];
    int cam_coords[ 2 ];
    int cam_angle;

    acvCamera *acv_camera;

  public:
    Camera( int cam_num_in, int cam_size_in[ 2 ], int cam_coords_in[ 2 ], int cam_angle_in );
    void GetFrame( );
    void WarpPerspective( );
   ~Camera( );
};

}

#endif
