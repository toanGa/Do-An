#include"mp4Export.h"

Mp4Exporter::Mp4Exporter(Size size, double fps)
{
	frameSize.width = static_cast<int>(size.width);
	frameSize.height = static_cast<int>(size.height);

	this->fps = fps;
}

bool Mp4Exporter::open(string fileName)
{
	videoWriter = VideoWriter(fileName, CV_FOURCC('M', 'P', '4', '2'),
		fps, frameSize, true);
	if (!videoWriter.isOpened()) //if not initialize the VideoWriter successfully, exit the program
	{
		cout << "ERROR: Failed to write the video" << endl;
		return false;
	}
	return true;
}

void Mp4Exporter::write(const Mat& mat)
{
	videoWriter.write(mat);
}

void Mp4Exporter::close()
{
	if (videoWriter.isOpened())
	{
		videoWriter.release();
	}
	
}

Mp4Exporter::~Mp4Exporter()
{
	if (videoWriter.isOpened())
	{
		videoWriter.release();
	}
}