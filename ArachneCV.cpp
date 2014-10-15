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

#include "camera.hpp"
#include "ArachneCV.hpp"

#include "ArachneCVConfig.h"

namespace acv{

Camera::Camera( int cam_num_in, int cam_size_in[ 2 ], int cam_coords_in[ 2 ], int cam_angle_in )
{
    cam_num = cam_num_in;
    cam_size[ 0 ] = cam_size_in[ 0 ];
    cam_size[ 1 ] = cam_size_in[ 1 ];
    cam_coords[ 0 ] = cam_coords_in[ 0 ];
    cam_coords[ 1 ] = cam_coords_in[ 1 ];
    cam_angle = cam_angle_in;

    acv_camera = new acvCamera( cam_num, cam_size, cam_coords, cam_angle );
}

void Camera::GetFrame( )
{
  acv_camera->acvGetFrame( );
}

void Camera::WarpPerspective( )
{
  acv_camera->acvWarpPerspective( );
}

void Camera::ShowFrame( )
{
  acv_camera->acvShowFrame( );
}

Camera::~Camera( )
{
  delete acv_camera;
}

}
