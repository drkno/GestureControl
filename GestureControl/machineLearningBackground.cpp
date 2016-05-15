#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;
using namespace cv;

int machineLearn(int argc, char *argv[])
{
	Mat frame;
	Mat back;
	Mat fore;
	vector<pair<Point, double> > palm_centers;
	VideoCapture cap(0);
	Ptr<BackgroundSubtractorMOG2> bg = createBackgroundSubtractorMOG2();
	bg->setNMixtures(3);
	bg->setDetectShadows(true);


	//namedWindow("Frame");
	namedWindow("Background");
	int backgroundFrame = 500;

	for (int i = 0; i < 100; i++)
	{
		cap >> frame;
		bg->apply(frame, fore);
		imshow("Background", frame);
		if (waitKey(10) >= 0) break;
	}
	cout << "here" << endl;
	while (true)
	{
		cap >> frame;
		Mat diffImage;
		Mat backgroundImage;
		Mat a, b;
		bg->getBackgroundImage(backgroundImage);

		cv::absdiff(backgroundImage, frame, diffImage);

		cv::Mat foregroundMask = cv::Mat::zeros(diffImage.rows, diffImage.cols, CV_8UC1);

		float threshold = 30.0f;
		float dist;

		for (int j = 0; j < diffImage.rows; ++j) {
			for (int i = 0; i < diffImage.cols; ++i)
			{
				cv::Vec3b pix = diffImage.at<cv::Vec3b>(j, i);

				dist = (pix[0] * pix[0] + pix[1] * pix[1] + pix[2] * pix[2]);
				dist = sqrt(dist);

				if (dist > threshold)
				{
					foregroundMask.at<unsigned char>(j, i) = 255;
				}
			}
		}

		imshow("Background", diffImage);
		if (waitKey(10) >= 0) break;

	}
}