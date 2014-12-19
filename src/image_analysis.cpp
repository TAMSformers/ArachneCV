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
#include <cmath>

#include "targets.hpp"

namespace acv {

void findPossibleBallsColor(cv::Mat frame, std::vector<Target> &r_targets,
                            std::string color);
void findPossibleRobotsColor(cv::Mat frame, std::vector<Target> &r_targets,
                             std::string color, float pix_per_ft_x, float pix_per_ft_y,
                             std::vector<std::vector<cv::Point>> &r_contours);
void convertPossibleTargetsWarp(std::vector<Target> &r_targets, cv::Mat frame,
                                float pix_per_ft_x, float pix_per_ft_y);

void findPossibleBallsDepth(cv::Mat depth, std::vector<Target> &r_targets, std::string color);
void convertPossibleTargetsDepth(std::vector<Target> &r_targets, cv::Mat depth);

void findTargetsWarp(cv::Mat frame, std::vector<Target> &r_targets, std::string color, double effective_height, int hfov, int vfov)
{
  std::vector<Target> ball_targets;
  std::vector<Target> robot_targets;
  std::vector<std::vector<cv::Point>> contours;

  float pix_per_ft_x = (frame.size().width /
                   (2 * effective_height * tan((hfov / 2) * 3.1415 / 180)));
  float pix_per_ft_y = (frame.size().height /
                   (2 * effective_height * tan((vfov / 2) * 3.1415 / 180)));

  /* Find each category of targets. These functions are not exposed globablly and WILL give incorrect results without the below checks. */
  findPossibleBallsColor(frame, ball_targets, color);
  convertPossibleTargetsWarp(ball_targets, frame, pix_per_ft_x, pix_per_ft_y);
  findPossibleRobotsColor(frame, robot_targets, color, pix_per_ft_x, pix_per_ft_y, contours);
  convertPossibleTargetsWarp(robot_targets, frame, pix_per_ft_x, pix_per_ft_y);

  std::cout << "Balls: " << ball_targets.size() << std::endl;
  std::cout << "Robots: " << robot_targets.size() << std::endl;

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
      std::cout << "Ball: " << color << " " << ball_targets[i].coords[0] << " " << ball_targets[i].coords[1] << std::endl;
      float pixels_x = pix_per_ft_x * ball_targets[i].coords[0] + (frame.size().width / 2);
      float pixels_y = frame.size().height - pix_per_ft_y * ball_targets[i].coords[1];
      cv::Point center(cvRound(pixels_x), cvRound(pixels_y));
      cv::circle(frame, center, 3, cv::Scalar(0, 0, 0), -1, 8, 0);
      cv::circle(frame, center, 50, cv::Scalar(0, 0, 0), 3, 8, 0);
    }
  }
  for (int i = 0; i < robot_targets.size(); i++) {
    if (robot_targets[i].is_real) {
      std::cout << "Robot: " << color << " " << robot_targets[i].coords[0] << " " << robot_targets[i].coords[1] << std::endl;
      float pixels_x = pix_per_ft_x * robot_targets[i].coords[0] + (frame.size().width / 2);
      float pixels_y = frame.size().height - pix_per_ft_y * robot_targets[i].coords[1];
      cv::Point center(cvRound(pixels_x), cvRound(pixels_y));
      cv::circle(frame, center, 3, cv::Scalar(255, 255, 255), -1, 8, 0);
      cv::circle(frame, center, 50, cv::Scalar(255, 255, 255), 3, 8, 0);
    }
  }
  if (color == "blue") {
    std::cout << std::endl;
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

void findTargetsDepth(cv::Mat frame, cv::Mat depth, std::vector<Target> &r_targets, std::string color, int hfov, int vfov)
{
  /* detect balls by depth */
  std::vector<Target> ball_depth_targets;
  findPossibleBallsDepth(depth, ball_depth_targets, color);

  /* detect color fields */
  std::vector<Target> robot_color_targets;
  std::vector<std::vector<cv::Point>> contours;
  findPossibleRobotsColor(frame, robot_color_targets, color, 5, 5, contours);

  /* determine which balls are the correct color */
  for (int i = 0; i < ball_depth_targets.size(); i++) {
    ball_depth_targets[i].is_real = false;
    for (int j = 0; j < robot_color_targets.size(); j++) {
      if (sqrt(pow(robot_color_targets[j].coords[0] -
                   ball_depth_targets[i].coords[0], 2) +
               pow(robot_color_targets[j].coords[1] -
                   ball_depth_targets[i].coords[1], 2)) <
          ball_depth_targets[i].coords[2]) {
        ball_depth_targets[i].is_real = true;
        break;
      }
    }
  }

  /* detect robots from size of color fields */
  for (int i = 0; i < robot_color_targets.size(); i++) {
    robot_color_targets[i].is_real = false;
    int robot_depth = depth.at<uchar>(robot_color_targets[i].coords[0],robot_color_targets[i].coords[1]) / 305.0;
    float pix_per_ft_x = (frame.size().width /
                     (2 * robot_depth * tan((hfov / 2) * 3.1415 / 180)));
    float pix_per_ft_y = (frame.size().height /
                     (2 * robot_depth * tan((vfov / 2) * 3.1415 / 180)));
    if (cv::contourArea(contours[i]) / (pix_per_ft_x * pix_per_ft_y) > .3) {
      robot_color_targets[i].is_real = true;
    }
  }

  /* convert coordinates */
  convertPossibleTargetsDepth(ball_depth_targets, depth);
  convertPossibleTargetsDepth(robot_color_targets, depth);

  /* If a "robot" is very close to a ball, it probably is the ball and not a robot. */
  for (int i = 0; i < ball_depth_targets.size(); i++) {
    ball_depth_targets[i].is_real = false;
    for (int j = 0; j < robot_color_targets.size(); j++) {
      if (sqrt(pow(robot_color_targets[j].coords[0] -
                   ball_depth_targets[i].coords[0], 2) +
               pow(robot_color_targets[j].coords[1] -
                   ball_depth_targets[i].coords[1], 2) +
               pow(robot_color_targets[j].coords[2] -
                   ball_depth_targets[i].coords[2], 2)) <
          ball_depth_targets[i].coords[2]) {
        robot_color_targets[j].is_real = false;
      }
    }
  }

  /* Add converted targets to array */
  for (int i = 0; i < ball_depth_targets.size(); i++) {
    if (ball_depth_targets[i].is_real) {
      r_targets.push_back(ball_depth_targets[i]);
    }
  }
  for (int i = 0; i < robot_color_targets.size(); i++) {
    if (robot_color_targets[i].is_real) {
      r_targets.push_back(robot_color_targets[i]);
    }
  }
}

void findPossibleBallsColor(cv::Mat frame, std::vector<Target> &r_targets, std::string color)
{
  /* modified frame for mask */
  cv::Mat mask;

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

  /* vector to hold detected circles */
  std::vector<cv::Vec3f> circles;

  /* detect circles */
  cv::HoughCircles(mask, circles, CV_HOUGH_GRADIENT, 4, 500, 500, 100, 50, 300);

  /* add circles to targets as balls */
  for (int i = 0; i < circles.size(); i++) {
    Target tmp_target;
    tmp_target.type = "ball";
    tmp_target.color = color;
    tmp_target.coords[0] = circles[i][0];
    tmp_target.coords[1] =  circles[i][1];
    /* can't determine z coordinate from above, so set to zero */
    tmp_target.coords[2] = 0;
    r_targets.push_back(tmp_target);
  }
}

void findPossibleRobotsColor(cv::Mat frame, std::vector<Target> &r_targets, std::string color, float pix_per_ft_x, float pix_per_ft_y, std::vector<std::vector<cv::Point>> &r_contours)
{
  /* modified frame for mask */
  cv::Mat mask;

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

  /* vector to hold detected robots */
  std::vector<cv::Vec2f> robots;

  /* vector of vectors to hold robot candidates */
  std::vector<std::vector<cv::Point>> contours;

  /* detect robots*/
  cv::findContours(mask, contours, cv::noArray(), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
  for (int i = 0; i < contours.size(); i++) {
    //std::cout << cv::contourArea(contours[i]) << std::endl;
    if (cv::contourArea(contours[i]) / (pix_per_ft_x * pix_per_ft_y) > .3) {
      cv::Rect br = cv::boundingRect(contours[i]);
      cv::Vec2f center = cv::Vec2f(br.x + br.width / 2, br.y + br.height / 2);
      robots.push_back(center);
      r_contours.push_back(contours[i]);
    }
  }

  /* add robots to targets */
  for (int i = 0; i < robots.size(); i++) {
    Target tmp_target;
    tmp_target.type = "robot";
    tmp_target.color = color;
    tmp_target.coords[0] = robots[i][0];
    tmp_target.coords[1] = robots[i][1];
    /* can't determine z coordinate from above, so set to zero */
    tmp_target.coords[2] = 0;
    r_targets.push_back(tmp_target);
  }
}

void convertPossibleTargetsWarp(std::vector<Target> &r_targets, cv::Mat frame, float pix_per_ft_x, float pix_per_ft_y)
{
  for (int i = 0; i < r_targets.size(); i++) {
    r_targets[i].coords[0] = (r_targets[i].coords[0] - (frame.size().width / 2)) / pix_per_ft_x;
    r_targets[i].coords[1] = (frame.size().height - r_targets[i].coords[1]) / pix_per_ft_y;
  }
}

void findPossibleBallsDepth(cv::Mat depth, std::vector<Target> &r_targets, std::string color)
{
  /* convert depth map from 16 bit to 8 bit so that we can analyze it */
  cv::Mat depth_8;
  depth.convertTo(depth_8, CV_8U, 1 / 256);

  /* vector to hold detected circles */
  std::vector<cv::Vec3f> circles;

  /* detect circles */
  cv::HoughCircles(depth_8, circles, CV_HOUGH_GRADIENT, 4, 500, 500, 100, 25, 1000);

  for (int i = 0; i < circles.size(); i++) {
    Target tmp_target;
    tmp_target.type = "ball";
    tmp_target.color = color;
    /* For depth only, we use (x, y, r) instead of (x, y, z) */
    tmp_target.coords[0] = circles[i][0];
    tmp_target.coords[1] = circles[i][1];
    tmp_target.coords[2] = circles[i][2];
    r_targets.push_back(tmp_target);
  }
}

void convertPossibleTargetsDepth(std::vector<Target> &r_targets, cv::Mat depth)
{
  for (int i = 0; i < r_targets.size(); i++) {
    double tmp_coords[3];
    tmp_coords[0] = r_targets[i].coords[0];
    tmp_coords[1] = r_targets[i].coords[1];
    tmp_coords[2] = r_targets[i].coords[2];

    r_targets[i].coords[1] = depth.at<uchar>(tmp_coords[0],tmp_coords[1]) / 305.0;

    int distance_x = 2 * r_targets[i].coords[1] * tan(57 / 2);
    r_targets[i].coords[0] = (tmp_coords[0] - (depth.size().width / 2)) / distance_x;

    int distance_y = 2 * r_targets[i].coords[1] * tan(43 / 2);
    r_targets[i].coords[2] = (tmp_coords[1] - (depth.size().width / 2)) / distance_y;
  }
}

}
