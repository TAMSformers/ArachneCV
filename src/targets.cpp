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

std::vector<Target> Targets::get()
{
  return targets;
}

void Targets::merge(std::vector<Target> input_targets)
{
  /* Make copy that we can change without side effects */
  std::vector<Target> merge_targets(input_targets);

  /* Mark duplicates */
  for (int i = 0; i < merge_targets.size(); i++) {
    for (int j = 0; j < targets.size(); j++) {
      if (sqrt(pow(merge_targets[i].coords[0] - targets[j].coords[0], 2) +
               pow(merge_targets[i].coords[1] - targets[j].coords[1], 2)) < 3) {
        merge_targets[i].is_real = false;
      }
    }
  }

  /* Add all unmarked merge_targets */
  for (int i = 0; i < merge_targets.size(); i++) {
    if (merge_targets[i].is_real) {
      targets.push_back(merge_targets[i]);
    }
  }

  /* Debug information */
  for (int i = 0; i < targets.size(); i++) {
    std::cout << targets[i].type << " " << targets[i].color << " " << targets[i].coords[0] << " " << targets[i].coords[1] << std::endl;
  }
  std::cout << std::endl;
}

void Targets::clear()
{
  while (targets.size()) {
    targets.pop_back();
  }
}

}
