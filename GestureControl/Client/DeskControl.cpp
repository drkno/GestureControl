#include "DeskControl.h"
#include <string>
using namespace std;

DeskControl::DeskControl()
{
	client = new http_client(U("http://harest.makereti.co.nz:9834/"));
}

void DeskControl::sendCommand(method method, uri location, string data) const
{
	http_request request(method);
	request.set_request_uri(location);
	request.set_body(data);
	request.headers().add(header_names::content_type, "application/json;charset=UTF-8");
	auto resp = client->request(request).get();
	resp.content_ready().get();
}

void DeskControl::up(int time) const
{
	auto data = "{timeout:" + to_string(time) + "}";
	sendCommand(methods::POST, U("/api/up"), data);
}

void DeskControl::down(int time) const
{
	auto data = "{timeout:" + to_string(time) + "}";
	sendCommand(methods::POST, U("/api/down"), data);
}

void DeskControl::abort() const
{
	sendCommand(methods::GET, U("/api/abort"));
}

DeskControl::~DeskControl()
{
	delete client;
}
