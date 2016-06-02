#pragma once
#include<opencv2/opencv.hpp>

using namespace cv;

class BackgroundRemover
{
public:
	BackgroundRemover(int=100);
	~BackgroundRemover();

	bool apply(Mat);
	Mat getBackgroundSubtraction() const;

private:
	Mat currentBackgroundSubtraction;
	Ptr<BackgroundSubtractorMOG2> bg;
	int counter = 100;
};

