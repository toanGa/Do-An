#pragma once
// main.cpp
//#include"connerDetect.h"



#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect.hpp"
#include<iostream>
#include<conio.h>           // it may be necessary to change or remove this line if not using Windows

#include "Blob.h"
#include"mp4Export.h"
#define SHOW_STEPS            // un-comment or comment this line to show steps or not
#include <string.h>
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
void assignCarDetectedForRect(Mat rootMat, vector<Blob> listBlobs, vector<Rect> &totalRectCars);
void drawListRectToMat(Mat &mat, vector<Rect> listRect);
void drawListBlobToMat(Mat &mat, vector<Blob> listBlob);
void assignCarDetectedForRect(Mat rootMat, Rect frameDetect, vector<Rect> &totalRectCars);

void printInfo(vector<Blob> blobs, string info);

String cars_cascade_name = "cars.xml";
CascadeClassifier cars_cascade;


///////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{

	cv::VideoCapture capVideo;

	cv::Mat imgFrame1;
	cv::Mat imgFrame2;

	std::vector<Blob> blobs;

	cv::Point crossingLine[2];



	int carCount = 0;
	//CarsDrivingUnderBridge
	//capVideo.open("CarsDrivingUnderBridge.mp4");
	capVideo.open("hamKimLien1.mp4");

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

	if (!cars_cascade.load(cars_cascade_name))
	{
		printf("--(!)Error loading car cascade\n");
		system("pause");
		return -1;
	};

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

	///Mp4Exporter videoExporter(Size(imgFrame1.cols, imgFrame1.rows), 15.0);
	///bool bCheck = videoExporter.open("detectMotobikeAndCar.avi");

	int instanceFrame = 0;



	while (capVideo.isOpened() && chCheckForEscKey != 27)
	{
		instanceFrame++;
		std::vector<Blob> currentFrameBlobs;
		std::vector<Blob> exeptionBlobs;
		std::vector<Rect> listRectCar;

		cv::Mat imgFrame1Copy = imgFrame1.clone();
		cv::Mat imgFrame2Copy = imgFrame2.clone();

		cv::Mat imgDifference;
		cv::Mat imgThresh;

		cv::cvtColor(imgFrame1Copy, imgFrame1Copy, CV_BGR2GRAY);
		cv::cvtColor(imgFrame2Copy, imgFrame2Copy, CV_BGR2GRAY);

		cv::GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(5, 5), 0);
		cv::GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(5, 5), 0);

		cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);

		cv::threshold(imgDifference, imgThresh, 30, 255.0, CV_THRESH_BINARY);

		cv::imshow("imgThresh", imgThresh);

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

		cv::Mat imgThreshCopy = imgThresh.clone();

		std::vector<std::vector<cv::Point> > contours;

		cv::findContours(imgThreshCopy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		drawAndShowContours(imgThresh.size(), contours, "imgContours");

		std::vector<std::vector<cv::Point> > convexHulls(contours.size());

		for (unsigned int i = 0; i < contours.size(); i++)
		{
			cv::convexHull(contours[i], convexHulls[i]);
		}

		drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");

		for (auto &convexHull : convexHulls)
		{
			Blob possibleBlob(convexHull);

			if (possibleBlob.currentBoundingRect.area() > 225 &&
				possibleBlob.dblCurrentAspectRatio > 0.25 &&
				possibleBlob.dblCurrentAspectRatio < 0.85 &&
				possibleBlob.currentBoundingRect.width > 15 &&
				possibleBlob.currentBoundingRect.height > 20 &&
				possibleBlob.dblCurrentDiagonalSize > 15.0 &&
				(cv::contourArea(possibleBlob.currentContour) / (double)possibleBlob.currentBoundingRect.area()) > 0.50
				// toan add here
				//s&& (possibleBlob.currentBoundingRect.height > possibleBlob.currentBoundingRect.width)
				&& (possibleBlob.currentBoundingRect.width < 35)
				&& (possibleBlob.currentBoundingRect.height < 75)
				&& (possibleBlob.currentBoundingRect.y > 200)
				)

			{
				if (possibleBlob.currentBoundingRect.y < 240)
				{
					if (possibleBlob.currentBoundingRect.width < 20 &&
						possibleBlob.currentBoundingRect.height < 30
						)
					{
						currentFrameBlobs.push_back(possibleBlob);
					}
				}
				else
					currentFrameBlobs.push_back(possibleBlob);
			}
			else
			{
				exeptionBlobs.push_back(possibleBlob);
			}
		}

		drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");
		printInfo(currentFrameBlobs, "current blob at frame" + to_string(instanceFrame));
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

		//printInfo(blobs, "after matching blob at frame " + to_string(instanceFrame));


		drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");

		imgFrame2Copy = imgFrame2.clone();          // get another copy of frame 2 since we changed the previous frame 2 copy in the processing above

		drawBlobInfoOnImage(blobs, imgFrame2Copy);
#if 0
		bool blnAtLeastOneBlobCrossedTheLine = checkIfBlobsCrossedTheLine(blobs, intHorizontalLinePosition, carCount);

		if (blnAtLeastOneBlobCrossedTheLine == true)
		{
			cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_GREEN, 2);
		}
		else
		{
			cv::line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_RED, 2);
		}
