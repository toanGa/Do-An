#pragma once
#include <iostream>
#include <fstream>
#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
#include "Defines.h"
using namespace cv;
using namespace std;
bool isGrayImage(Mat gray);

class DetectGray
{
private:
	Size_detect frameSize;
	Mat grayFrame;
	Point pointCalc;
public:
	DetectGray(Mat grayFrame, Size_detect sizeDetect,Point pointStart)
	{
		this->grayFrame = grayFrame.clone();
		frameSize = sizeDetect;
		pointCalc = pointStart;
	}

	void genarateFile(string nameFile)
	{
		Mat img(100, 100, CV_8UC1, Scalar(0));
		ofstream fileOut;
		fileOut.open(nameFile);
		for (int i = 0; i < frameSize.size_x; i++)
		{
			for (int j = 0; j < frameSize.size_y; j++)
			{
				int val;
				if ((pointCalc.x + i < grayFrame.rows) && (pointCalc.y + j < grayFrame.cols))
				{
					val = grayFrame.at<uchar>(pointCalc.x + i, pointCalc.y + j);
					img.at<uchar>(i, j) = val;
					//fileOut << val << " ";
				}
				
				
			}
			namedWindow("img", CV_WINDOW_NORMAL);
			imshow("img", img);
			fileOut << endl;
		}
		fileOut.close();
	}
};