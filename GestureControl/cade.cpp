// Imported files used in this program
#include <opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <stdio.h>
#include <iostream>
#include <sstream>

// Using namespaces makes method calls easier and more compact
using namespace cv;
using namespace std;


// Background subtraction matrices
Mat image, previousFrame, differenceImage;
Mat fgMaskMOG;
// Background subtractor initialisation
Ptr<BackgroundSubtractorMOG2> pMOG = createBackgroundSubtractorMOG2();
// Canny image matrix
Mat canny;
// Grayscale image matrix
Mat gray_image;

// Variables for the Covexity defects algorithm
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);



/// Function headers
void thresh_callback(int, void*);
int findBiggestContour(vector<vector<Point> > contours);


int main() {

	VideoCapture capture(1); // connect to a camera
	capture >> image; // get the first frame

					  // Get the background image for background subtraction
	pMOG->getBackgroundImage(fgMaskMOG);

	// Continue the program until the user presses any button
	while (waitKey(10) < 0) {
		// Take the frame and save it to the image matrix
		capture >> image;
		// Compare the current frame with the background image for background subtraction
		pMOG->apply(image, fgMaskMOG);

		// Get the background subtracted image and perform median blur to remove noise
		gray_image = fgMaskMOG;
		blur(gray_image, gray_image, Size(3, 3));
		medianBlur(gray_image, gray_image, 3);
		medianBlur(gray_image, gray_image, 5);

		// Use the Canny algorithm to detect the edges of the hand
		Canny(gray_image, canny, 50, 150);

		// Dilate the canny image to make sure the outline is a solid border with no gaps
		dilate(canny, canny, Mat(), Point(-1, -1), 1, 1, 1);

		// Call the convex hull function thresh_callback
		createTrackbar(" Threshold:", "Source", &thresh, max_thresh, thresh_callback);
		thresh_callback(0, 0);

		// Create the windows to display the steps of the program to the user
		namedWindow("Image");
		imshow("Image", image);

		namedWindow("Foreground Mask");
		imshow("Foreground Mask", gray_image);

		namedWindow("Canny");
		imshow("Canny", canny);
	}
}

/** @function thresh_callback */
void thresh_callback(int, void*) {
	// Copy the current frame
	Mat src_copy = image.clone();
	// Create a threshold matrix
	Mat threshold_output;
	// Create the vectors for the contours
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	// Detect edges using Threshold
	threshold(canny, threshold_output, thresh, 255, THRESH_BINARY);

	// Find contours
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	// Create the vectors for the convexity defects function
	vector<vector<Point>> hull(contours.size());
	vector<int> hullsI;
	vector<Vec4i> defects;

	// Find the largest convex hull in the image
	int indexBiggestContour = findBiggestContour(contours);

	// If there is a convex hull in the image
	if (indexBiggestContour >= 0) {
		// Calculate the convex hull for display purposes
		convexHull(Mat(contours[indexBiggestContour]), hull[indexBiggestContour], false);
		// Calculate the convexity defects of the largest convex hull
		convexHull(Mat(contours[indexBiggestContour]), hullsI, true);
		convexityDefects(contours[indexBiggestContour], hullsI, defects);
	}

	// Analyse the depths of each convexity defects to see if it fits the finger gap threshold
	int counter = 0;
	for (int i = 0; i < defects.size(); i++)
	{
		int startIdx = defects[i].val[0];
		int endIdx = defects[i].val[1];
		int defectPtIdx = defects[i].val[2];
		double depth = (double)defects[i].val[3] / 256.0f;

		if (depth > 90 && depth < 200) {
			counter++;
			printf("Depth: %f\n", depth);
		}
	}
	if (counter > 0) {
		printf("Total Count = %d\n", counter);
	}

	// Draw contours + hull results
	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		drawContours(drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point());
	}

	// Show the convex hull in a window
	namedWindow("Hull demo", CV_WINDOW_AUTOSIZE);
	imshow("Hull demo", drawing);
}

/* @function findBiggestContour
*	Finds the largest contour in the image and returns the index in the contour array
*/
int findBiggestContour(vector<vector<Point> > contours)
{
	int indexOfBiggestContour = -1;
	int sizeOfBiggestContour = 0;

	for (int i = 0; i < contours.size(); i++) {
		if (contours[i].size() > sizeOfBiggestContour) {
			sizeOfBiggestContour = contours[i].size();
			indexOfBiggestContour = i;
		}
	}
	return indexOfBiggestContour;
}