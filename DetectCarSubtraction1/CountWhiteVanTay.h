#pragma once
#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void*);
std::string getImageType(int number)
{
	// find type
	int imgTypeInt = number % 8;
	std::string imgTypeString;

	switch (imgTypeInt)
	{
	case 0:
		imgTypeString = "8U";
		break;
	case 1:
		imgTypeString = "8S";
		break;
	case 2:
		imgTypeString = "16U";
		break;
	case 3:
		imgTypeString = "16S";
		break;
	case 4:
		imgTypeString = "32S";
		break;
	case 5:
		imgTypeString = "32F";
		break;
	case 6:
		imgTypeString = "64F";
		break;
	default:
		break;
	}

	// find channel
	int channel = (number / 8) + 1;

	std::stringstream type;
	type << "CV_" << imgTypeString << "C" << channel;

	return type.str();
}
/** @function main */
int main(int argc, char** argv)
{
	/// Load source image and convert it to gray
	namedWindow("vantay", WINDOW_NORMAL);
	src = imread("WX.PNG");

	cout << " src type: " << getImageType(src.type()) << endl;
	/// Convert image to gray and blur it
	if (CV_8UC1 != src.type())
	{
		cvtColor(src, src_gray, CV_BGR2GRAY);
	}

	int width = src.cols;
	int height = src.rows;
	double t1 = 50, t2 = 100;
	Canny(src_gray, src_gray, t1, t2, 3, false);// binary image

	int white = countNonZero(src_gray);

	imshow("vantay", src_gray);
	waitKey(0);
	return(0);
}
