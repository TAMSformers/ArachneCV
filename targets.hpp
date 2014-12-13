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
  std::string type; /**< either robot, ball, or some other target */
  std::string color; /**< either red or blue */
  float coords[3]; /**< coordinates in 3-space */
  bool is_real = true;
} Target;

class Targets
{
  private:
    std::vector<acv::Target> targets;
  public:
//    Targets();
    void merge(std::vector<acv::Target> input_targets);
//   ~Targets();
};

}

#endif
