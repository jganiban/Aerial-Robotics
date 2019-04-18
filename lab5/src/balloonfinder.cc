#include <cstdlib>
#include <string>
#include <iostream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include "balloonfinder.h"
#include "structurecomputer.h"



// bool BallonFinderOfColor(const cv::Mat* image,
// 						const BalloonColor color,
// 						Eigen::Matrix2d* rx){
	
// 	if(color == RED){

// 	}

// 	if(color == BLUE){

// 	}

// 	return false;

// }

void BalloonFinder::
findBalloons(const cv::Mat* image,
             const Eigen::Matrix3d RCI,
             const Eigen::Vector3d rc,
             std::vector<std::shared_ptr<const CameraBundle>>* bundles,
             std::vector<BalloonColor>* colors) {

	using namespace cv;
	namedWindow("Image",WINDOW_NORMAL);
  // Implement the function here
	BalloonFinder balloonfinder;



	std::shared_ptr<CameraBundle> cb = std::make_shared<CameraBundle>();
	cb->RCI = RCI;
	cb->rc = rc;

	Mat framep;

	GaussianBlur(*image, framep, Size(11,11), 0, 0);
	// Convert the image from the original BGR color space to HSV, which
	// easier to segment based on color: color is encoded only in H.
	cvtColor(framep, framep, COLOR_BGR2HSV);
	
	/////////////////////////////////////////////////////////////////
	// Filter only red
	/////////////////////////////////////////////////////////////////

	Scalar redLower(0,100,100);
	Scalar redUpper(25,255,255);
	inRange(framep, redLower, redUpper, framep);
	// Erode image to eliminate stray wisps of red
	const int iterations = 10;
	erode(framep, framep, Mat(), cv::Point(-1,-1), iterations);
	// Dilate image to restore red square to original size
	dilate(framep, framep, Mat(), cv::Point(-1,-1), iterations);
	// Find contours
	std::vector<std::vector<cv::Point>> contours;
	std::vector<Vec4i> hierarchy;
	findContours(framep, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	// Loop through the contours.  Bound each contour by both a bounding
	// (rotated) rectangle and a minimum enclosing circle.  Draw the contour
	// on the original image. If the bounding rectangle indicates that the
	// contour is square enough, and if the enclosing circle indicates it's
	// large enough, then also draw the minimum enclosing circle.
	Mat drawing = Mat::zeros(framep.size(), CV_8UC3);
	RNG rng(12345);
	Point2f center;
	float radius;
	const float maxAspectRatio = 3;
	const float minRadius = 100;
	const int minPointsFor_fitEllipse = 5;
	for(size_t ii = 0; ii < contours.size(); ++ii) {
	  Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256),
	                        rng.uniform(0, 256));
	  minEnclosingCircle(contours[ii], center, radius);
	  float aspectRatio = maxAspectRatio;
	  if(contours[ii].size() >= minPointsFor_fitEllipse) {
	    RotatedRect boundingRectangle = fitEllipse(contours[ii]);
	    const Size2f rectSize = boundingRectangle.size;
	    aspectRatio = static_cast<float>(std::max(rectSize.width,rectSize.height))/
	      std::min(rectSize.width,rectSize.height);
		float xc    = boundingRectangle.center.x;
		float yc    = boundingRectangle.center.y;

		Point2f centerOfBalloon(xc,yc);

		cb->rx = {xc,yc};
		circle(*image, centerOfBalloon, 5, color, 10);
		
	  }
	  std::cout << aspectRatio << " " << radius << std::endl;
	  drawContours(*image, contours, ii, color, 2, LINE_8, hierarchy, 0);
	  if(aspectRatio < maxAspectRatio && radius > minRadius) {
	    circle(*image, center, (int)radius, color, 2);
		bundles->push_back(cb);
		colors->push_back(RED);
	  }
	}
	std::cout << cb->rx << std::endl;

	/////////////////////////////////////////////////////////////////
	// Filter only blue
	/////////////////////////////////////////////////////////////////
	std::shared_ptr<CameraBundle> cb2 = std::make_shared<CameraBundle>();
	cb2->RCI = RCI;
	cb2->rc = rc;

	Mat framep2;

	GaussianBlur(*image, framep2, Size(11,11), 0, 0);
	// Convert the image from the original BGR color space to HSV, which
	// easier to segment based on color: color is encoded only in H.
	cvtColor(framep2, framep2, COLOR_BGR2HSV);
	

	Scalar blueLower(95,100,100);
	Scalar blueUpper(135,255,255);
	inRange(framep2, blueLower, blueUpper, framep2);
	// Erode image to eliminate stray wisps of red
	const int iterations2 = 10;
	erode(framep2, framep2, Mat(), cv::Point(-1,-1), iterations);
	// Dilate image to restore red square to original size
	dilate(framep2, framep2, Mat(), cv::Point(-1,-1), iterations);
	// Find contours
	std::vector<std::vector<cv::Point>> contours2;
	std::vector<Vec4i> hierarchy2;
	findContours(framep2, contours2, hierarchy2, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	// Loop through the contours.  Bound each contour by both a bounding
	// (rotated) rectangle and a minimum enclosing circle.  Draw the contour
	// on the original image. If the bounding rectangle indicates that the
	// contour is square enough, and if the enclosing circle indicates it's
	// large enough, then also draw the minimum enclosing circle.
	Mat drawing2 = Mat::zeros(framep2.size(), CV_8UC3);
	Point2f center2;
	float radius2;
	const float maxAspectRatio2 = 3;
	const float minRadius2 = 100;
	const int minPointsFor_fitEllipse2 = 5;
	for(size_t ii = 0; ii < contours2.size(); ++ii) {
	  Scalar color2 = Scalar(rng.uniform(0, 256), rng.uniform(0, 256),
	                        rng.uniform(0, 256));
	  minEnclosingCircle(contours2[ii], center2, radius2);
	  float aspectRatio2 = maxAspectRatio2;
	  if(contours2[ii].size() >= minPointsFor_fitEllipse2) {
	    RotatedRect boundingRectangle2 = fitEllipse(contours2[ii]);
	    const Size2f rectSize2 = boundingRectangle2.size;
	    aspectRatio2 = static_cast<float>(std::max(rectSize2.width,rectSize2.height))/
	      std::min(rectSize2.width,rectSize2.height);
		float xc2   = boundingRectangle2.center.x;
		float yc2    = boundingRectangle2.center.y;

		Point2f centerOfBalloon2(xc2,yc2);

		cb2->rx = {xc2,yc2};
		circle(*image, centerOfBalloon2, 5, color2, 10);
		
	  }
	  std::cout << aspectRatio2 << " " << radius2 << std::endl;
	  drawContours(*image, contours2, ii, color2, 2, LINE_8, hierarchy2, 0);
	  if(aspectRatio2 < maxAspectRatio2 && radius2 > minRadius2) {
	    circle(*image, center2, (int)radius2, color2, 2);
		bundles->push_back(cb2);
		colors->push_back(BLUE);
	  }
	}

	std::cout << "////////////////////" << std::endl;
	std::cout << "size of bundles " << bundles->size() << std::endl;
	std::cout << std::endl;

	imshow("Image", *image);
	waitKey(0);



}
