// ZMQDemo.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "zmq.h"
#include <assert.h>
using namespace std;

int main()
{
	int a, b, c;
	zmq_version(&a, &b, &c);
	std::cout << "zmq-version:" << a << "-" << b << "-" << c << std::endl;

	//  Socket to talk to clients
	void *context = zmq_ctx_new();
	void *responder = zmq_socket(context, ZMQ_REP);
	int rc = zmq_bind(responder, "tcp://*:5555");
	assert(rc == 0);
	int recv_count = 0;
	while (1) {
		char buffer[10] = { 0 };
		int recv_len = zmq_recv(responder, buffer, 10, 0);
		recv_count++;
		printf(buffer);
		printf("\n");
		printf("recv_len = %d, recv_count = %d\n", recv_len, recv_count);
		Sleep(1);          //  Do some 'work'
		zmq_send(responder, "World", 5, 0);
	}

	getchar();
    return 0;
}

