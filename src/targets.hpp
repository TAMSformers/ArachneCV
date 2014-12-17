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

/**
 * A ball, robot, etc.
 */
typedef struct
{
  /**
   * Identity of target. Usually either robot or ball.
   */
  std::string type;

  /**
   * Either red or blue.
   */
  std::string color;

  /**
   * Coordinates in 3D space relative to the center of the robot.
   */
  double coords[3];

  /**
   * Used within image_analysis to mark false positives.
   */
  bool is_real = true;
} Target;

/**
 * A collection of targets.
 */
class Targets
{
  private:

    /**
     * Vector of targets. Usually holds targets from several different
     * cameras.
     */
    std::vector<Target> m_targets;

  public:

    /**
     * Retrieve targets stored internally by merge().
     */
    std::vector<acv::Target> get();

    /**
     * Add targets to internal collection.
     *
     * @param[in] input_targets Targets to be added.
     */
    void merge(std::vector<Target> input_targets);

    /**
     * Clear targets stored internally by merge().
     */
    void clear();
};

}

#endif
