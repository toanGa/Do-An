#pragma once
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include "getValMaxInstance.h"
#include <iostream>
using namespace cv;
using namespace std;
typedef int channel;
#define CHANNEL_SINGLE 0
#define CHANNEL_COLOR 1
class RestoreBackground
{
private:
	int rows;
	int cols;

	int width;
	int height;

	channel channel;
	Mat imageRestore;
	//Mat externMat;
	vector<Mat> vectorMat;
	VideoCapture capture;
	vector<VideoCapture> listCapture;
	//vector<int> totalFrame;
public:
	RestoreBackground(VideoCapture capture)
	{
		if (!capture.isOpened())
		{
			while (1);
		}
		this->capture = capture;

		Mat externMat;
		capture.read(externMat);
		imageRestore = externMat.clone();

		if (CV_8UC1 == externMat.type())
		{
			this->channel = CHANNEL_SINGLE;
		}
		else if (CV_8UC3 == externMat.type())
		{
			this->channel = CHANNEL_COLOR;
		}

		rows = externMat.rows;
		cols = externMat.cols;

		width = cols;
		height = rows;
		imwrite("beforeRun.png", imageRestore);

	}

	RestoreBackground(vector<VideoCapture> listVideo)
	{
		if (!listVideo.at(0).isOpened())
		{
			while (1);
		}
		this->listCapture = listVideo;

		Mat externMat;
		listVideo.at(0).read(externMat);
		imageRestore = externMat.clone();

		if (CV_8UC1 == externMat.type())
		{
			this->channel = CHANNEL_SINGLE;
		}
		else if (CV_8UC3 == externMat.type())
		{
			this->channel = CHANNEL_COLOR;
		}

		rows = externMat.rows;
		cols = externMat.cols;

		width = cols;
		height = rows;
		imwrite("beforeRun.png", imageRestore);
	}


	// get data from video and set for matrix class
	void getDataFromSingleVideo()
	{
		Mat tempMat;
		while (capture.read(tempMat))
		{
			vectorMat.push_back(tempMat);
		}
	}

	void getDataFromListVideo()
	{
		Mat tempMat;
		size_t listVideo = listCapture.size();
		for (int i = 0; i < listVideo; i++)
		{
			while (listCapture.at(i).read(tempMat))
			{
				vectorMat.push_back(tempMat);
			}
		}
	}
	// start find in pixel
	void calcData()
	{
		size_t totalSize = vectorMat.size();
		for (int i = 0; i < totalSize; i++)
		{
			Mat tempMat = vectorMat.at(i);
			
			if (CHANNEL_COLOR == channel)
			{
				

				for (int z = 0; z < 3; z++)
				{
					vector <uchar> singlePixelChannel;
					for (int j = 0; j < cols; j++)//x
					{
						for (int k = 0; k < rows; k++)//y
						{
							// get pixel
							Vec3b vec = tempMat.at<Vec3b>(Point(j, k));// bgr
							singlePixelChannel.push_back(vec.val[k]);
						}
					}
					singlePixelChannel.clear();
				}			
			}
			
		}
	}

	// start find in pixel
	void calcData_t()
	{
		size_t totalSize = vectorMat.size();		
		if (CHANNEL_COLOR == channel)
		{
#define EXPAND_X 500
#define EXPAND_Y 500
			for (int x  = 0 + EXPAND_Y; x < 100 + EXPAND_Y; x++)//rows -> height
			{
				for (int y = 0 + EXPAND_X; y < 100 + EXPAND_X; y++)//cols -> 
				{
					for (int z = 0 ; z < 3 ; z++)
					{
						vector <uchar> singlePixelChannel;
						for (int i = 0; i < totalSize; i++)
						{
							Vec3b vec = vectorMat.at(i).at<Vec3b>(Point(y, x));// bgr
							singlePixelChannel.push_back(vec.val[z]);
						}
						uchar findedVal = getValMaxFreqOfArray(singlePixelChannel);
						imageRestore.at<Vec3b>(Point(y, x)).val[z] = findedVal;

						singlePixelChannel.clear();
					}
					//cout << "done at <" << x << "," << y << ">\n";
				}
			}
			//cout << "done at"
		}

	}

	void writeImage(String name)
	{
		Rect rect(EXPAND_X, EXPAND_Y, 100, 100);
		Mat split = imageRestore(rect);
		imwrite("afterRun.png", imageRestore);
		imwrite(name, split);
	}

	~RestoreBackground()
	{
		vectorMat.clear();
		listCapture.clear();
	}
};
