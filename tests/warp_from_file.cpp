/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * Main test. Temporary; we need a testing suite.
 *
 */

#include <ArachneCV/ArachneCV.hpp>

int main( )
{
  int cam_coords[ 2 ];
  acv::Camera camera1( 0, cam_coords, 45);
  while( true )
  {
    camera1.GetFrameFromImage( "image.jpg" );
    camera1.WarpPerspective( );
    camera1.FindBalls( );
    camera1.ShowFrame( );
  }
  return 0;
}
