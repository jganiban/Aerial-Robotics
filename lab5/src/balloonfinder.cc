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
// 		using namespace cv;
// 		namedWindow("Image",WINDOW_NORMAL);
// 	  // Implement the function here
// 		Mat framep;

// 		GaussianBlur(*image, framep, Size(11,11), 0, 0);
// 		// Convert the image from the original BGR color space to HSV, which
// 		// easier to segment based on color: color is encoded only in H.
// 		cvtColor(framep, framep, COLOR_BGR2HSV);
// 		// Filter only red
// 		Scalar redLower(0,100,100);
// 		Scalar redUpper(40,255,255);
// 		inRange(framep, redLower, redUpper, framep);
// 		// Erode image to eliminate stray wisps of red
// 		const int iterations = 10;
// 		erode(framep, framep, Mat(), cv::Point(-1,-1), iterations);
// 		// Dilate image to restore red square to original size
// 		dilate(framep, framep, Mat(), cv::Point(-1,-1), iterations);
// 		// Find contours
// 		std::vector<std::vector<cv::Point>> contours;
// 		std::vector<Vec4i> hierarchy;
// 		findContours(framep, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
// 		// Loop through the contours.  Bound each contour by both a bounding
// 		// (rotated) rectangle and a minimum enclosing circle.  Draw the contour
// 		// on the original image. If the bounding rectangle indicates that the
// 		// contour is square enough, and if the enclosing circle indicates it's
// 		// large enough, then also draw the minimum enclosing circle.
// 		Mat drawing = Mat::zeros(framep.size(), CV_8UC3);
// 		RNG rng(12345);
// 		Point2f center;
// 		float radius;
// 		const float maxAspectRatio = 2;
// 		const float minRadius = 200;
// 		const int minPointsFor_fitEllipse = 5;
// 		for(size_t ii = 0; ii < contours.size(); ++ii) {
// 		  Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256),
// 		                        rng.uniform(0, 256));
// 		  minEnclosingCircle(contours[ii], center, radius);
// 		  float aspectRatio = maxAspectRatio;
// 		  if(contours[ii].size() >= minPointsFor_fitEllipse) {
// 		    RotatedRect boundingRectangle = fitEllipse(contours[ii]);
// 		    const Size2f rectSize = boundingRectangle.size;
// 		    aspectRatio = static_cast<float>(std::max(rectSize.width,rectSize.height))/
// 		      std::min(rectSize.width,rectSize.height);
// 		  }
// 		  std::cout << aspectRatio << " " << radius << std::endl;
// 		  drawContours(*image, contours, ii, color, 2, LINE_8, hierarchy, 0);
// 		  if(aspectRatio < maxAspectRatio && radius > minRadius) {
// 		    circle(*image, center, (int)radius, color, 2);
// 		  }
// 		}


// 		imshow("Image", *image);
// 		waitKey(0);

// 		return true;

// 	}

// 	if(color == BLUE){
// 		using namespace cv;
// 		namedWindow("Image",WINDOW_NORMAL);
// 	  // Implement the function here
// 		Mat framep;

// 		GaussianBlur(*image, framep, Size(11,11), 0, 0);
// 		// Convert the image from the original BGR color space to HSV, which
// 		// easier to segment based on color: color is encoded only in H.
// 		cvtColor(framep, framep, COLOR_BGR2HSV);
// 		// Filter only red
// 		Scalar blueLower(95,100,100);
// 		Scalar blueUpper(135,255,255);
// 		inRange(framep, blueLower, blueUpper, framep);
// 		// Erode image to eliminate stray wisps of red
// 		const int iterations = 10;
// 		erode(framep, framep, Mat(), cv::Point(-1,-1), iterations);
// 		// Dilate image to restore red square to original size
// 		dilate(framep, framep, Mat(), cv::Point(-1,-1), iterations);
// 		// Find contours
// 		std::vector<std::vector<cv::Point>> contours;
// 		std::vector<Vec4i> hierarchy;
// 		findContours(framep, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
// 		// Loop through the contours.  Bound each contour by both a bounding
// 		// (rotated) rectangle and a minimum enclosing circle.  Draw the contour
// 		// on the original image. If the bounding rectangle indicates that the
// 		// contour is square enough, and if the enclosing circle indicates it's
// 		// large enough, then also draw the minimum enclosing circle.
// 		Mat drawing = Mat::zeros(framep.size(), CV_8UC3);
// 		RNG rng(1234);
// 		Point2f center;
// 		float radius;
// 		const float maxAspectRatio = 2;
// 		const float minRadius = 200;
// 		const int minPointsFor_fitEllipse = 5;
// 		for(size_t ii = 0; ii < contours.size(); ++ii) {
// 		  Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256),
// 		                        rng.uniform(0, 256));
// 		  minEnclosingCircle(contours[ii], center, radius);
// 		  float aspectRatio = maxAspectRatio;
// 		  if(contours[ii].size() >= minPointsFor_fitEllipse) {
// 		    RotatedRect boundingRectangle = fitEllipse(contours[ii]);
// 		    const Size2f rectSize = boundingRectangle.size;
// 		    aspectRatio = static_cast<float>(std::max(rectSize.width,rectSize.height))/
// 		      std::min(rectSize.width,rectSize.height);
// 		  }
// 		  std::cout << aspectRatio << " " << radius << std::endl;
// 		  drawContours(*image, contours, ii, color, 2, LINE_8, hierarchy, 0);
// 		  if(aspectRatio < maxAspectRatio && radius > minRadius) {
// 		    circle(*image, center, (int)radius, color, 2);
// 		  }
// 		}


// 		imshow("Image", *image);
// 		waitKey(0);

// 		return true;

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
	Mat framep;
	BalloonFinder balloonfinder;



	std::shared_ptr<CameraBundle> cb = std::make_shared<CameraBundle>();
	cb->RCI = RCI;
	cb->rc = rc;


	GaussianBlur(*image, framep, Size(11,11), 0, 0);
	// Convert the image from the original BGR color space to HSV, which
	// easier to segment based on color: color is encoded only in H.
	cvtColor(framep, framep, COLOR_BGR2HSV);
	// Filter only red
	Scalar redLower(0,100,100);
	Scalar redUpper(40,255,255);
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
	const float maxAspectRatio = 2;
	const float minRadius = 200;
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

		cb->rx = {xc,yc};
		bundles->push_back(cb);
		colors->push_back(RED);
		
	  }
	  std::cout << aspectRatio << " " << radius << std::endl;
	  drawContours(*image, contours, ii, color, 2, LINE_8, hierarchy, 0);
	  if(aspectRatio < maxAspectRatio && radius > minRadius) {
	    circle(*image, center, (int)radius, color, 2);
	  }
	}
	std::cout << cb->rx << std::endl;

	// Eigen boundingRectangle->origin;


	imshow("Image", *image);
	waitKey(0);



}
