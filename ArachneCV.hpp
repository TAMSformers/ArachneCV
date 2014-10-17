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

#include <string>

#include "camera.hpp"

#include "ArachneCVConfig.h"

namespace acv{

typedef struct _Target
{
  std::string type; /**< either robot, ball, or some other target */
  std::string color; /**< either red or blue */
  int coords[ 3 ]; /**< coordinates in 3-space */
} Target;

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

  public:
    acvCamera *acv_camera; /**< pointer to the internal camera object */

    Camera( int cam_num_in, int cam_size_in[ 2 ], int cam_coords_in[ 2 ], int cam_angle_in ); /**< constructor */
    void GetFrame( ); /**< retrieve next frame, should be run in a loop */
    void WarpPerspective( ); /**< warp image to generate overhead view from angled camera */
    void FindBalls( /*data pointer*/ ); /**< TODO wrap function in targets */
    void FindRobots( /*data pointer*/ ); /**< TODO wrap function in targets */
    void ShowFrame( ); /**< create a window if not opened and show the current frame */
   ~Camera( ); /**< destructor */
};

/* FindBalls and FindRobots will return pointers to dynamically allocated memory. Call from within Targets. Merge and copy to internal member, then free. */

/* Call Targets **type** without any parameters. Call Targets.merge( $Camera ) on each individual camera; it will call FindBalls and FindRobots and do above */

class Targets
{
  private:
    Target target[10];
  public:
    Targets( );
    void Merge( Camera camera );
   ~Targets( );
};

}

#endif
