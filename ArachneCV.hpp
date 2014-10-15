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

#include "camera.hpp"

#include "ArachneCVConfig.h"

namespace acv{

/** \brief external interface to the camera
 *
 */
class Camera
{
  private:
    int cam_num; /**< camera ID as seen by the computer`*/
    int cam_size[ 2 ]; /**< camera resolution */
    int cam_coords[ 2 ]; /**< physical location */
    int cam_angle; /**< angle of declination, needed for warping */

    acvCamera *acv_camera; /**< pointer to the internal camera object */

  public:
    Camera( int cam_num_in, int cam_size_in[ 2 ], int cam_coords_in[ 2 ], int cam_angle_in ); /**< constructor */
    void GetFrame( ); /**< retrieve next frame, should be run in a loop */
    void WarpPerspective( ); /**< warp image to generate overhead view from angled camera */
    void ShowFrame( ); /**< create a window if not opened and show the current frame */
   ~Camera( ); /**< destructor */
};

}

#endif
