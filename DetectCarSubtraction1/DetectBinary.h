#pragma once
#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
#include"Defines.h"
using namespace cv;
using namespace std;


bool isBinaryImage(Mat img);
int CalcTotalBinary(Mat binaryMat);

class DetectBinary
{
private:
	int widthImage;
	int heightImage;
	Size_detect minSizeDetect;
	int totalSizeDetect;
	Mat binary;
public:
	DetectBinary(Mat binaryMat, Size_detect minSize, int totalSize);
	DetectBinary();
};