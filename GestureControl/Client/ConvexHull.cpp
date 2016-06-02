#include "ConvexHull.h"

ConvexHull::ConvexHull(int drawMode) {
	namedWindow(windowName, WINDOW_AUTOSIZE);
	this->drawMode = drawMode;
}

void ConvexHull::apply(Mat img)
{
	Mat closing;
	Mat structuringElement = getStructuringElement(MORPH_ELLIPSE, Size(25, 25));
	morphologyEx(img, closing, MORPH_CLOSE, structuringElement); // erode image to join gaps in contours

	// get contours
	vector<Vec4i> hierarchy;
	contours.clear();
	findContours(closing, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));

	// find contour with largest area
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
		convexHull(contours[largest], hull, false, false); // create a convex hull of points on the largest contour

		auto s = -1;
		for (auto i = 0; i < hull.size(); i++)
		{
			auto pt1 = contours[largest][hull[i]];
			auto pt2 = contours[largest][hull[i + 1 % hull.size()]];

			auto d = distanceBetween(pt1, pt2);
			if (s == -1 && d > 20)
			{
				s = i + 1;
				break;
			}
		}

		// Remove points on the curved ends of fingers. Only needed for low resolution cameras or close ups
		if (s != -1)
		{
			vector<int> newHull;
			auto c = -1;
			for (auto i = s; i < hull.size() + s; i++)
			{
				auto end = i % hull.size();
				auto pt1 = contours[largest][hull[end]];
				auto pt2 = contours[largest][hull[(i + 1) % hull.size()]];

				auto d = distanceBetween(pt1, pt2);
				if (c == -1 && d <= 20)
				{
					c = i;
				}
				else if (c != -1 && d > 20)
				{
					int pos = ((i - c) / 2 + c) % hull.size();
					newHull.push_back(hull[pos]);
					c = -1;
				}
				else if (c == -1)
				{
					newHull.push_back(hull[end]);
				}
			}
			hull = newHull;
		}

		// draw circles on fingers
		if (drawMode & CONVEX_HULL_DRAW_SELECTED) {
			drawContours(drawing, contours, largest, Scalar(255, 255, 255));
			for (auto i = 0; i < hull.size(); i++)
			{
				auto next = i + 1 == hull.size() ? 0 : i + 1;
				line(drawing, contours[largest][hull[i]], contours[largest][hull[next]], Scalar(0, 255, 0));
				circle(drawing, contours[largest][hull[i]], 20, Scalar(0, 0, 255), 1);
			}
		}
	}

	// draw the convex hull on the image
	if (drawMode & CONVEX_HULL_DRAW_CONTOURS || drawMode & CONVEX_HULL_DRAW_HULLS) {
		for (auto i = 0; i < contours.size(); i++)
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
	}

	imshowext("Hull", drawing);
}

double ConvexHull::distanceBetween(Point pt1, Point pt2) const
{
	return norm(pt1 - pt2);
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
