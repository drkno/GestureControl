#include "FingerCounter.h"

void FingerCounter::apply(const vector<int>& hull, const vector<Point_<int>>& contour)
{
	try {
		defects = vector<Vec4i>();
		convexityDefects(contour, hull, defects);

		Point center;
		for (auto i = 0; i < contour.size(); i++)
		{
			center.x += contour[i].x;
			center.y += contour[i].y;
		}
		center.x /= contour.size();
		center.y /= contour.size();

		averageAngle = 0;
		numFingers = 0;
		for (auto i = 0; i < defects.size(); i++) {
			auto defect = defects[i];
			auto startPoint = contour[defect[0]];
			auto endPoint = contour[defect[1]];
			auto farPoint = contour[defect[2]];

			auto a = sqrt(pow(endPoint.x - startPoint.x, 2) + pow(endPoint.y - startPoint.y, 2));
			auto b = sqrt(pow(farPoint.x - startPoint.x, 2) + pow(farPoint.y - startPoint.y, 2));
			auto c = sqrt(pow(endPoint.x - farPoint.x, 2) + pow(endPoint.y - farPoint.y, 2));
			auto angle = acos((pow(b, 2) + pow(c, 2) - pow(a, 2)) / (2 * b * c)) * 57;
			if (angle <= 90) {
				numFingers += 1;
				averageAngle += (angleBetween(center, startPoint) + angleBetween(center, endPoint)) / 2;
			}
		}
		averageAngle /= numFingers;
	}
	catch(Exception e)
	{
		// not enough information to get convexity defects from... ignore
		if (e.msg.find("convexityDefects") != string::npos)
		{
			numFingers = 0;
		}
		else
		{
			throw e;
		}
	}
}

HandOrientation FingerCounter::getOrientation() const
{
	return averageAngle > M_PI_2 ? Up : Down;
}

int FingerCounter::getNumberOfFingers() const
{
	return numFingers;
}

float FingerCounter::distanceBetween(Point x, Point y)
{
	return (x.x - y.x)*(x.x - y.x) + (x.y - y.y)*(x.y - y.y);
}

void FingerCounter::drawPoseAngle(Mat image) const
{
	line(image, currv1, currCenter, Scalar(0, 255, 0));
	line(image, currv2, currCenter, Scalar(0, 255, 0));
	line(image, currv1, currv2, Scalar(0, 0, 255));
}

float FingerCounter::angleBetween(const Point &center, const Point &oldv2)
{
	currCenter = center;
	currv1 = Point(0, center.y);
	currv2 = Point(oldv2.x - center.x, oldv2.y - center.y);

	float len1 = sqrt(currv1.x * currv1.x + currv1.y * currv1.y);
	float len2 = sqrt(currv2.x * currv2.x + currv2.y * currv2.y);

	float dot = currv1.x * currv2.x + currv1.y * currv2.y;

	float a = dot / (len1 * len2);

	if (a >= 1.0) {
		return 0.0;
	}
	
	if (a <= -1.0) {
		return M_PI;
	}
	
	return acos(a); // 0..PI
}
