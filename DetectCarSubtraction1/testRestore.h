#pragma once
#include "RestoreBackground.h"
//20170301-073531
//20170301-073635
//20170301-073739
//20170301-073843
//20170301-073947
//20170301-074051
//20170301-074155
//20170301-074259
void main()
{

	//
	cout << "start\n";
	VideoCapture cap;
	cap.open("20170228-090949.mp4");

	VideoCapture cap0;
	VideoCapture cap1;
	VideoCapture cap2;
	VideoCapture cap3;
	VideoCapture cap4;
	VideoCapture cap5;
	VideoCapture cap6;
	VideoCapture cap7;

	cap0.open("20170301-073531.mp4");
	cap1.open("20170301-073635.mp4");
	cap2.open("20170301-073739.mp4");
	cap3.open("20170301-073843.mp4");
	cap4.open("20170301-073947.mp4");
	cap5.open("20170301-074051.mp4");
	cap6.open("20170301-074155.mp4");
	cap7.open("20170301-074259.mp4");

	vector < VideoCapture> listVideo;

	cout << "push to list\n";
	listVideo.push_back(cap0);
	listVideo.push_back(cap1);
	listVideo.push_back(cap2);
	listVideo.push_back(cap3);
	listVideo.push_back(cap4);
	listVideo.push_back(cap5);
	listVideo.push_back(cap6);
	listVideo.push_back(cap7);
	//RestoreBackground restore(cap);
	//restore.getDataFromSingleVideo();

	RestoreBackground restoreList(listVideo);
	restoreList.getDataFromListVideo();
	restoreList.calcData_t();
	restoreList.writeImage("myImage.png");
	system("pause");
}