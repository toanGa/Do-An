#include "DetectGray.h"
#include "Defines.h"
bool isGrayImage(Mat gray)
{
	if (gray.type() == CV_8UC1)
	{
		return true;
	}
	return false;
}

