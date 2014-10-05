/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * This file implements perspective warping and camera placement framework.
 *
 */

#ifndef __ARACHNECV_OVERHEAD__
#define __ARACHNECV_OVERHEAD__

#include <math.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//#include "ArachneCVConfig.h"

namespace acv {

void WarpPerspective( cv::InputArray src, cv::OutputArray dst, cv::Size dsize, int angle );

}

#endif
