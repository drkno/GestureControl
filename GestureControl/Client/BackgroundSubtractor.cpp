#include "BackgroundSubtractor.h"

BackgroundRemover::BackgroundRemover()
{
	bg = createBackgroundSubtractorMOG2();
	counter = 100;
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
		bg->apply(image, mask);
		counter--;
		return false;
	}

	Mat background;
	bg->apply(image, currentBackgroundSubtraction);

	return true;
}

Mat BackgroundRemover::getBackgroundSubtraction() const
{
	return this->currentBackgroundSubtraction;
}
