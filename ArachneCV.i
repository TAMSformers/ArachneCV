/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 */

%module ArachneCV

%{
#include "camera.hpp"
#include "targets.hpp"
#include "image_analysis.hpp"

using namespace acv;
%}

%include "camera.hpp"
%include "targets.hpp"
%include "image_analysis.hpp"

%Camera::Camera(int cam_num_in, double cam_coords_in[3], int cam_angle_in, int orientation, int pix_per_ft);
%Camera::GetFrame();
%Camera::GetFrameFromImage();
%Camera::WarpPerspective();
%Camera::FindBalls();
%Camera::FindRobots();
%Camera::ShowFrame();
%Camera::~Camera();

%Targets::merge(Target input_targets[16]);
