#include "ConvexHull.h"

ConvexHull::ConvexHull(int drawMode) {
	namedWindow(windowName, WINDOW_AUTOSIZE);
	this->drawMode = drawMode;
}

void ConvexHull::apply(Mat img)
{
	Mat closing;
	Mat structuringElement = getStructuringElement(MORPH_ELLIPSE, Size(25, 25));
	morphologyEx(img, closing, MORPH_CLOSE, structuringElement);

	vector<Vec4i> hierarchy;
	contours.clear();
	findContours(closing, contours, hierarchy, CV_RETR_CCOMP, CHAIN_APPROX_NONE, Point(0, 0));

	auto largestArea = 0.0;
	largest = -1;
	for (auto i = 0; i < contours.size(); i++) {
		auto area = contourArea(contours[i]);
		if (area > largestArea) {
			largestArea = area;
			largest = i;
		}
	}

	Mat drawing = Mat::zeros(img.size(), CV_8UC3);
	if (contours.size() > 0 && hasHull() && contours[largest].size() > 1) {
		hull.clear();
		convexHull(contours[largest], hull, false, false);

		if (drawMode & CONVEX_HULL_DRAW_SELECTED) {
			drawContours(drawing, contours, largest, Scalar(255, 255, 255));
			for (auto i = 0; i < hull.size(); i++)
			{
				auto next = i + 1 == hull.size() ? 0 : i + 1;
				line(drawing, contours[largest][hull[i]], contours[largest][hull[next]], Scalar(0, 255, 0));
			}
		}
	}

	for (auto i = 0; i< contours.size(); i++)
	{
		vector<Point> hull;
		convexHull(Mat(contours[i]), hull);
		auto color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		if (drawMode & CONVEX_HULL_DRAW_CONTOURS) {
			drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		}
		if (drawMode & CONVEX_HULL_DRAW_HULLS) {
			drawContours(drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		}
	}

	imshow("Hull", drawing);
}

bool ConvexHull::hasHull() const
{
	return largest != -1;
}

vector<Point> ConvexHull::getLargestHullContour() const
{
	return contours[largest];
}

vector<int> ConvexHull::getLargestHullPoints() const
{
	return hull;
}

ConvexHull::~ConvexHull()
{
	destroyWindow(windowName);
}
