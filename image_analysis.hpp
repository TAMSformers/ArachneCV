/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * Object-finding algorithms
 *
 */

#ifndef __ARACHNECV_IMAGE_ANALYSIS__
#define __ARACHNECV_IMAGE_ANALYSIS__

#include <opencv2/opencv.hpp>

#include "targets.hpp"

namespace acv {

void findTargetsInFrame(cv::Mat frame_in, std::vector<Target> &targets, std::string color, double cam_distance, int pix_per_ft);

}

#endif
