#pragma once
#include "Shape.h"
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include "DetectMotobike.h"
#include "CountWhiteValue.h"
using namespace cv;
using namespace std;
/*  Case detect:
 *	1.Detect for full object in single scale
 *	2.Detect for full object in multi scale
 *  3.Detect for part of object in multi scale
 */
#define DISTANCE_DETECT 5// dung trong detect multiscale, 2 lan lien tiep cach nhau 5 pixel

class MotoBinaryDetecter
{
private:
	int sizeXMin;
	int sizeYMin;

	
	vector <Rect> diffObj;// difference rect detectd
	//vector <Rect> rectDetect;// total rect detected

	
	bool isInRect(Rect master, Rect slave);// check a rect is inside a rect
	bool isInRect(Rect rect, Point point);// check a point is inside a rect
	Rect findMaser(vector<Rect> listRect);

	vector <Rect> findSameObj(vector<Rect>& listRect, Rect sample);
public:
	vector <Rect> rectDetect;// total rect detected
	MotoBinaryDetecter(int sizeXMin, int sizeYMin);
	bool detectSingleScale(Mat binaryMat, int scale);
	bool detectMultiScale(Mat binaryMat, int maxScale);

	void reduceRectDetect();// reduce rect in rectDetect
	vector <Rect> getRectsDetected();

	~MotoBinaryDetecter();
};