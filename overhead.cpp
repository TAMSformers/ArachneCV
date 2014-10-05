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

#include <vector>
#include <cmath>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//#include "ArachneCVConfig.h"

#define PI 3.14159265

namespace acv {

void WarpPerspective( cv::InputArray src, cv::OutputArray dst, cv::Size dsize, int angle )
{
  /* point arrays to generate transform matrix */
  std::vector<cv::Point2f> src_pts( 4 );
  std::vector<cv::Point2f> dst_pts( 4 );

  /* initialize source array */
  cv::Point2f S0( 0.0, 0.0 );
  cv::Point2f S1( (float)dsize.width, 0.0 );
  cv::Point2f S2( (float)dsize.width, (float)dsize.height );
  cv::Point2f S3( 0.0, (float)dsize.height );
  src_pts[ 0 ] = S0;
  src_pts[ 1 ] = S1;
  src_pts[ 2 ] = S2;
  src_pts[ 3 ] = S3;

  /* find height of triangle as a function of angle */
  int height = dsize.width * tan( angle * ( PI / 180.0 ) );

 /* intersect triangle with image */
  if( height <= dsize.height ){
    cv::Point2f D0( dsize.width / 2, height );
    cv::Point2f D1( dsize.width / 2, height );
    cv::Point2f D2( dsize.width, dsize.height );
    cv::Point2f D3( 0, dsize.height );
    dst_pts[ 0 ] = D0;
    dst_pts[ 1 ] = D1;
    dst_pts[ 2 ] = D2;
    dst_pts[ 3 ] = D3;
}
  else{
    int offset = ( dsize.height * ( dsize.width/2 ) ) / height;
    cv::Point2f D0( offset, height );
    cv::Point2f D1( dsize.width - offset, height );
    cv::Point2f D2( dsize.width, dsize.height );
    cv::Point2f D3( 0, dsize.height );
    dst_pts[ 0 ] = D0;
    dst_pts[ 1 ] = D1;
    dst_pts[ 2 ] = D2;
    dst_pts[ 3 ] = D3;
}

  /* generate transform matrix */
  cv::InputArray M = cv::getPerspectiveTransform( src_pts, dst_pts );

  /* apply transform matrix */
  cv::warpPerspective( src, dst, M, dsize );
}

}
