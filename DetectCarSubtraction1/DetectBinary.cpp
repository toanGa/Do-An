#include"DetectBinary.h"


bool isBinaryImage(Mat img) // returns true if the given 3 channel image is B = G = R
{
	if (img.type() == CV_8UC1)
	{
		return true;
	}
	return false;
}


int CalcTotalBinary(Mat binaryMat)
{
	if (isBinaryImage(binaryMat))
	{
		return countNonZero(binaryMat);
	}
	return -1;
}

DetectBinary::DetectBinary(Mat binaryMat, Size_detect minSize, int totalSize)
{
	//int channel = binaryMat.channels;
	widthImage = binaryMat.rows;
	heightImage = binaryMat.cols;
	minSizeDetect.size_x = minSize.size_x;
	minSizeDetect.size_y = minSize.size_y;

	totalSizeDetect = totalSize;
	binary = binaryMat.clone();
}

DetectBinary::DetectBinary()
{

}
