#include "CannyEdgeTuner.h"
#include "ConvexHull.h"
#include "DeskControl.h"
#include "FingerCounter.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	try {
		DeskControl control;
		VideoCapture cap;
		Mat src;

		auto input = 5;
		switch (input)
		{
		default:
		case 0: {
			cap = VideoCapture(0);
			cap >> src;
			break;
		}
		case 1: {
			src = imread("C:\\Users\\Matthew\\Desktop\\test.jpg");
			break;
		}
		case 2: {
			src = imread("C:\\Users\\Matthew\\Desktop\\untitled.jpg");
			break;
		}
		case 3: {
			src = imread("C:\\Users\\Matthew\\Documents\\University\\S1 COSC428 Computer Vision\\Untitled.jpg");
			break;
		}
		case 4: {
			src = imread("C:\\Users\\Matthew\\Documents\\University\\S1 COSC428 Computer Vision\\Untitled_Down.jpg");
			break;
		}
		case 5: {
			src = imread("C:\\Users\\Matthew\\Documents\\University\\S1 COSC428 Computer Vision\\Untitled_Up.jpg");
			break;
		}
		}

		namedWindow("Input", WINDOW_AUTOSIZE);
		namedWindow("test");

		CannyEdgeTuner edgeTuner(src.size(), src.type());
		ConvexHull convxHull;
		FingerCounter fingerCounter;

		while (waitKey(10) == -1)
		{
			Mat hsv;
			cvtColor(src, hsv, CV_RGB2HSV);

			edgeTuner.apply(src);
			auto edges = edgeTuner.getEdges();
			convxHull.apply(edges);

			if (convxHull.hasHull()) {
				auto hull = convxHull.getLargestHullPoints();
				auto contour = convxHull.getLargestHullContour();

				Mat frame2;
				src.copyTo(frame2);

				fingerCounter.apply(hull, contour);

				fingerCounter.drawPoseAngle(frame2);

				auto pose = fingerCounter.getOrientation();
				auto fingers = fingerCounter.getNumberOfFingers();
				cout << fingers << endl;
				cout << (pose == Up ? "Up" : "Down") << endl;
				if (fingers <= 0 || fingers > 5)
				{
					control.abort();
				}
				else if (pose == Up)
				{
					control.up(fingers);
				}
				else
				{
					control.down(fingers);
				}

				imshow("test", frame2);
			}
			else
			{
				imshow("test", src);
			}

			imshow("Input", hsv);
			if (input == 0) {
				cap >> src;
			}
		}
	}
	catch(Exception exception)
	{
		cout << exception.msg << endl;
	}
	return 0;
}