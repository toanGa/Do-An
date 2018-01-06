#pragma once
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect.hpp"
#include<iostream>
#include<conio.h>    
using namespace std;
using namespace cv;
#define NUMS_POINT_TRACT 7
// codinate of point in left of image
static Point ImageLeft[NUMS_POINT_TRACT] =
{
	Point(30, 100),
	Point(40, 90),
	Point(50, 80),
	Point(60, 70),
	Point(70, 60),
	Point(80, 50),
	Point(90, 40)
};

// codinate of point in right of image
static Point ImageRight[NUMS_POINT_TRACT] =
{
	Point(130, 100),
	Point(140, 90),
	Point(150, 80),
	Point(160, 70),
	Point(170, 60),
	Point(180, 50),
	Point(190, 40)
};

// codinate of point in left of real world
static Point RealLeft[NUMS_POINT_TRACT] = 
{
	Point(500, 10500),
	Point(500, 15500),
	Point(500, 17000),
	Point(500, 17500),
	Point(500, 17650),
	Point(500, 17720),
	Point(500, 17770),
};

// codinate of point in left of real world
static Point RealRight[NUMS_POINT_TRACT] =
{
	Point(1500, 10500),
	Point(1500, 15500),
	Point(1500, 17000),
	Point(1500, 17500),
	Point(1500, 17650),
	Point(1500, 17720),
	Point(1500, 17770),
};

