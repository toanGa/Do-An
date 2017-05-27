#pragma once
// main.cpp
//#include"connerDetect.h"

//#include "premetiveCode.h"
//#include "findContours.h"
//#include "cannyTimbienanh.h"
//#include "DetectBinaryExam.h"
//#include "FindPixel10x10.h"
//#include "testRestore.h"
//#include"testDetectMotobike.h"
//#include"testDetectMultiScale.h"

#if 1
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include"DetectMotobike.h"
#include<iostream>
#include<conio.h>           // it may be necessary to change or remove this line if not using Windows

#include "Blob.h"

#define SHOW_STEPS            // un-comment or comment this line to show steps or not
using namespace std;
using namespace cv;

// global variables ///////////////////////////////////////////////////////////////////////////////
const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
const cv::Scalar SCALAR_YELLOW = cv::Scalar(0.0, 255.0, 255.0);
const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 200.0, 0.0);
const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);

// function prototypes ////////////////////////////////////////////////////////////////////////////
void matchCurrentFrameBlobsToExistingBlobs(std::vector<Blob> &existingBlobs, std::vector<Blob> &currentFrameBlobs);
void addBlobToExistingBlobs(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs, int &intIndex);
void addNewBlob(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs);
double distanceBetweenPoints(cv::Point point1, cv::Point point2);
void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName);
void drawAndShowContours(cv::Size imageSize, std::vector<Blob> blobs, std::string strImageName);
bool checkIfBlobsCrossedTheLine(std::vector<Blob> &blobs, int &intHorizontalLinePosition, int &carCount);
void drawBlobInfoOnImage(std::vector<Blob> &blobs, cv::Mat &imgFrame2Copy);
void drawCarCountOnImage(int &carCount, cv::Mat &imgFrame2Copy);
static void drawResult(Mat image, vector<Rect> rects);
///////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{

	///cv::namedWindow("img1", cv::WINDOW_NORMAL);
	///cv::namedWindow("img2", cv::WINDOW_NORMAL);
	///cv::namedWindow("img3", cv::WINDOW_NORMAL);
	///cv::namedWindow("img4", cv::WINDOW_NORMAL);
	///cv::namedWindow("img5", cv::WINDOW_NORMAL);
	///cv::namedWindow("img6", cv::WINDOW_NORMAL);
	///cv::namedWindow("img7", cv::WINDOW_NORMAL);
	///cv::namedWindow("img8", cv::WINDOW_NORMAL);
	///cv::namedWindow("img9", cv::WINDOW_NORMAL);
	///cv::namedWindow("img10", cv::WINDOW_NORMAL);

	//cv::namedWindow("rootImage", cv::WINDOW_NORMAL);
	//cv::namedWindow("rootImage1", cv::WINDOW_NORMAL);
	//cv::namedWindow("grayImage", cv::WINDOW_NORMAL);
	system("pause");
	cv::VideoCapture capVideo;

	cv::Mat imgFrame1;
	cv::Mat imgFrame2;
	cv::Mat imgFrameCvt;
	cv::Mat imgGray;
	std::vector<Blob> blobs;

	cv::Point crossingLine[2];

	int carCount = 0;
	//CarsDrivingUnderBridge
	//20170301-073531
	//20170301-073635
	//20170301-073739
	//20170301-073843
	//20170301-073947
	//20170301-074051
	//20170301-074155
	//20170301-074259
	cout << "start" << endl;
	//capVideo.open("20170228-090949.mp4");
	capVideo.open("hamKimLien1.mp4");
	double fps = capVideo.get(CV_CAP_PROP_FPS);


	int dWidth = capVideo.get(CV_CAP_PROP_FRAME_WIDTH);
	int dHeight = capVideo.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "Frame Size = " << dWidth << "x" << dHeight << endl;
	// create video writer 
	Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));
	VideoWriter oVideoWriter("MyOutputVideo.avi", CV_FOURCC('M', 'P', '4', '2'), 15, frameSize, true);
	//VideoWriter my("ny",fsfs,)
	if (!oVideoWriter.isOpened()) //if not initialize the VideoWriter successfully, exit the program
	{
		cout << "ERROR: Failed to write the video" << endl;
		return -1;
	}



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

	capVideo.read(imgFrame1);
	capVideo.read(imgFrame2);

	int intHorizontalLinePosition = (int)std::round((double)imgFrame1.rows * 0.35);

	crossingLine[0].x = 0;
	crossingLine[0].y = intHorizontalLinePosition;

	crossingLine[1].x = imgFrame1.cols - 1;
	crossingLine[1].y = intHorizontalLinePosition;

	char chCheckForEscKey = 0;

	bool blnFirstFrame = true;

	int frameCount = 2;

	int numFrame = 0;
	while (capVideo.isOpened() && chCheckForEscKey != 27)
	{

		std::vector<Blob> currentFrameBlobs;

		cv::Mat imgFrame1Copy = imgFrame1.clone();
		cv::Mat imgFrame2Copy = imgFrame2.clone();


		cv::Mat imgDifference;
		cv::Mat imgThresh;
		cv::Mat imgAfterSub;
		cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgAfterSub);
		///cv::imshow("first sub", imgAfterSub);

		cv::cvtColor(imgFrame1Copy, imgFrame1Copy, CV_BGR2GRAY);
		cv::cvtColor(imgFrame2Copy, imgFrame2Copy, CV_BGR2GRAY);

		cv::GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(5, 5), 0);
		cv::GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(5, 5), 0);

		cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);

		cv::threshold(imgDifference, imgThresh, 30, 255.0, CV_THRESH_BINARY);


		cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
		cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
		cv::Mat structuringElement7x7 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
		cv::Mat structuringElement15x15 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));

		for (unsigned int i = 0; i < 2; i++)
		{
			cv::dilate(imgThresh, imgThresh, structuringElement5x5);
			cv::dilate(imgThresh, imgThresh, structuringElement5x5);
			cv::erode(imgThresh, imgThresh, structuringElement5x5);
		}

		// sau khi thu hien cac phep toan co dan
		///cv::imshow("imgThresh", imgThresh);// anh nhi phan tu anh xam tru hai anh lien tiep
		//cv::waitKey(0);
		cv::Mat imgThreshCopy = imgThresh.clone();

		std::vector<std::vector<cv::Point> > contours;

		cv::findContours(imgThreshCopy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		// after find countoirs from subtraction image, show countours
		drawAndShowContours(imgThresh.size(), contours, "imgContours");
		//cv::waitKey(0);
		std::vector<std::vector<cv::Point> > convexHulls(contours.size());

		for (unsigned int i = 0; i < contours.size(); i++)
		{
			cv::convexHull(contours[i], convexHulls[i]);
		}

		// after convex Hull list contours
		///drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");
		//cv::waitKey(0);
		for (auto &convexHull : convexHulls)
		{
			Blob possibleBlob(convexHull);

			if (possibleBlob.currentBoundingRect.area() > 225 &&
				possibleBlob.dblCurrentAspectRatio > 0.25 &&
				possibleBlob.dblCurrentAspectRatio < 1 &&
				possibleBlob.currentBoundingRect.width > 20 &&
				possibleBlob.currentBoundingRect.height > 20 &&
				possibleBlob.dblCurrentDiagonalSize > 15.0 &&
				(cv::contourArea(possibleBlob.currentContour) / (double)possibleBlob.currentBoundingRect.area()) > 0.50
				// toan add here
				&& (possibleBlob.currentBoundingRect.height > possibleBlob.currentBoundingRect.width)
				&& (possibleBlob.currentBoundingRect.width < 40)
				&& (possibleBlob.currentBoundingRect.height < 50)
				)
			{
				currentFrameBlobs.push_back(possibleBlob);
			}
		}

		///drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");
		//cv::waitKey(0);
		if (blnFirstFrame == true)
		{
			for (auto &currentFrameBlob : currentFrameBlobs)
			{
				blobs.push_back(currentFrameBlob);
			}
		}
		else
		{
			matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
		}

		// after assign current blobs to exist blobs
		///drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");
		//cv::waitKey(0);
		imgFrame2Copy = imgFrame2.clone();          // get another copy of frame 2 since we changed the previous frame 2 copy in the processing above

		///drawBlobInfoOnImage(blobs, imgFrame2Copy);


		// start check to show color line
		bool blnAtLeastOneBlobCrossedTheLine = checkIfBlobsCrossedTheLine(blobs, intHorizontalLinePosition, carCount);

		if (blnAtLeastOneBlobCrossedTheLine == true)
		{
			cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_GREEN, 2);
		}
		else
		{
			cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_RED, 2);
		}// stop draw line


		///drawCarCountOnImage(carCount, imgFrame2Copy);

		//cv::waitKey(0);                 // uncomment this line to go frame by frame for debugging

		// now we prepare for the next iteration

		// detect on moving here
		int x = 1;
		for (auto blob : currentFrameBlobs)
		{
			Rect currBoundMoving = blob.currentBoundingRect;
			MotoBinaryDetecter detecter = MotoBinaryDetecter(15, 45);//60, 100,(25, 70)
			double t1 = 10, t2 = 100;
			Mat tempMov = imgFrame1(currBoundMoving);
			cvtColor(tempMov, tempMov, CV_BGR2GRAY);
			GaussianBlur(tempMov, tempMov, Size(3, 3), 2);
			Canny(tempMov, tempMov, 50, 120, 3, false);// binary image
			//detecter.detectMultiScale(tempMov, 3);
			detecter.detectSingleScale(tempMov, 1);
			detecter.reduceRectDetect();
			//detecter.reduceRectDetect();

			vector<Rect> drawRect = detecter.getRectsDetected();
			size_t sizeOfRectDetected = detecter.getRectsDetected().size();
  			for (int i = 0; i < sizeOfRectDetected; ++i)
			{
				drawRect.at(i).x += currBoundMoving.x;
				drawRect.at(i).y += currBoundMoving.y;
			}

			
			drawResult(imgFrame1, drawRect);
			oVideoWriter.write(imgFrame1);
			drawRect.clear();

		}

		currentFrameBlobs.clear();

		imgFrame1 = imgFrame2.clone();           // move frame 1 up to where frame 2 is

		if ((capVideo.get(CV_CAP_PROP_POS_FRAMES) + 1) < capVideo.get(CV_CAP_PROP_FRAME_COUNT))
		{
			capVideo.read(imgFrame2);
		}
		else
		{
			std::cout << "end of video\n";
			break;
		}

		blnFirstFrame = false;
		frameCount++;
		chCheckForEscKey = cv::waitKey(1);


	}
	oVideoWriter.release();
	if (chCheckForEscKey != 27)
	{               // if the user did not press esc (i.e. we reached the end of the video)
		cv::waitKey(0);                         // hold the windows open to allow the "end of video" message to show
	}
	// note that if the user did press esc, we don't need to hold the windows open, we can simply let the program end which will close the windows

	return(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void matchCurrentFrameBlobsToExistingBlobs(std::vector<Blob> &existingBlobs, std::vector<Blob> &currentFrameBlobs)
{

	for (auto &existingBlob : existingBlobs)
	{

		existingBlob.blnCurrentMatchFoundOrNewBlob = false;

		existingBlob.predictNextPosition();
	}

	for (auto &currentFrameBlob : currentFrameBlobs)
	{

		int intIndexOfLeastDistance = 0;
		double dblLeastDistance = 100000.0;

		for (unsigned int i = 0; i < existingBlobs.size(); i++)
		{

			if (existingBlobs[i].blnStillBeingTracked == true)
			{

				double dblDistance = distanceBetweenPoints(currentFrameBlob.centerPositions.back(), existingBlobs[i].predictedNextPosition);

				if (dblDistance < dblLeastDistance)
				{
					dblLeastDistance = dblDistance;
					intIndexOfLeastDistance = i;
				}
			}
		}

		if (dblLeastDistance < currentFrameBlob.dblCurrentDiagonalSize * 0.5)
		{
			addBlobToExistingBlobs(currentFrameBlob, existingBlobs, intIndexOfLeastDistance);
		}
		else
		{
			addNewBlob(currentFrameBlob, existingBlobs);
		}

	}

	for (auto &existingBlob : existingBlobs)
	{

		if (existingBlob.blnCurrentMatchFoundOrNewBlob == false)
		{
			existingBlob.intNumOfConsecutiveFramesWithoutAMatch++;
		}

		if (existingBlob.intNumOfConsecutiveFramesWithoutAMatch >= 5)
		{
			existingBlob.blnStillBeingTracked = false;
		}

	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void addBlobToExistingBlobs(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs, int &intIndex)
{

	existingBlobs[intIndex].currentContour = currentFrameBlob.currentContour;
	existingBlobs[intIndex].currentBoundingRect = currentFrameBlob.currentBoundingRect;

	existingBlobs[intIndex].centerPositions.push_back(currentFrameBlob.centerPositions.back());

	existingBlobs[intIndex].dblCurrentDiagonalSize = currentFrameBlob.dblCurrentDiagonalSize;
	existingBlobs[intIndex].dblCurrentAspectRatio = currentFrameBlob.dblCurrentAspectRatio;

	existingBlobs[intIndex].blnStillBeingTracked = true;
	existingBlobs[intIndex].blnCurrentMatchFoundOrNewBlob = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void addNewBlob(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs)
{

	currentFrameBlob.blnCurrentMatchFoundOrNewBlob = true;

	existingBlobs.push_back(currentFrameBlob);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
double distanceBetweenPoints(cv::Point point1, cv::Point point2)
{

	int intX = abs(point1.x - point2.x);
	int intY = abs(point1.y - point2.y);

	return(sqrt(pow(intX, 2) + pow(intY, 2)));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName)
{
	cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);

	cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);

	cv::imshow(strImageName, image);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawAndShowContours(cv::Size imageSize, std::vector<Blob> blobs, std::string strImageName)
{

	cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);

	std::vector<std::vector<cv::Point> > contours;

	for (auto &blob : blobs)
	{
		if (blob.blnStillBeingTracked == true)
		{
			contours.push_back(blob.currentContour);
		}
	}

	cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);

	cv::imshow(strImageName, image);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool checkIfBlobsCrossedTheLine(std::vector<Blob> &blobs, int &intHorizontalLinePosition, int &carCount)
{
	bool blnAtLeastOneBlobCrossedTheLine = false;

	for (auto blob : blobs)
	{

		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2)
		{
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;

			if (blob.centerPositions[prevFrameIndex].y > intHorizontalLinePosition && blob.centerPositions[currFrameIndex].y <= intHorizontalLinePosition)
			{
				carCount++;
				blnAtLeastOneBlobCrossedTheLine = true;
			}
		}

	}

	return blnAtLeastOneBlobCrossedTheLine;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawBlobInfoOnImage(std::vector<Blob> &blobs, cv::Mat &imgFrame2Copy)
{

	for (unsigned int i = 0; i < blobs.size(); i++)
	{

		if (blobs[i].blnStillBeingTracked == true)
		{
			cv::rectangle(imgFrame2Copy, blobs[i].currentBoundingRect, SCALAR_RED, 2);

			int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
			double dblFontScale = blobs[i].dblCurrentDiagonalSize / 60.0;
			int intFontThickness = (int)std::round(dblFontScale * 1.0);

			cv::putText(imgFrame2Copy, std::to_string(i), blobs[i].centerPositions.back(), intFontFace, dblFontScale, SCALAR_GREEN, intFontThickness);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawCarCountOnImage(int &carCount, cv::Mat &imgFrame2Copy)
{

	int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
	double dblFontScale = (imgFrame2Copy.rows * imgFrame2Copy.cols) / 300000.0;
	int intFontThickness = (int)std::round(dblFontScale * 1.5);

	cv::Size textSize = cv::getTextSize(std::to_string(carCount), intFontFace, dblFontScale, intFontThickness, 0);

	cv::Point ptTextBottomLeftPosition;

	ptTextBottomLeftPosition.x = imgFrame2Copy.cols - 1 - (int)((double)textSize.width * 1.25);
	ptTextBottomLeftPosition.y = (int)((double)textSize.height * 1.25);

	cv::putText(imgFrame2Copy, std::to_string(carCount), ptTextBottomLeftPosition, intFontFace, dblFontScale, SCALAR_GREEN, intFontThickness);

}

#endif



// draw for resutl find motibike
static void drawResult(Mat image, vector<Rect> rects)
{
	namedWindow("drawResult", CV_WINDOW_NORMAL);
	size_t size = rects.size();
	for (int i = 0; i < size; ++i)
	{
		rectangle(image, rects.at(i), cv::Scalar(255, 255, 0));
		imshow("drawResult", image);
		//waitKey(-1);
	}

}



