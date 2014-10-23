/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * This file implements the camera frameframework.
 *
 */

#include <vector>
#include <string>

#include <opencv2/opencv.hpp>

#include "camera.hpp"
#include "targets.hpp"

#include "ArachneCVConfig.h"

namespace acv{

Camera::Camera( int cam_num_in, int cam_size_in[ 2 ], int cam_coords_in[ 2 ], int cam_angle_in )
{
  cam_num = cam_num_in;
  cam_size.width = cam_size_in[ 0 ];
  cam_size.height = cam_size_in[ 1 ];
  cam_coords[ 0 ] = cam_coords_in[ 0 ];
  cam_coords[ 1 ] = cam_coords_in[ 1 ];
  cam_angle = cam_angle_in;

  capture = cam_num_in;
  if( !capture.isOpened( ) )
    exit( 1 );
}

void Camera::GetFrame( )
{
  capture >> frame;
  cv::imshow("frame", frame);
  cv::waitKey( 30 );
}

void Camera::WarpPerspective( )
{
  /* point arrays to generate transform matrix */
  std::vector<cv::Point2f> src_pts( 4 );
  std::vector<cv::Point2f> dst_pts( 4 );

  /* initialize destination array */
  cv::Point2f D0( 0.0, 0.0 );
  cv::Point2f D1( (float)cam_size.width, 0.0 );
  cv::Point2f D2( (float)cam_size.width, (float)cam_size.height );
  cv::Point2f D3( 0.0, (float)cam_size.height );
  dst_pts[ 0 ] = D0;
  dst_pts[ 1 ] = D1;
  dst_pts[ 2 ] = D2;
  dst_pts[ 3 ] = D3;

  /* find transform from reference angle */
  if( cam_angle = 45 )
  {
    cv::Point2f S0( 75.0, 200 );
    cv::Point2f S1( cam_size.width - 75.0, 200 );
    cv::Point2f S2((float)cam_size.width, (float)cam_size.height );
    cv::Point2f S3( 0.0 , (float)cam_size.height );
    src_pts[ 0 ] = S0;
    src_pts[ 1 ] = S1;
    src_pts[ 2 ] = S2;
    src_pts[ 3 ] = S3;
  } else {
    /* hard-coded to 45 for now, exit if different */
    exit(1);
  }

  /* generate transform matrix */
  cv::Mat M = cv::getPerspectiveTransform( src_pts, dst_pts );
  M.convertTo( M, CV_32F );

  cv::Size dsize = frame.size();
  /* apply transform matrix */
  cv::warpPerspective( frame, warped, M, dsize );
}

void Camera::ShowFrame( )
{
  /* TODO add camera name to window name */
  cv::imshow( "warped", warped );
  cv::waitKey(30);
}

void Camera::FindBalls( )
{
  /*TODO link to algorithms, probably implemented in their own class */
  /*TODO determine which objects in merge_targets are already in target, average the positions and velocities of the two, and add any that are absent */
}

void Camera::ClearTargets( )
{
  /* don't bother setting all parameters to NULL; this is enough to identify */
  for( int i = 0; i < 16; i++ ) {
    targets[ i ].type = "";
    targets[ i ].color = "";
  }
}

void Camera::FindRobots( )
{
  Target merge_targets[ 16 ];

  /*TODO link to algorithms, probably implemented in their own class */
  /*TODO determine which objects in merge_targets are already in target, average the positions and velocities of the two, and add any that are absent */
}

}
