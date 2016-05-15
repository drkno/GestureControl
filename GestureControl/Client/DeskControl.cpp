#include "DeskControl.h"
#include <string>
using namespace std;

DeskControl::DeskControl()
{
	//client = new http_client(U("http://harest.makereti.co.nz:8080/"));
}

void DeskControl::sendCommand(method method, uri location, string data) const
{
	return;
	http_request request(method);
	request.set_request_uri(location);
	request.set_body(data);
	request.headers().add(header_names::content_type, "application/json;charset=UTF-8");
	auto resp = client->request(request).get();
	resp.content_ready().get();
}

void DeskControl::up(int time) const
{
	cout << "Up " << time << endl;
	auto data = "{timeout:" + to_string(time) + "}";
	sendCommand(methods::POST, U("/api/up"), data);
}

void DeskControl::down(int time) const
{
	cout << "Down " << time << endl;
	auto data = "{timeout:" + to_string(time) + "}";
	sendCommand(methods::POST, U("/api/down"), data);
}

void DeskControl::abort() const
{
	cout << "Abort" << endl;
	sendCommand(methods::GET, U("/api/abort"));
}

DeskControl::~DeskControl()
{
	delete client;
}
