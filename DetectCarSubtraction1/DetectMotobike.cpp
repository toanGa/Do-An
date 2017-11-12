#include "DetectMotobike.h"
int abs(int var1, int var2);
MotoBinaryDetecter::MotoBinaryDetecter(int sizeXMin, int sizeYMin)
{
	this->sizeXMin = sizeXMin;
	this->sizeYMin = sizeYMin;
}

bool MotoBinaryDetecter::detectSingleScale(Mat binaryMat, int scale)
{
	if (CV_8UC1 != binaryMat.type())
	{
		return false;
	}
	int sizeXMat = binaryMat.cols;
	int sizeYMat = binaryMat.rows;

	int sizeXDetect = sizeXMin + (scale - 1)*DISTANCE_DETECT;
	int sizeYDetect = sizeYMin + (scale - 1)*DISTANCE_DETECT;
	if ((sizeXDetect > sizeXMat) || (sizeYDetect > sizeYMat))
	{
		return false;
	}


	int xMaxSizeDetect = sizeXMat - sizeXDetect;
	int yMaxSizeDetect = sizeYMat - sizeYDetect;
	//namedWindow("detectFrame", CV_WINDOW_NORMAL);
	for (int i = 0; i < xMaxSizeDetect; ++i)
	{
		for (int j = 0; j < yMaxSizeDetect; ++j)
		{
			Rect tempRect = Rect(i, j, sizeXDetect, sizeYDetect);
			Mat checkMat = binaryMat(tempRect);
			//imshow("detectFrame", checkMat);
			//waitKey(-1);
#if 1
			if (checkMotobike(checkMat, ORIENT_BACK))
			{
				rectDetect.push_back(tempRect);
			}
#endif
#if 0
			if (checkMotobike(checkMat, ORIENT_FACE))
			{
				rectDetect.push_back(tempRect);
			}
#endif
		}
	}
	if (rectDetect.size() != 0)
	{
		return true;
	}
	return false;
}

bool MotoBinaryDetecter::detectMultiScale(Mat binaryMat, int maxScale)
{
	bool returnVal = false;
	for (int i = 1; i <= maxScale; ++i)
	{
		bool val = detectSingleScale(binaryMat, i);
		if (true == val)
		{
			returnVal = true;
		}
	}
	return returnVal;
}

vector <Rect> MotoBinaryDetecter::getRectsDetected()
{
	return diffObj;
}

void MotoBinaryDetecter::reduceRectDetect()
{
	diffObj.clear();// clear before process

	size_t sizeInstance = rectDetect.size();
	while (sizeInstance)
	{
		Rect rectCmp = rectDetect.at(0);
		//diffObj.push_back(rectCmp);
		//rectDetect.erase(rectDetect.begin());

		//size_t sizeNow = rectDetect.size();
		vector<Rect> sameObj = findSameObj(rectDetect, rectCmp);
		Rect master = findMaser(sameObj);
		diffObj.push_back(master);

		sizeInstance = rectDetect.size();
	}


}

Rect MotoBinaryDetecter::findMaser(vector<Rect> listRect)
{
	vector<Point> listCenter;
	size_t size = listRect.size();



	for (int i = 0; i < size; ++i)// get total center
	{
		Rect curr = listRect.at(i);
		Point center = (curr.tl() + curr.br()) / 2;
		listCenter.push_back(center);
	}

	int minX = listCenter.at(0).x, minY = listCenter.at(0).y;
	int maxX, maxY;

	int maxSizeX = 0;
	int maxSizeY = 0;
	for (int i = 0; i < size; ++i)
	{
		Point curr = listCenter.at(i);
		if (curr.x < minX)
		{
			minX = curr.x;
		}
		else
		{
			maxX = curr.x;
		}

		if (curr.y < minY)
		{
			minY = curr.y;
		}
		else
		{
			maxY = curr.y;
		}

	}

	maxSizeX = maxX - minX;
	maxSizeY = maxY - minY;

	int width = maxSizeX;
	int height = maxSizeY;

	int totalBounded = 0;
	int indexFinded = 0;
	for (int i = 0; i < size; ++i)// get rect bound max point
	{
		int xRect = listCenter.at(i).x - width / 2;
		if (xRect < 0)
		{
			xRect = 0;
		}

		int yRect = listCenter.at(i).y - height / 2;
		if (yRect < 0)
		{
			yRect = 0;
		}


		Rect bound = Rect(xRect, yRect, width, height);
		int tempBounded = 0;
		for (int j = 0; j < size; ++j)
		{
			if (isInRect(bound, listCenter.at(j)))
			{
				++tempBounded;
			}
		}

		if (tempBounded > totalBounded)
		{
			indexFinded = i;
			totalBounded = tempBounded;
		}
	}
	listCenter.clear();

	return listRect.at(indexFinded);
}

bool MotoBinaryDetecter::isInRect(Rect master, Rect slave)
{
	Point centerMaster = (master.tl() + master.br()) / 2;
	Point centerSlave = (slave.tl() + slave.br() / 2);
	if ((centerMaster.x <= slave.x) && (centerMaster.y <= slave.y) &&
		(master.width >= slave.width + slave.x) &&
		(master.height >= slave.height + slave.y))
	{
		return true;
	}
	return false;
}

bool MotoBinaryDetecter::isInRect(Rect rect, Point point)
{
	if ((point.x >= rect.x) && 
		(point.y >= rect.y) &&
		(abs(point.x, rect.x) <= rect.width) &&
		(abs(point.y, rect.y) <= rect.height)
		)
		return true;
	return false;
}

vector <Rect> MotoBinaryDetecter::findSameObj(vector<Rect>& listRect, Rect sample)
{
	int width = sample.width;
	int height = sample.height;
	size_t size = listRect.size();

	vector<Rect> returnRect;
	vector<int> indexDelete;
	for (int i = 0; i < size; ++i)
	{
		Rect temp = listRect.at(i);
		if ((temp.width == width) && (temp.height == height)
			&& (abs(sample.x, temp.x) <= width / 2)
			&& (abs(sample.y, temp.y) <= height / 2)
			)
		{
			returnRect.push_back(temp);
			//listRect.erase(listRect.begin() + i);// delete from root list
			indexDelete.push_back(i);
		}
	}

	size_t sizeDelete = indexDelete.size();
	int deleted = 0;
	for (int i = 0; i < sizeDelete; ++i)
	{
		int index = indexDelete.at(i) - deleted;
		listRect.erase(listRect.begin() + index);
		++deleted;
	}


	indexDelete.clear();
	return returnRect;
}

MotoBinaryDetecter::~MotoBinaryDetecter()
{
	rectDetect.clear();
}

int abs(int var1, int var2)
{
	if (var1 > var2)
	{
		return var1 - var2;
	}
	return var2 - var1;
}