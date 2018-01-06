#pragma once
#include "config_data.h"
// ax + by = c
class Line
{
public:
	float a;
	float b;
	float c;
public:
	Line(float a, float b, float c);
	Line();
	static Line getLine(Point p1, Point p2);
	static Point getCutPoint(Line l1, Line l2);
	~Line();
};

