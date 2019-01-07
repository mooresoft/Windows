#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WinSock2.h>

int main()
{
	WORD ver = MAKEWORD(2, 2);
	WSADATA wsa;


	WSAStartup(ver, &wsa);

	WSACleanup();
	return 0;
}