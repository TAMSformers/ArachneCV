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

#include "targets.hpp"

namespace acv{

std::vector<Target> mergeTargets(std::vector<Target> targets_1,
                                   std::vector<Target> targets_2)
{
  /* Mark duplicates */
  for (int i = 0; i < targets_1.size(); i++) {
    for (int j = 0; j < targets_2.size(); j++) {
      if ((sqrt(pow(targets_1[i].coords[0] - targets_2[j].coords[0], 2) +
                pow(targets_1[i].coords[1] - targets_2[j].coords[1], 2)) < 3) &&
          targets_1[i].type == targets_2[j].type) {
        /* average the coordinates of the two duplicates */
        targets_1[i].coords[0] = (targets_1[i].coords[0] + targets_2[j].coords[0]) / 2;
        targets_1[i].coords[1] = (targets_1[i].coords[1] + targets_2[j].coords[1]) / 2;
        if (targets_1[i].coords[2] != 0 && targets_2[j].coords[2] != 0) {
          targets_1[i].coords[2] = (targets_1[i].coords[2] + targets_2[j].coords[2]) / 2;
        } else {
          targets_1[i].coords[2] = targets_1[i].coords[2] + targets_2[j].coords[2];
        }
      }
    }
  }

  /* Union of two inputs */
  std::vector<Target> targets_return;

  /* Add all unmarked targets */
  for (int i = 0; i < targets_1.size(); i++) {
    targets_return.push_back(targets_1[i]);
  }
  for (int i = 0; i < targets_2.size(); i++) {
    targets_return.push_back(targets_2[i]);
  }

  /* Debug information */
/*  for (int i = 0; i < targets_return.size(); i++) {
    std::cout << targets_return[i].type << " " <<
                 targets_return[i].coords[0] << " " <<
                 targets_return[i].coords[1] << " " <<
                 targets_return[i].coords[2] << " " <<
                 targets_return[i].angle << " " <<
                 targets_return[i].orientation <<
                 std::endl;
  }*/
  std::cout << std::endl;

  return targets_return;
}

}
