#pragma once
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include "Windows.h"

#define CONVEX_HULL_DRAW_HULLS 1
#define CONVEX_HULL_DRAW_CONTOURS 2
#define CONVEX_HULL_DRAW_SELECTED 4
#define CONVEX_HULL_DRAW_DEFAULT CONVEX_HULL_DRAW_SELECTED

using namespace std;
using namespace cv;

class ConvexHull
{
public:
	ConvexHull(int = CONVEX_HULL_DRAW_DEFAULT);
	void apply(Mat img);
	bool hasHull() const;
	vector<Point> getLargestHullContour() const;
	vector<int> getLargestHullPoints() const;
	~ConvexHull();

private:
	double distanceBetween(Point, Point) const;

	const char * windowName = "Hull";
	RNG rng = RNG(12345);
	vector<vector<Point>> contours;
	vector<int> hull;
	int drawMode;
	int largest = -1;
};