#endif
		///drawCarCountOnImage(carCount, imgFrame2Copy);
		///assignCarDetectedForRect(imgFrame2Copy, exeptionBlobs, listRectCar);
		///drawListRectToMat(imgFrame2Copy, listRectCar);
		//drawListBlobToMat(imgFrame2Copy, exeptionBlobs);
		assignCarDetectedForRect(imgFrame2, Rect(159, 159, 480, 320), listRectCar);
		drawListRectToMat(imgFrame2Copy, listRectCar);
		cv::imshow("imgFrame2Copy", imgFrame2Copy);
		///videoExporter.write(imgFrame2Copy);
		//cv::waitKey(0);                 // uncomment this line to go frame by frame for debugging

		// now we prepare for the next iteration

		currentFrameBlobs.clear();
		exeptionBlobs.clear();
		listRectCar.clear();

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
	///videoExporter.close();
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
	cout << "match current blob to existing blob" << endl;
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
			cout << "x = " << currentFrameBlob.currentBoundingRect.x << ",y = " << currentFrameBlob.currentBoundingRect.y << ", is old object" << endl;
		}
		else
		{
			addNewBlob(currentFrameBlob, existingBlobs);
			cout << "x = " << currentFrameBlob.currentBoundingRect.x << ",y = " << currentFrameBlob.currentBoundingRect.y << ", is new object" << endl;
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
			cv::rectangle(imgFrame2Copy, blobs[i].currentBoundingRect, SCALAR_GREEN);

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

void assignCarDetectedForRect(Mat rootMat, vector<Blob> listBlobs, vector<Rect> &totalRectCars)
{
	Mat temMat = rootMat.clone();
	std::vector<Rect> tempRectCars;

	size_t size = listBlobs.size();
	if (CV_8UC1 != rootMat.type())
	{
		cvtColor(temMat, temMat, CV_BGR2GRAY);
	}
	for (int i = 0; i < size; ++i)
	{
		Mat matDetect = temMat(listBlobs.at(i).currentBoundingRect);
		cars_cascade.detectMultiScale(matDetect, tempRectCars, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
		int sizeDetected = tempRectCars.size();
		for (int j = 0; j < sizeDetected; ++j)
		{
			tempRectCars.at(j).x += listBlobs.at(i).currentBoundingRect.x;
			tempRectCars.at(j).y += listBlobs.at(i).currentBoundingRect.y;
			totalRectCars.push_back(tempRectCars.at(j));
		}
		tempRectCars.clear();
	}
}

void drawListRectToMat(Mat &mat, vector<Rect> listRect)
{
	size_t size = listRect.size();
	for (int i = 0; i < size; ++i)
	{
		Point vertex1(listRect[i].x, listRect[i].y);
		Point vertex2(listRect[i].x + listRect[i].width,
			listRect[i].y + listRect[i].height);
		rectangle(mat, vertex1, vertex2, SCALAR_RED,2);
	}
}

void drawListBlobToMat(Mat &mat, vector<Blob> listBlob)
{
	size_t size = listBlob.size();
	for (int i = 0; i < size; ++i)
	{
		Rect rect = listBlob.at(i).currentBoundingRect;
		Point vertex1(rect.x, rect.y);
		Point vertex2(rect.x + rect.width,
			rect.y + rect.height);
		rectangle(mat, vertex1, vertex2, SCALAR_WHITE, 2);
	}
}

void assignCarDetectedForRect(Mat rootMat, Rect frameDetect, vector<Rect> &totalRectCars)
{
	Mat temMat = rootMat(frameDetect);
	size_t size;
	if (CV_8UC1 != rootMat.type())
	{
		cvtColor(temMat, temMat, CV_BGR2GRAY);
	}
	cars_cascade.detectMultiScale(temMat, totalRectCars,
		1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	size = totalRectCars.size();
	for (int i = 0; i < size; ++i)
	{
		totalRectCars.at(i).x += frameDetect.x;
		totalRectCars.at(i).y += frameDetect.y;
	}
}


void printInfo(vector<Blob> blobs, string info)
{
	cout << info << endl;
	for (int i = 0; i < blobs.size(); ++i)
	{
		cout << "x: " << blobs.at(i).currentBoundingRect.x << ",y:" << blobs.at(i).currentBoundingRect.y << endl;
	}
	cout << "end of " << info << endl;
}