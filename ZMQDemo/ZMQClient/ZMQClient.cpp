// ZMQClient.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "zmq.h"
#include <assert.h>
using namespace std;

int main()
{
	void *context = zmq_ctx_new();
	void *requester = zmq_socket(context, ZMQ_REQ);
	zmq_connect(requester, "tcp://localhost:5555");

	int send_count = 0;
	for (int request_nbr = 0; request_nbr != 10; request_nbr++)
	{
		int send_len = zmq_send(requester, "Hello", 5, 0);
		send_count++;
		printf("send_len = %d, send_count = %d\n", send_len, send_count);
		char buffer[10] = { 0 };
		zmq_recv(requester, buffer, 10, 0);
		printf(buffer);
		printf("\n");
	}
	
	getchar();
    return 0;
}

