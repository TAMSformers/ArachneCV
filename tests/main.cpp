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
  int cam_size[2] = {638,479};
  int cam_coords[2] = {400,400};
  acv::Camera camera1( 1, cam_size, cam_coords, 45);
  while( true )
  {
    camera1.GetFrame( );
    camera1.WarpPerspective( );
    camera1.ShowFrame( );
  }
  return 0;
}
