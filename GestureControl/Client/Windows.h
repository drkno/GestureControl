#pragma once

#include <opencv2/highgui/highgui.hpp>
#include <unordered_map>
#include <string>

using namespace std;
using namespace cv;

#define imshowext imshow
//
//unordered_map<string, bool> _windows;
//
//inline void setWindowIsVisible(string windowName, bool isVisible)
//{
//	_windows[windowName] = isVisible;
//}
//
//inline void imshow_ext(string windowName, InputArray image)
//{
//	unordered_map<string, bool>::const_iterator f = _windows.find(windowName);
//	if (f == _windows.end())
//	{
//		_windows[windowName] = true;
//	}
//	else if (!f->second)
//	{
//		return;
//	}
//	imshow(windowName, image);
//}