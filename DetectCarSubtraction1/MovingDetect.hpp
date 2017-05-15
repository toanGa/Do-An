#pragma once
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include"Blob.h"
using namespace std;
using namespace cv;




class MovingDetect {
private:
	Mat rootImage;
	vector<Blob> blobs;

public:
	/*
	* Defineition global value
	*/
	const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
	const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
	const cv::Scalar SCALAR_YELLOW = cv::Scalar(0.0, 255.0, 255.0);
	const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 200.0, 0.0);
	const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);


	void matchCurrentFrameBlobsToExistingBlobs(std::vector<Blob> &existingBlobs, std::vector<Blob> &currentFrameBlobs)
	{

		for (auto &existingBlob : existingBlobs) {

			existingBlob.blnCurrentMatchFoundOrNewBlob = false;

			existingBlob.predictNextPosition();
		}

		for (auto &currentFrameBlob : currentFrameBlobs) {

			int intIndexOfLeastDistance = 0;
			double dblLeastDistance = 100000.0;

			for (unsigned int i = 0; i < existingBlobs.size(); i++) {

				if (existingBlobs[i].blnStillBeingTracked == true) {

					double dblDistance = distanceBetweenPoints(currentFrameBlob.centerPositions.back(), existingBlobs[i].predictedNextPosition);

					if (dblDistance < dblLeastDistance) {
						dblLeastDistance = dblDistance;
						intIndexOfLeastDistance = i;
					}
				}
			}

			if (dblLeastDistance < currentFrameBlob.dblCurrentDiagonalSize * 0.5) {
				addBlobToExistingBlobs(currentFrameBlob, existingBlobs, intIndexOfLeastDistance);
			}
			else {
				addNewBlob(currentFrameBlob, existingBlobs);
			}

		}

		for (auto &existingBlob : existingBlobs) {

			if (existingBlob.blnCurrentMatchFoundOrNewBlob == false) {
				existingBlob.intNumOfConsecutiveFramesWithoutAMatch++;
			}

			if (existingBlob.intNumOfConsecutiveFramesWithoutAMatch >= 5) {
				existingBlob.blnStillBeingTracked = false;
			}

		}
	}

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

	void addNewBlob(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs)
	{
		currentFrameBlob.blnCurrentMatchFoundOrNewBlob = true;

		existingBlobs.push_back(currentFrameBlob);
	}

	double distanceBetweenPoints(cv::Point point1, cv::Point point2)
	{

		int intX = abs(point1.x - point2.x);
		int intY = abs(point1.y - point2.y);

		return(sqrt(pow(intX, 2) + pow(intY, 2)));
	}


	void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName)
	{
		cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);

		cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);

		cv::imshow(strImageName, image);
	}


	void drawAndShowContours(cv::Size imageSize, std::vector<Blob> blobs, std::string strImageName)
	{
		cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);

		std::vector<std::vector<cv::Point> > contours;

		for (auto &blob : blobs) {
			if (blob.blnStillBeingTracked == true) {
				contours.push_back(blob.currentContour);
			}
		}

		cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);

		cv::imshow(strImageName, image);
	}

	bool checkIfBlobsCrossedTheLine(std::vector<Blob> &blobs, int &intHorizontalLinePosition, int &carCount)
	{
		bool blnAtLeastOneBlobCrossedTheLine = false;

		for (auto blob : blobs) {

			if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
				int prevFrameIndex = (int)blob.centerPositions.size() - 2;
				int currFrameIndex = (int)blob.centerPositions.size() - 1;

				if (blob.centerPositions[prevFrameIndex].y > intHorizontalLinePosition && blob.centerPositions[currFrameIndex].y <= intHorizontalLinePosition) {
					carCount++;
					blnAtLeastOneBlobCrossedTheLine = true;
				}
			}

		}

		return blnAtLeastOneBlobCrossedTheLine;
	}

	void drawBlobInfoOnImage(std::vector<Blob> &blobs, cv::Mat &imgFrame2Copy)
	{
		for (unsigned int i = 0; i < blobs.size(); i++) {

			if (blobs[i].blnStillBeingTracked == true) {
				cv::rectangle(imgFrame2Copy, blobs[i].currentBoundingRect, SCALAR_RED, 2);

				int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
				double dblFontScale = blobs[i].dblCurrentDiagonalSize / 60.0;
				int intFontThickness = (int)std::round(dblFontScale * 1.0);

				cv::putText(imgFrame2Copy, std::to_string(i), blobs[i].centerPositions.back(), intFontFace, dblFontScale, SCALAR_GREEN, intFontThickness);
			}
		}
	}


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
};