/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * This file implements the camera framework.
 *
 */

#include <vector>
#include <cmath>

#include <opencv2/opencv.hpp>

#include "camera_acv.hpp"

//#include "ArachneCVConfig.h"

#define PI 3.14159265

namespace acv{

acvCamera::acvCamera( int cam_num_in, int cam_size_in[ 2 ], int cam_coords_in[ 2 ], int cam_angle_in )
{
  cam_size.width = cam_size_in[ 0 ];
  cam_size.height = cam_size_in[ 1 ];
  cam_coords[ 0 ] = cam_coords_in[ 0 ];
  cam_coords[ 1 ] = cam_coords_in[ 1 ];
  cam_angle = cam_angle_in;

  cv::VideoCapture capture( cam_num_in );

  if( !capture.isOpened( ) )
    exit( 1 );
}

void acvCamera::acvGetFrame( )
{
  capture.grab( );
  capture.retrieve( frame );
}

void acvCamera::acvWarpPerspective( )
{
  /* point arrays to generate transform matrix */
  std::vector<cv::Point2f> src_pts( 4 );
  std::vector<cv::Point2f> dst_pts( 4 );

  /* initialize source array */
  cv::Point2f S0( 0.0, 0.0 );
  cv::Point2f S1( (float)cam_size.width, 0.0 );
  cv::Point2f S2( (float)cam_size.width, (float)cam_size.height );
  cv::Point2f S3( 0.0, (float)cam_size.height );
  src_pts[ 0 ] = S0;
  src_pts[ 1 ] = S1;
  src_pts[ 2 ] = S2;
  src_pts[ 3 ] = S3;

  /* find height of triangle as a function of angle */
  int height = cam_size.width * tan( cam_angle * ( PI / 180.0 ) );

 /* intersect triangle with image */
  if( height <= cam_size.height ){
    cv::Point2f D0( cam_size.width / 2, height );
    cv::Point2f D1( cam_size.width / 2, height );
    cv::Point2f D2( cam_size.width, cam_size.height );
    cv::Point2f D3( 0, cam_size.height );
    dst_pts[ 0 ] = D0;
    dst_pts[ 1 ] = D1;
    dst_pts[ 2 ] = D2;
    dst_pts[ 3 ] = D3;
}
  else{
    int offset = ( cam_size.height * ( cam_size.width/2 ) ) / height;
    cv::Point2f D0( offset, height );
    cv::Point2f D1( cam_size.width - offset, height );
    cv::Point2f D2( cam_size.width, cam_size.height );
    cv::Point2f D3( 0, cam_size.height );
    dst_pts[ 0 ] = D0;
    dst_pts[ 1 ] = D1;
    dst_pts[ 2 ] = D2;
    dst_pts[ 3 ] = D3;
}

  /* generate transform matrix */
  cv::InputArray M = cv::getPerspectiveTransform( src_pts, dst_pts );

  /* apply transform matrix */
  cv::warpPerspective( frame, frame, M, cam_size );
}

}
