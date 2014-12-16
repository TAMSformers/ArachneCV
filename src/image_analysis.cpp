/*
 * Arachne CV
 *
 * Written by David Hashe and Jacob Brunson
 *
 *
 *
 * Object-finding algorithms
 *
 */

#include <opencv2/opencv.hpp>

#include "targets.hpp"

namespace acv {

void findPossibleBallsInFrame(cv::Mat frame, std::vector<Target> &r_targets, std::string color, double distance, int pix_per_ft);
void findPossibleRobotsInFrame(cv::Mat frame, std::vector<Target> &r_targets, std::string color, double distance, int pix_per_ft);

void findTargetsInFrame(cv::Mat frame, std::vector<Target> &r_targets, std::string color, double distance, int pix_per_ft)
{
  std::vector<Target> ball_targets;
  std::vector<Target> robot_targets;

  /* Find each category of targets. These functions are not exposed globablly and WILL give incorrect results without the below checks. */
  findPossibleBallsInFrame(frame, ball_targets, color, distance, pix_per_ft);
  findPossibleRobotsInFrame(frame, robot_targets, color, distance, pix_per_ft);

  /* If a ball is not very close to any "robots", it is probably of the other color. */
  /* If a "robot" is very close to a ball, it probably is the ball and not a robot. */
  for (int i = 0; i < ball_targets.size(); i++) {
    if (ball_targets[i].type == "ball") {
      ball_targets[i].is_real = false;
      for (int j = 0; j < robot_targets.size(); j++) {
        if (robot_targets[j].type != "" && sqrt(pow(robot_targets[j].coords[0] - ball_targets[i].coords[0], 2) + pow(robot_targets[j].coords[1] - ball_targets[i].coords[1], 2)) < 1) {
          robot_targets[j].is_real = false;
          ball_targets[i].is_real = true;
        }
      }
    }
  }

  /* Annotate frame with identified targets (debugging) */
  for (int i = 0; i < ball_targets.size(); i++) {
    if (ball_targets[i].is_real) {
      //std::cout << "Ball: " << color << " " << ball_targets[i].coords[0] << " " << ball_targets[i].coords[1] << std::endl;
      float pixels_x = (distance * (frame.size().width / frame.size().height)) * ball_targets[i].coords[0] + (frame.size().width / 2);
      float pixels_y = frame.size().height - distance * ball_targets[i].coords[1];
      cv::Point center(cvRound(pixels_x), cvRound(pixels_y));
      cv::circle(frame, center, 3, cv::Scalar(0, 0, 0), -1, 8, 0);
      cv::circle(frame, center, 50, cv::Scalar(0, 0, 0), 3, 8, 0);
    }
  }
  for (int i = 0; i < robot_targets.size(); i++) {
    if (robot_targets[i].is_real) {
      //std::cout << "Robot: " << color << " " << robot_targets[i].coords[0] << " " << robot_targets[i].coords[1] << std::endl;
      float pixels_x = (distance * (frame.size().width / frame.size().height)) * robot_targets[i].coords[0] + (frame.size().width / 2);
      float pixels_y = frame.size().height - distance * robot_targets[i].coords[1];
      cv::Point center(cvRound(pixels_x), cvRound(pixels_y));
      cv::circle(frame, center, 3, cv::Scalar(255, 255, 255), -1, 8, 0);
      cv::circle(frame, center, 50, cv::Scalar(255, 255, 255), 3, 8, 0);
    }
  }
  if (color == "blue") {
    //std::cout << std::endl;
  }

  /* Add converted targets to array */
  for (int i = 0; i < ball_targets.size(); i++) {
    if (ball_targets[i].is_real) {
      r_targets.push_back(ball_targets[i]);
    }
  }
  for (int i = 0; i < robot_targets.size(); i++) {
    if (robot_targets[i].is_real) {
      r_targets.push_back(robot_targets[i]);
    }
  }
}

void findPossibleBallsInFrame(cv::Mat frame, std::vector<Target> &r_targets, std::string color, double distance, int pix_per_ft)
{
  /* modified frame for mask */
  cv::Mat mask;
  /* modified frame end result */
//  cv::Mat frame_out;

  /* bounds for inRange (in HSV color space) */
  int lower_bound_array[3] = {-1, 120, 0};
  int upper_bound_array[3] = {-1, 255, 255};

  if (color == "red") {
    lower_bound_array[0] = 160;
    upper_bound_array[0] = 180;
  } else if (color == "blue") {
    lower_bound_array[0] = 100;
    upper_bound_array[0] = 140;
  } else {
    exit(1);
  }

  std::vector<int> lower_bound(lower_bound_array, lower_bound_array + 3);
  std::vector<int> upper_bound(upper_bound_array, upper_bound_array + 3);

  /* create mask */
  cv::cvtColor(frame, mask, CV_BGR2HSV, 0);
  cv::medianBlur(mask, mask, 99);
  cv::inRange(mask, lower_bound, upper_bound, mask);

  /* apply mask */
//  cv::bitwise_and(frame, frame, frame_out, mask);

  /* vector to hold detected circles */
  std::vector<cv::Vec3f> circles;

  /* detect circles */
  cv::HoughCircles(mask, circles, CV_HOUGH_GRADIENT, 4, 500, 500, 100, 50, 300);

  /* add circles to targets as balls */
  for (int i = 0; i < circles.size(); i++) {
    Target tmp_target;
    tmp_target.type =  "ball";
    tmp_target.color = color;
    tmp_target.coords[0] = (circles[i][0] - (frame.size().width / 2)) / (distance * (frame.size().width / frame.size().height));
    tmp_target.coords[1] = (frame.size().height - circles[i][1]) / distance;
    /* can't determine z coordinate from above, so set to zero */
    tmp_target.coords[2] = 0;
    r_targets.push_back(tmp_target);
  }
}

void findPossibleRobotsInFrame(cv::Mat frame, std::vector<Target> &r_targets, std::string color, double distance, int pix_per_ft)
{
  /* modified frame for mask */
  cv::Mat mask;
  /* modified frame end result */
//  cv::Mat frame_out;

  /* bounds for inRange (in HSV color space) */
  int lower_bound_array[3] = {-1, 120, 0};
  int upper_bound_array[3] = {-1, 255, 255};

  if (color == "red") {
    lower_bound_array[0] = 165;
    upper_bound_array[0] = 180;
  } else if (color == "blue") {
    lower_bound_array[0] = 100;
    upper_bound_array[0] = 140;
  } else {
    exit(1);
  }

  std::vector<int> lower_bound(lower_bound_array, lower_bound_array + 3);
  std::vector<int> upper_bound(upper_bound_array, upper_bound_array + 3);

  /* create mask */
  cv::cvtColor(frame, mask, CV_BGR2HSV, 0);
  cv::medianBlur(mask, mask, 45);
  cv::inRange(mask, lower_bound, upper_bound, mask);

  /* apply mask */
//  cv::bitwise_and(frame, frame, frame_out, mask);

  /* vector to hold detected robots */
  std::vector<cv::Vec2f> robots;
  /* vector of vectors to hold contours (robot candidates) */
  std::vector<std::vector<cv::Point>> contours;

  /* detect robots*/
  cv::findContours(mask, contours, cv::noArray(), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
  for (int i = 0; i < contours.size(); i++) {
    if (cv::contourArea(contours[i]) / pow(pix_per_ft, 2) > 2) {
      cv::Rect br = cv::boundingRect(contours[i]);
      cv::Vec2f center = cv::Vec2f(br.x + br.width / 2, br.y + br.height / 2);
      robots.push_back(center);
    }
  }

  /* add robots to targets */
  for (int i = 0; i < robots.size(); i++) {
    Target tmp_target;
    tmp_target.type =  "robot";
    tmp_target.color = color;
    tmp_target.coords[0] = (robots[i][0] - (frame.size().width / 2)) / (distance * (frame.size().width / frame.size().height));
    tmp_target.coords[1] = (frame.size().height - robots[i][1]) / distance;
    /* can't determine z coordinate from above, so set to zero */
    tmp_target.coords[2] = 0;
    r_targets.push_back(tmp_target);
  }
}

}
