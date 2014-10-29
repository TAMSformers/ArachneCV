/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * Find balls and display.
 *
 */

#include <ArachneCV/ArachneCV.hpp>

int main( )
{
  int cam_coords[ 2 ];
  acv::Camera camera1( 1, cam_coords, 45);
  while( true )
  {
    camera1.GetFrame( );
    camera1.WarpPerspective( );
    camera1.FindBalls( );
    camera1.ShowFrame( );
  }
  return 0;
}
