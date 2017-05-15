#pragma once
typedef struct
{
	int size_x;
	int size_y;
}Size_detect;

class data
{
private:
	int rows;
	int cows;
	int *matData;
public:
	data(int rows, int cows)
	{
		this->rows = rows;
		this->cows = cows;
		matData = new int[rows*cows];
	}
	~data()
	{
		delete matData;
	}
};