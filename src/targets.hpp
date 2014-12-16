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
  double coords[3]; /**< coordinates in 3-space */
  bool is_real = true;
} Target;

class Targets
{
  private:
    std::vector<Target> targets;
  public:
    std::vector<acv::Target> get();
    void merge(std::vector<Target> input_targets);
    void clear();
};

}

#endif