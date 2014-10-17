/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * This file implements object locations.
 *
 */

#include <opencv2/opencv.hpp>

#include "ArachneCV.hpp"

#include "ArachneCVConfig.h"

namespace acv{

Targets::Targets( )
{
  /*TODO maybe add dynamically allocated memory */
}

void Targets::Merge( Camera camera )
{
  camera.acv_camera->acvFindBalls( );
  camera.acv_camera->acvFindRobots( );
}

Targets::~Targets( )
{
  /*TODO free any memory we need to */
}

}
