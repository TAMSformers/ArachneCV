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
%}

%include "camera.hpp"

%Camera::Camera(int cam_num_in, double cam_coords_in[3], int cam_angle_in, string orientation);
%Camera::GetFrame();
%Camera::GetFrameFromImage();
%Camera::WarpPerspective();
%Camera::FindBalls();
%Camera::FindRobots();
%Camera::ShowFrame();
%Camera::~Camera();

