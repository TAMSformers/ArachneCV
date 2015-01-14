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

#include "../targets.hpp"

namespace acv {

void find_bins(cv::Mat frame, cv::Mat warp, cv::Mat depth,
               std::vector<Target> &r_targets, double effective_height,
               double depth_correction, int hfov, int vfov);

//void find_totes_yellow(cv::Mat frame, cv::Mat warp, cv::Mat depth,
//               std::vector<Target> &r_targets, double effective_height,
//               int hfov, int vfov);

//void find_totes_gray(cv::Mat frame, cv::Mat warp, cv::Mat depth,
//               std::vector<Target> &r_targets, double effective_height,
//               int hfov, int vfov);

void find_noodles(cv::Mat frame, cv::Mat warp, cv::Mat depth,
               std::vector<Target> &r_targets, double effective_height,
               double depth_correction, int hfov, int vfov);

void find_scoring_zones(cv::Mat frame, cv::Mat warp, cv::Mat depth,
               std::vector<Target> &r_targets, double effective_height,
               double depth_correction, int hfov, int vfov);

}

#endif
