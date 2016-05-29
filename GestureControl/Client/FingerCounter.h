#pragma once
#include <opencv2/core/mat.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

enum HandOrientation
{
	Up,
	Down
};

class FingerCounter
{
public:
	void apply(const vector<int>& hull, const vector<Point_<int>>& contour);
	HandOrientation getOrientation() const;
	int getNumberOfFingers() const;
	void drawPoseAngle(Mat image) const;

private:
	float averageAngle = 0.0;
	int numFingers = 0;
	Point currCenter, currv1, currv2;
	vector<Vec4i> defects;

	static float distanceBetween(Point x, Point y);
	float angleBetween(const Point& oldv1, const Point& oldv2);
};

