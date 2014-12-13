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

#include <opencv2/opencv.hpp>

#include "ArachneCVConfig.h"
#include "targets.hpp"

namespace acv{

void Targets::merge(std::vector<acv::Target> input_targets)
{
  /*TODO determine which objects in input_targets are already in targets, average the positions and velocities of the two, and add any that are absent */
}

}
