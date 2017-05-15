#pragma once
#include"DetectBinary.h"
using namespace std;
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
	DetectBinary detect1;
	int totalPointPass;
	if (isBinaryImage(dst1))
	{
		totalPointPass = countNonZero(dst1);
	}
	//count << "total point pass in dst1: " << totalPointPass << endl;

	//t1 = 100;
	//t1 = 120;
	t1 = 30; t2 = 50;
	Canny(gray, dst2, t1, t2, 3, false);
	totalPointPass = CalcTotalBinary(dst2);
	//count << "total point pass in 2: " << totalPointPass << endl;
	imshow("bien trong anh voi nguong 1", dst1);
	imshow("bien trong anh voi nguong 2", dst2);
	waitKey(0);
}