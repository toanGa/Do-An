#pragma once
#include "DetectMotobike.h"
#include "CountWhiteValue.h"

void drawResult(Mat image, vector<Rect> rects);
int draw = 0;
void main()
{
	namedWindow("root image", CV_WINDOW_NORMAL);
	namedWindow("cut image", CV_WINDOW_NORMAL);
#if 1
	Mat root = imread("imgframe3.png", CV_LOAD_IMAGE_GRAYSCALE);
	///imshow("root image", root);
	///waitKey(0);
	GaussianBlur(root, root, Size(3, 3), 2);
	///imshow("root image", root);
	///waitKey(0);
	double t1 = 10, t2 = 100;
	Canny(root, root, t1, t2, 3, false);// binary image
	imshow("root image", root);
	waitKey(0);
	Rect wrap = Rect(4, 2, 61, 110);
	Mat cut = root(wrap);
	imshow("root image", root);
	imshow("cut image", cut);
	waitKey(0);
	imwrite("cutImage.png", cut);
#if 1
	// orient face
	Mat drawMat = imread("imgframe3.png");
	MotoBinaryDetecter detecter = MotoBinaryDetecter(60, 110);
	detecter.detectSingleScale(root, 1);
	draw = 0;
	drawResult(drawMat, detecter.rectDetect);// before process
	detecter.reduceRectDetect();

	draw = 1;
	Mat drawMat1 = imread("imgframe3.png");
	drawResult(drawMat1, detecter.getRectsDetected());// after process

	///vector<Rect> listDetectd = detecter.getRectsDetected();
	///drawResult(root, detecter.getRectsDetected());
#endif

#if 0// back orient
	Mat drawMat = imread("imgframe3.png");
	MotoBinaryDetecter detecter = MotoBinaryDetecter(36, 100);
	detecter.detectSingleScale(root, 1);
	drawResult(drawMat, detecter.rectDetect);// before process
	detecter.reduceRectDetect();
	drawResult(drawMat, detecter.getRectsDetected());// before process

#endif

#if 0
	Mat drawMat = imread("imgframe5.png", CV_LOAD_IMAGE_GRAYSCALE);
   	Rect center = detecter.findMaser(detecter.getRectsDetected());
	vector <Rect> draw;
	draw.push_back(center);
	drawResult(drawMat, draw);
	//waitKey(-1);
 	bool b = checkMotobike(cut, ORIENT_BACK);
#endif
#endif
	///////////// start detect



	system("pause");
}

void drawResult(Mat image, vector<Rect> rects)
{
	namedWindow("drawResult", CV_WINDOW_NORMAL);
	size_t size = rects.size();
	for (int i = 0; i < size; ++i)
	{
		rectangle(image, rects.at(i), cv::Scalar(255, 255, 0));
		//imshow("drawResult", image);
		//waitKey(-1);
	}
	//imshow("drawResult", image);
	//waitKey(-1);
	if (0 == draw)
	{
		imwrite("imageBeforeReduce.png", image);
	}
	else
	{
		imwrite("imageAfterReduce.png", image);
	}
	
}