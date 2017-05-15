#pragma once
#include "Shape.h"
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace cv;

enum Orient
{
	ORIENT_FACE,
	ORIENT_BACK,
	ORIENT_BOTH,
};


class CountWhiteValue
{
private:
	Shape shape;
	Rect rectangle;
	int totalWhite;
public:
	CountWhiteValue(Shape shape);

	CountWhiteValue(Rect rect);

	Shape getShape();

	int getTotalWhile();


	int CalcTotalWhiteValue(Mat mat);
};


int CalcTotalWhite(Mat mat, Rect rect);


// head, emptyBody part, buttom
bool checkMotobike(Mat binaryMat, int orient);