#include "assign_locate.h"
#include "Line.h"
#include <math.h>
static int getLocateSpace(Point imgPoint);
static int distance(Point p1, Point p2);
// convert image point to real point
Point getRealPoint(Point imgPoint)
{
	int locateSpace = getLocateSpace(imgPoint);
	if (-1 == locateSpace)
	{
		return NULL;
	}
	Point lowP_Left, highP_Left;
	Point lowP_Right, highP_Right;
	lowP_Left = ImageLeft[locateSpace];
	highP_Left = ImageLeft[locateSpace + 1];
	lowP_Right = ImageRight[locateSpace];
	highP_Right = ImageRight[locateSpace + 1];
	Line lineThrowLeft = Line::getLine(lowP_Left, highP_Left);
	Line lineThrowRight = Line::getLine(lowP_Right, highP_Right);

	Line crossPoint(0, 1, imgPoint.y);

	Point cutPointLeft = Line::getCutPoint(lineThrowLeft, crossPoint);
	Point cutPointRight = Line::getCutPoint(lineThrowRight, crossPoint);

	// start get img point
}

// get locate in image
// 0-1-2-3-4-5
static int getLocateSpace(Point imgPoint)
{
	if (ImageLeft[0].y <= imgPoint.y 
		&& imgPoint.y < ImageLeft[1].y)
	{
		return 0;
	}
	if (ImageLeft[1].y <= imgPoint.y
		&& imgPoint.y < ImageLeft[2].y)
	{
		return 1;
	}
	if (ImageLeft[2].y <= imgPoint.y
		&& imgPoint.y < ImageLeft[3].y)
	{
		return 2;
	}
	if (ImageLeft[3].y <= imgPoint.y
		&& imgPoint.y < ImageLeft[4].y)
	{
		return 3;
	}
	if (ImageLeft[4].y <= imgPoint.y
		&& imgPoint.y < ImageLeft[5].y)
	{
		return 4;
	}
	if (ImageLeft[5].y <= imgPoint.y
		&& imgPoint.y <= ImageLeft[6].y)
	{
		return 5;
	}
	return -1;
}

// get distance between two point
static int distance(Point p1, Point p2)
{
	return (int)sqrt(p1.x*p1.x + p2.x*p2.x);
}