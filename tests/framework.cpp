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

int main()
{
  acv::Camera *camera0 = new acv::Camera($params);
  acv::Camera *camera1 = new acv::Camera($params);

  acv::Targets *targets = new acv::Targets();

  while (true) {
    camera0.getFrame();
    camera1.getFrame();

    camera0.warpPerspective();
    camera1.warpPerspective();

    camera0.findBalls();
    camera0.findRobots();

    camera1.findBalls();
    camera1.findRobots();

    targets.merge( camera0.targets );
    targets.merge( camera1.targets );

    camera0.ClearTargets( );
    camera1.ClearTargets( );
  }

  delete camera0;
  delete camera1;

  return 0;
}



