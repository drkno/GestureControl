#include "BackgroundSubtractor.h"

BackgroundRemover::BackgroundRemover(int learnFrames)
{
	bg = createBackgroundSubtractorMOG2();	// gaussian background learner
	counter = learnFrames;	// number of frames to learn for
}


BackgroundRemover::~BackgroundRemover()
{
	bg.release();
}

bool BackgroundRemover::apply(Mat image)
{
	if (counter > 0)
	{
		Mat mask;
		bg->apply(image, mask);	// learn the background
		counter--;
		return false;
	}

	Mat background;
	bg->apply(image, currentBackgroundSubtraction);	// get a difference image/mask

	return true;
}

Mat BackgroundRemover::getBackgroundSubtraction() const
{
	return this->currentBackgroundSubtraction;
}
