/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * Currently unfinished. Don't actually run this one.
 *
 */

#include <ArachneCV.hpp>

int main( )
{
  acv::Camera *camera0 = new acv::Camera( $params );
  acv::Camera *camera1 = new acv::Camera( $params );

  acv::Targets *targets = new acv::Targets( );

  while( true ) {
    camera0.GetFrame( );
    camera1.GetFrame( );

    camera0.WarpPerspective( );
    camera1.WarpPerspective( );

    camera0.FindBalls( );
    camera0.FindRobots( );

    camera1.FindBalls( );
    camera1.FindRobots( );

    targets.Merge( camera0.targets );
    targets.Merge( camera1.targets );

    camera0.ClearTargets( );
    camera1.ClearTargets( );
  }

  delete camera0;
  delete camera1;

  return 0;
}



