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

void Targets::merge(std::vector<Target> input_targets)
{
  /* Mark duplicates */
  for (int i = 0; i < input_targets.size(); i++) {
    for (int j = 0; j < targets.size(); j++) {
      if (sqrt(pow(input_targets[i].coords[0] - targets[j].coords[0], 2) +
               pow(input_targets[i].coords[1] - targets[j].coords[1], 2)) < 3) {
        input_targets[i].is_real = false;
      }
    }
  }

  /* Add all unmarked input_targets */
  for (int i = 0; i < input_targets.size(); i++) {
    if (input_targets[i].is_real) {
      targets.push_back(input_targets[i]);
    }
  }

  /* Debug information */
  for (int i = 0; i < targets.size(); i++) {
    std::cout << targets[i].color << " " << targets[i].type << " " << targets[i].coords[0] << " " << targets[i].coords[1] << std::endl;
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
