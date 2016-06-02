#pragma once

/*
	Cross Platform pause function
	Copyright (c) Matthew Knox 2005.
	Last updated: 02/06/2016
*/

#ifndef _WIN32
#include <termios.h>
#include <unistd.h>
#include <unistd.h>
#define GetCurrentDir getcwd

inline int getch() {
	struct termios oldt,
		newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}
#else
#include <conio.h>
#include <direct.h>
#define GetCurrentDir _getcwd
#define getch _getch
#endif

inline void pause()
{
	std::cout << "Press any key to continue . . .";
	auto c = getch();
	if (c != '\n' && c != '\r')
	{
		std::cout << std::endl;
	}
}

inline const char* getWorkingDirectory() {
	// adapted from http://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from
	char currPath[FILENAME_MAX];
	if (!GetCurrentDir(currPath, sizeof(currPath)))
	{
		throw errno;
	}
	currPath[sizeof(currPath) - 1] = '\0';
	return currPath;
}