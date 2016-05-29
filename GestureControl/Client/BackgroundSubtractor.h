#pragma once
#include<opencv2/opencv.hpp>

using namespace cv;

class BackgroundRemover
{
public:
	BackgroundRemover();
	~BackgroundRemover();

	bool apply(Mat image);
	Mat getBackgroundSubtraction() const;

private:
	Mat currentBackgroundSubtraction;
	Ptr<BackgroundSubtractorMOG2> bg;
	int counter = 100;
};

