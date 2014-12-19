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

void findTargetsWarp(cv::Mat frame, std::vector<Target> &r_targets, std::string color, double effective_height, int hfov, int vfov);
void findTargetsDepth(cv::Mat frame, cv::Mat depth, std::vector<Target> &r_targets, std::string color, int hfov, int vfov);

}

#endif
