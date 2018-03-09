// Semaphore.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <process.h>
#include <windows.h>
using namespace std;
HANDLE hsem1,hsem2,hsem3;
BOOL bExit = FALSE;
DWORD dwTick = 0;

unsigned __stdcall threadFunA(void*)
{
	DWORD dwCurTick;
	DWORD dwRet;
	while (FALSE == bExit)
	{
		dwRet = WaitForSingleObject(hsem1, 500);
		dwCurTick = GetTickCount();
		switch (dwRet)
		{
		case WAIT_OBJECT_0:
			cout<<"wait "<<(dwCurTick - dwTick)<<"ms WAIT_OBJECT_0 "<<dwRet<<endl;
			break;
		case WAIT_TIMEOUT:
			cout<<"wait "<<(dwCurTick - dwTick)<<"ms WAIT_TIMEOUT "<<dwRet<<endl;
			break;
		case WAIT_FAILED:
			cout<<"wait "<<(dwCurTick - dwTick)<<"ms WAIT_FAILED "<<dwRet<<endl;
			break;
		default:
			cout<<"wait "<<(dwCurTick - dwTick)<<"ms UNKNOWN "<<dwRet<<endl;
			break;
		}
		
		dwTick = dwCurTick;
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	hsem1 = CreateSemaphore(NULL, 0, 1, NULL);
	HANDLE hth1;
	char chr;
	hth1 = (HANDLE)_beginthreadex(NULL, 0, threadFunA, NULL, 0, NULL);
	while (1)
	{
		chr = getchar();
		if ('x' == chr)
		{
			bExit = TRUE;
			WaitForSingleObject(hth1, INFINITE);
		}
		else
		{
			ReleaseSemaphore(hsem1, 1, NULL);
		}
	}
	return 0;
}

