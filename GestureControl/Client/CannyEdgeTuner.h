#pragma once
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Windows.h"

using namespace cv;

class CannyEdgeTuner
{
public:
	CannyEdgeTuner(Size size, int type);
	void apply(Mat src);
	int getUpperBound() const;
	int getLowerBound() const;
	int getKernelSize() const;
	Mat getEdges() const;
	~CannyEdgeTuner();

private:
	const char * windowName = "Countours";
	const int maxThreshold = 100;
	const int maxRatio = 25;
	int ratio = 2;
	int lowerBound = 40;
	int kernel = 3;
	Mat edges, detected, gray;
};

