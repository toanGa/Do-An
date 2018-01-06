#include "Line.h"



Line::Line(float a, float b, float c)
{
	this->a = a;
	this->b = b;
	this->c = c;
}

Line::Line()
{
}

Line::~Line()
{
}

// viet phuong trinh duong thang di qua 2 diem
Line Line::getLine(Point p1, Point p2)
{
	Line lineCalc;
	lineCalc.a = p2.x - p1.x;
	lineCalc.b = p1.y - p2.y;
	lineCalc.c = p1.y*(p2.x - p1.x) - p1.x*(p2.y - p1.y);
	return lineCalc;
}

// giao diem cua hai duong thang
Point Line::getCutPoint(Line l1, Line l2)
{
	double d, dx, dy;
	Point returnPoint = NULL;
	d = l1.a*l2.b - l2.a*l1.b;
	dx = l1.c*l2.b - l2.c*l1.b;
	dy = l1.a*l2.c - l2.c * l1.a;
	if (d)// dangerous when compare equare with double
	{
		returnPoint.x = dx / d;
		returnPoint.y = dy / d;
	}
	return returnPoint;
}