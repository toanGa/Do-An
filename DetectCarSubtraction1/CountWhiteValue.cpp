#include"CountWhiteValue.h"
int CalcTotalWhite(Mat mat, Rect rect)
{
	Mat buffer = mat(rect);
	return countNonZero(buffer);
}

// head, emptyBody part, buttom
bool checkMotobike(Mat binaryMat, int orient)
{
	Mat head;
	Mat emptyBody;
	Mat buttom;
	int sizeXMat = binaryMat.cols;
	int sizeYMat = binaryMat.rows;



	if (ORIENT_BACK == orient)
	{
		int heightHead = sizeYMat / 10;
		int widthHead = sizeXMat / 2;
		int xHead = sizeXMat / 4;

		Rect headRect = Rect(xHead, 0, widthHead, heightHead);
		head = binaryMat(headRect);


		int heightBody = (sizeYMat - heightHead) / 2;
		int expandX = sizeXMat / 3;
		int expandY = heightBody / 4;

		int widthEmptyBodyRect = sizeXMat - (expandX + expandX);
		int heightEmptyBodyRect = heightBody - (expandY + expandY);
		Rect emptyBodyRect = Rect(expandX, heightHead + expandY, widthEmptyBodyRect, heightEmptyBodyRect);

		emptyBody = binaryMat(emptyBodyRect);

		int heightButtom = sizeYMat - heightHead - heightBody;
		Rect buttomRect = Rect(0, heightButtom, sizeXMat, heightButtom);
		buttom = binaryMat(buttomRect);
	}
	else if (ORIENT_FACE == orient)
	{
		int heightHead = sizeYMat / 6;
		int widthHead = sizeXMat / 2;
		int xHead = sizeXMat / 4;

		Rect headRect = Rect(xHead, 0, widthHead, heightHead);
		head = binaryMat(headRect);


		int heightBody = (sizeYMat - heightHead) / 3;
		int expandX = sizeXMat / 3;
		int expandY = heightBody / 4;

		int widthEmptyBodyRect = sizeXMat - (expandX + expandX);
		int heightEmptyBodyRect = heightBody - (expandY + expandY);
		Rect emptyBodyRect = Rect(expandX, heightHead + expandY, widthEmptyBodyRect, heightEmptyBodyRect);

		emptyBody = binaryMat(emptyBodyRect);

		int heightButtom = sizeYMat - heightHead - heightBody;
		int yButtom = heightHead + heightBody;
		Rect buttomRect = Rect(0, yButtom, sizeXMat, heightButtom);
		buttom = binaryMat(buttomRect);
	}

	int totalWhite = countNonZero(binaryMat);
	int totalWhiteHead = countNonZero(head);
	int totalWhiteEmptyBody = countNonZero(emptyBody);
	int totalWhiteButtom = countNonZero(buttom);

	double fHead = (double)totalWhiteHead / (double)totalWhite;
	double fEmptyBody = (double)totalWhiteEmptyBody / (double)totalWhite;
	double fButtom = (double)totalWhiteButtom / (double)totalWhite;

	// checkCondition
	if ((ORIENT_BACK == orient))
	{
		if ((fHead > 0.05 && fHead < 0.1) &&
			(fEmptyBody < 0.02) &&
			(fButtom > 0.5) && fButtom < 0.7)
			return true;
		else
			return false;
	}
	else if (ORIENT_FACE == orient)
	{
		if ((fHead > 0.05 && fHead < 0.1) &&
			(fEmptyBody < 0.02) &&
			(fButtom > 0.65) && fButtom < 0.75)
			return true;
		else
			return false;
	}
	else if (ORIENT_BOTH == orient)
	{
		if ((fHead > 0.06 && fHead < 0.08) &&
			(fEmptyBody < 0.01) &&
			(fButtom > 0.55) && fButtom < 0.7)
			return true;
		else
			return false;
	}

}

// class defenition
CountWhiteValue::CountWhiteValue(Shape shape)
{
	this->shape = shape;
	totalWhite = 0;
}

CountWhiteValue::CountWhiteValue(Rect rect)
{
	rectangle = rect;
}

Shape CountWhiteValue::getShape()
{
	return shape;
}

int CountWhiteValue::getTotalWhile()
{
	return totalWhite;
}


int CountWhiteValue::CalcTotalWhiteValue(Mat mat)
{
	if (CV_8UC1 == mat.type())
	{
		Mat buffer = mat(rectangle);
		totalWhite = countNonZero(buffer);
		return totalWhite;
	}
	return -1;
}
