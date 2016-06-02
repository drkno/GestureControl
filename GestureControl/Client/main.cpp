#include "CannyEdgeTuner.h"
#include "ConvexHull.h"
#include "DeskControl.h"
#include "FingerCounter.h"
#include "BackgroundSubtractor.h"
#include "Windows.h"
#include "cp.h"

using namespace std;
using namespace cv;

int handleError(int,const char*,const char*,const char*,int,void*)
{
	// noop stderr handler
	return 0;
}

int main(int argc, char** argv)
{
	cout << "###############################################\n"
		<< "#                                             #\n"
		<< "#           Gesture Control Client            #\n"
		<< "#  <https://github.com/mrkno/GestureControl>  #\n"
		<< "#                                             #\n"
		<< "###############################################"
		<< endl
		<< "Image Search Directory:" << endl << getWorkingDirectory() << endl;

	// we will catch the exceptions, this will prevent logging to stderr
	redirectError(handleError);

	try {
		DeskControl control;
		VideoCapture cap;
		Mat src;

start:
		auto input = 1;
		switch (input)
		{
			default:
			case 0: { // live video
				cap = VideoCapture(0);
				cap >> src;
				break;
			}
			case 1: {
				cap = VideoCapture("3down.mp4");
				cap >> src;
				break;
			}
			case 2: {
				cap = VideoCapture("5up.mp4");
				cap >> src;
				break;
			}
			case 3: {
				src = imread("Images\\1.jpg");
				break;
			}
			case 4: {
				src = imread("Images\\2.jpg");
				break;
			}
			case 5: {
				src = imread("Images\\3.jpg");
				break;
			}
			case 6: {
				src = imread("Images\\4.jpg");
				break;
			}
			case 7: {
				src = imread("Images\\5.jpg");
				break;
			}
			case 8: {
				src = imread("Images\\6.jpg");
				break;
			}
		}

		namedWindow("Input", WINDOW_AUTOSIZE);
		namedWindow("test");

		CannyEdgeTuner edgeTuner(src.size(), src.type());
		ConvexHull convxHull;
		FingerCounter fingerCounter;
		BackgroundRemover remover;

		while (waitKey(50) == -1)
		{
			// convert to HSV colour space
			Mat hsv;
			cvtColor(src, hsv, CV_RGB2HSV); // convert image to hsv colour space

			/*
			// Background subtraction. Enable *ONLY* on live camera feed
			if (!remover.apply(hsv))
			{
				cout << "Still learning background..." << endl;
				continue;
			}

			edgeTuner.apply(remover.getBackgroundSubtraction());
			*/

			edgeTuner.apply(src);
			auto edges = edgeTuner.getEdges();	// canny edge tuner
			convxHull.apply(edges);

			if (convxHull.hasHull()) {
				// get convex hull and contours
				auto hull = convxHull.getLargestHullPoints();
				auto contour = convxHull.getLargestHullContour();

				Mat frame2;
				src.copyTo(frame2);

				/*
					Get estimated number of of fingers and estimated pose.
					Pass number of fingers as time and pose as direction to the desk control rest service.
				*/
				fingerCounter.apply(hull, contour);
				fingerCounter.drawPoseAngle(frame2);
				auto pose = fingerCounter.getOrientation();
				auto fingers = fingerCounter.getNumberOfFingers();

				cout << "Fingers: " << fingers << endl;
				cout << "Pose:    " << (pose == Up ? "Up" : "Down") << endl;
				if (fingers <= 0 || fingers > 5)
				{
					control.abort(); // no or invalid number of fingers? abort
				}
				else if (pose == Up)
				{
					control.up(fingers);
				}
				else
				{
					control.down(fingers);
				}
				cout << "-----" << endl;

				imshowext("test", frame2);
			}
			else
			{
				imshowext("test", src);
			}

			imshowext("Input", hsv);
			if (input <= 2) {
				cap >> src;
				if (src.empty())
				{
					goto start; // nasty workaround for testing videos
				}
			}
		}
	}
	catch(Exception exception)
	{
		// error was unexpected
		cout << exception.msg << endl;
		pause();
	}
	return 0;
}