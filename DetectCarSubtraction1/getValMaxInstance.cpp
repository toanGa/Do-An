#include "getValMaxInstance.h"
#include <iostream>
using namespace std;
bool isNotHaveComponent(uchar comp, vector<uchar> vect)
{
	int size = (int)vect.size();
	for (int i = 0; i < size; ++i)
	{
		if (comp == vect.at(i))
		{
			return false;
		}
	}
	return true;
}



int getMaxValInArr(vector<int> arr)
{
	size_t size = arr.size();
	int max = arr.at(0);
	for (int i = 1; i < size; ++i)
	{
		if (arr.at(i) > max)
		{
			max = arr.at(i);
		}
	}
	return max;
}



int getOrderByVal(int val, vector<int> arr)
{
	size_t size = arr.size();
	for (int i = 0; i < size; ++i)
	{
		if (val == arr.at(i))
		{
			return i;
		}
	}
	return -1;
}


uchar getValMaxFreqOfArray(vector<uchar> arrayUchar)
{
	vector <uchar> diffVal;
	vector <int> freqVal;
	diffVal.push_back(arrayUchar.at(0));
	size_t size = arrayUchar.size();
	for (int i = 1; i < size; i++)//get total difference val have
	{
		if (isNotHaveComponent(arrayUchar.at(i), diffVal))
		{
			diffVal.push_back(arrayUchar.at(i));
		}
	}
	//cout << "done get val for diffVal\n";
	size_t sizeDiff = diffVal.size();
	for (int i = 0; i < sizeDiff; ++i)// find total val appear in array
	{
		int count = 0;
		for (int j = 0; j < size; ++j)
		{
			if (arrayUchar.at(j) == diffVal.at(i))
			{
				++count;
			}
		}
		freqVal.push_back(count);
	}

	// gia tri xuat hien nhieu nhat trong freqVal
	int maxTimeApper = getMaxValInArr(freqVal);
	int order = getOrderByVal(maxTimeApper, freqVal);

	uchar returnVal = diffVal.at(order);
	diffVal.clear();
	freqVal.clear();
	return returnVal;
}