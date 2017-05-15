#pragma once
#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>

using namespace cv;
void main()
{
	namedWindow("anh xam", CV_WINDOW_NORMAL);
	namedWindow("bien trong anh voi nguong 1", CV_WINDOW_NORMAL);
	namedWindow("bien trong anh voi nguong 2", CV_WINDOW_NORMAL);
	Mat gray = imread("imgframe4.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat dst1, dst2;
	imshow("anh xam", gray);
	GaussianBlur(gray, gray, Size(9, 9), 2);
	double t1 = 30, t2 = 200;
	Canny(gray, dst1, t1, t2, 3, false);
	//t1 = 100;
	//t1 = 120;
	t1 = 100; t2 = 120;
	Canny(gray, dst2, t1, t2, 3, false);
	imshow("bien trong anh voi nguong 1", dst1);
	imshow("bien trong anh voi nguong 2", dst2);
	waitKey(0);
}