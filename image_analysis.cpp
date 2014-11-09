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

void findBallsInFrame(cv::Mat frame_in, Target targets[16], double cam_distance)
{
  /* modified frame for mask */
  cv::Mat frame_mask;
  /* modified frame end result */
  cv::Mat frame_out;

  /* color bounds for inRange */
  int lower_bound_array[3] = {165, 120, 0};
  int upper_bound_array[3] = {180, 255, 255};
  std::vector<int> lower_bound(lower_bound_array, lower_bound_array + 3);
  std::vector<int> upper_bound(upper_bound_array, upper_bound_array + 3);

  /* create mask */
  cv::cvtColor(frame_in, frame_mask, CV_BGR2HSV, 0);
  cv::medianBlur(frame_mask, frame_mask, 99);
  cv::inRange(frame_mask, lower_bound, upper_bound, frame_mask);

  /* apply mask */
  cv::bitwise_and(frame_in, frame_in, frame_out, frame_mask);

  /* vector to hold detected circles */
//  std::vector<float> circles( 8 );
  std::vector<cv::Vec3f> circles;

  /* detect circles */
  cv::HoughCircles(frame_mask, circles, CV_HOUGH_GRADIENT, 4, 500, 500, 100, 50, 300);

  /* add circles to image for debugging */
  for (int i = 0; i < circles.size(); i++) {
    cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
    int radius = cvRound(circles[i][2]);
    cv::circle(frame_in, center, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
    cv::circle(frame_in, center, radius, cv::Scalar(0, 0, 255), 3, 8, 0);
  }

  /* add circles to targets as balls */
  for (int i = 0; i < circles.size(); i++) {
    targets[i].type =  "ball";
    targets[i].color = "red";
//    printf("%f %f\n", circles[i][0], circles[i][1]);
    targets[i].coords[0] = (circles[i][0] - (frame_in.size().width / 2)) / (cam_distance * (frame_in.size().width / frame_in.size().height));
    targets[i].coords[1] = (frame_in.size().height - circles[i][1]) / cam_distance;
    /* can't determine z coordinate from above, so set to zero */
    targets[i].coords[2] = 0;
//    printf("%f %f\n", targets[i].coords[0], targets[i].coords[1]);
  }
}

void findRobotsInFrame(cv::Mat frame, Target targets[16], double cam_distance)
{

}

}
