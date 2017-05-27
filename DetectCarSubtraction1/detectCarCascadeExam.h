#pragma once
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <stdio.h>
using namespace std;
using namespace cv;

void detectAndDisplay(Mat frame);
String cars_cascade_name = "cars.xml";
CascadeClassifier cars_cascade;

String window_name = "Capture - car detection";

/** @function main */
int main(void)
{

	VideoCapture capture("hamKimLien1.mp4");
	Mat frame;

	//-- 1. Load the cascades
	if (!cars_cascade.load(cars_cascade_name))
	{
		printf("--(!)Error loading car cascade\n");
		system("pause");
		return -1;
	};
	//-- 2. Read the video stream
	double fps = capture.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
	cout << "Frame per seconds : " << fps << endl;


	if (!capture.isOpened()) { printf("--(!)Error opening video capture\n"); return -1; }

	while (capture.read(frame))
	{
		if (frame.empty())
		{
			printf(" --(!) No captured frame -- Break!");
			break;
		}

		//-- 3. Apply the classifier to the frame
		detectAndDisplay(frame);

		char c = (char)waitKey(10);
		if (c == 27) { break; } // escape
	}
	return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay(Mat frame)
{
	std::vector<Rect> cars;
	Mat frame_gray;

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//-- Detect faces
	cars_cascade.detectMultiScale(frame_gray, cars, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < cars.size(); i++)
	{
		//Point center(cars[i].x + cars[i].width / 2, cars[i].y + cars[i].height / 2);
		//ellipse(frame, center, Size(cars[i].width / 2, cars[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
		Point vertex1(cars[i].x, cars[i].y);
		Point vertex2(cars[i].x + cars[i].width, cars[i].y + cars[i].height);
		rectangle(frame, vertex1, vertex2, Scalar(0, 255, 255));
		///putText(frame, "Hi all...", vertex1, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 200, 200), 4);
	}
	//-- Show what you got
	imshow(window_name, frame);
}

#pragma once
