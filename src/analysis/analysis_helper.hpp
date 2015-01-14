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

#ifndef __ARACHNECV_IMAGE_ANALYSIS_HELPER__
#define __ARACHNECV_IMAGE_ANALYSIS_HELPER__

#include <opencv2/opencv.hpp>

#include "../targets.hpp"

namespace acv {

/**
 * Returns number of pixels equal to a foot in some direction.
 */
int fov_to_pix_per_ft(int frame_dim, double effective_height, int hfov, int vfov);

/**
 * Find all contours within given HSV range in image.
 */
void color_fields_in_range(std::vector<std::vector<cv::Point>> &r_contours,
                           cv::Mat frame, int lower_bound[3],
                           int upper_bound[3]);

/**
 * Filter out all color fields under size threshold_ft.
 */
void filter_color_fields(std::vector<std::vector<cv::Point>> &r_contours, std::vector<std::vector<cv::Point>> &r_all_contours, float pix_per_ft_x, float pix_per_ft_y, float threshold_ft);

/**
 * Find the mass center of a vector of color fields.
 */
void find_centers_of_color_fields(std::vector<cv::Vec3f> &r_centers, std::vector<std::vector<cv::Point>> &r_contours);

/**
 * Synthesis target characteristics into vector of Target.
 */
void add_color_fields_as_targets(std::vector<Target> &r_targets, std::vector<cv::Vec3f> coords, std::string type, std::vector<std::string> orientations, std::vector<double> angles);

/**
 * Convert coordinates from feet to pixels without depth info.
 */
void pix_to_ft_warp(std::vector<Target> &r_targets, cv::Size warp_size, int pix_per_ft_x, int pix_per_ft_y);

/**
 * Convert coordinates from feet to pixels with depth info.
 */
void pix_to_ft_depth(std::vector<Target> &r_targets, cv::Mat depth, double effective_height, int hfov, int vfov);

/**
 * Warp the perspective of Targets from in_size to out_size.
 */
void warp_coords(std::vector<Target> &r_targets, cv::Size in_size, cv::Size out_size);


/**
 * Annotate frame with debug information.
 */
void annotate_frame(std::vector<Target> targets, cv::Mat frame, int bgr_color[3], float pix_per_ft_x, float pix_per_ft_y);
}

#endif
