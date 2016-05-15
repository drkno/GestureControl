#include "CannyEdgeTuner.h"

CannyEdgeTuner::CannyEdgeTuner(Size size, int type)
{
	edges.create(size, type);
	namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	createTrackbar("Min:", windowName, &lowerBound, maxThreshold);
	createTrackbar("Ratio:", windowName, &ratio, maxRatio);
}

void CannyEdgeTuner::apply(Mat src)
{
	cvtColor(src, gray, CV_BGR2GRAY);
	blur(gray, detected, Size(kernel, kernel));
	Canny(detected, detected, lowerBound, lowerBound*ratio, kernel);
	edges = Scalar::all(0);
	src.copyTo(edges, detected);
	imshow(windowName, edges);
}

int CannyEdgeTuner::getUpperBound() const
{
	return lowerBound*ratio;
}

int CannyEdgeTuner::getLowerBound() const
{
	return lowerBound;
}

int CannyEdgeTuner::getKernelSize() const
{
	return kernel;
}

Mat CannyEdgeTuner::getEdges() const
{
	return detected;
}

CannyEdgeTuner::~CannyEdgeTuner()
{
	destroyWindow(windowName);
}