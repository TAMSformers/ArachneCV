/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * This file implements object locations.
 *
 */

#ifndef __ARACHNECV_TARGETS__
#define __ARACHNECV_TARGETS__

#include <string>

#include <opencv2/opencv.hpp>

namespace acv {

typedef struct {
  std::string type = ""; /**< either robot, ball, or some other target */
  std::string color = ""; /**< either red or blue */
  int coords[3]; /**< coordinates in 3-space */
  int velocity[3]; /**< velocity in 3-space */
} Target;

class Targets
{
  private:
    Target targets[16];
  public:
    Targets();
    void merge(Target input_targets[16]);
   ~Targets();
};

}

#endif
