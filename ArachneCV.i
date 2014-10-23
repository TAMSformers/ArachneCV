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

%Camera::Camera( int cam_num_in, int cam_size_in[ 2 ], int cam_coords_in[ 2 ], int cam_angle_in );
%Camera::GetFrame( );
%Camera::WarpPerspective( );
%Camera::ShowFrame( );
%Camera::~Camera( );

