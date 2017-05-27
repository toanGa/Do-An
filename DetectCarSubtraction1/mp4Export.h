#pragma once
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include"DetectMotobike.h"
#include<iostream>
#include<conio.h>           // it may be necessary to change or remove this line if not using Windows

#include "Blob.h"
class Mp4Exporter
{
private:
	Size frameSize;
	double fps;
	VideoWriter videoWriter;
public:
	Mp4Exporter(Size size, double fps);
	bool open(string fileName);
	void write(const Mat& mat);
	void close();
	~Mp4Exporter();
};