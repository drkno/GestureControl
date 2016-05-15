#pragma once

#include <cpprest/http_client.h>

using namespace std;
using namespace utility;
using namespace web;
using namespace http;
using namespace client;
using namespace concurrency::streams;

class DeskControl
{
public:
	DeskControl();
	void sendCommand(method method, uri location, string data = "") const;
	void up(int time) const;
	void down(int time) const;
	void abort() const;
	~DeskControl();

private:
	http_client *client;
};

