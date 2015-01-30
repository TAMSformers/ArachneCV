/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 * This is ArachneCV, a computer vision library for the FIRST        *
 * Robotics Competition. Source hosted at                            *
 * https://github.com/tamsformers/ArachneCV                          *
 *                                                                   *
 * Copyright 2014, 2015 by David Hashe and Jacob Brunson.            *
 *                                                                   *
 * This program is free software; you can redistribute it and/or     *
 * modify it under the terms of the GNU General Public License as    *
 * published by the Free Software Foundation - version 3 or          *
 * (at your option) any later version.                               *
 *                                                                   *
 * This program is distributed in the hope that it will be useful,   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     *
 * GNU General Public License in file COPYING for more details.      *
 *                                                                   *
 * You should have received a copy of the GNU General Public         *
 * License along with this program; if not, write to the Free        *
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,       *
 * Boston, MA 02111, USA.                                            *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * This file implements object locations.
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
