#pragma once
#include "DetectMotobike.h"
#include "CountWhiteValue.h"
#include <iostream>
#include<conio.h>  
using namespace std;
static void drawResult(Mat image, vector<Rect> rects);
int draw = 0;
int main()
{
	namedWindow("root image", CV_WINDOW_NORMAL);
	namedWindow("binary image", CV_WINDOW_NORMAL);
	VideoCapture capVideo;
	capVideo.open("20170228-090949.mp4");

	if (!capVideo.isOpened())
	{                                                 // if unable to open video file
		std::cout << "error reading video file" << std::endl << std::endl;      // show error message
		_getch();                   // it may be necessary to change or remove this line if not using Windows
		return(0);                                                              // and exit program
	}

	if (capVideo.get(CV_CAP_PROP_FRAME_COUNT) < 2)
	{
		std::cout << "error: video file must have at least two frames";
		_getch();                   // it may be necessary to change or remove this line if not using Windows
		return(0);
	}

	
	MotoBinaryDetecter detecter = MotoBinaryDetecter(16, 80);//36, 100
	while (capVideo.isOpened())
	{
		Mat image;
		capVideo.read(image);
		Mat draw = image.clone();
		cvtColor(image, image, CV_BGR2GRAY);
		imshow("root image", image);
		waitKey(0);
		GaussianBlur(image, image, Size(3, 3), 2);
		Canny(image, image, 50, 120, 3, false);// binary image
		imwrite("testDetectMultiScale.png", image);
		imshow("binary image", image);
		waitKey(0);

		///detecter.detectSingleScale(image, 1);
		detecter.detectMultiScale(image, 3);
		detecter.reduceRectDetect();

		drawResult(draw, detecter.getRectsDetected());
	}
}


static void drawResult(Mat image, vector<Rect> rects)
{
	namedWindow("drawResult", CV_WINDOW_NORMAL);
	size_t size = rects.size();
	for (int i = 0; i < size; ++i)
	{
		rectangle(image, rects.at(i), cv::Scalar(255, 255, 0));
		imshow("drawResult", image);
		waitKey(-1);
	}

 }