// Pinger.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Winsock2.h>
#include "iphlpapi.h" 
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
using namespace std;

#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"Ws2_32.lib")
typedef HANDLE (WINAPI* ICMPCREATEFILE)(VOID);   
typedef BOOL (WINAPI* ICMPCLOSEHANDLE)(HANDLE);   
typedef DWORD (WINAPI* ICMPSENDECHO)(HANDLE, DWORD, LPVOID, WORD,PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD);    

//定义三个指针函数   
ICMPCREATEFILE pIcmpCreateFile;   
ICMPCLOSEHANDLE pIcmpCloseHandle;   
ICMPSENDECHO pIcmpSendEcho; 
// 函数功能：初始化ICMP函数：
BOOL InitIcmp()   
{   
	HINSTANCE hIcmp = LoadLibrary(_T("ICMP.DLL"));   
	if(hIcmp==NULL)
	{   
		  return false;
	}  
	pIcmpCreateFile = (ICMPCREATEFILE)GetProcAddress(hIcmp,"IcmpCreateFile");   
	pIcmpCloseHandle = (ICMPCLOSEHANDLE)GetProcAddress(hIcmp,"IcmpCloseHandle");   
	pIcmpSendEcho = (ICMPSENDECHO)GetProcAddress(hIcmp,"IcmpSendEcho");   
	if ((pIcmpCreateFile == NULL)||(pIcmpCloseHandle == NULL)||(pIcmpSendEcho == NULL))   
		return false;   
	return true;   
}

// 函数功能：判断是否能ping通IP
// 函数参数：IP地址或域名
BOOL ICMPPing(const char* host) 
{   
	DWORD timeOut=1000;                                              //设置超时   
	ULONG hAddr=inet_addr(host);                                     //如果是IP地址就直接转换   
	if(hAddr==INADDR_NONE)   
	{   
		hostent* hp=gethostbyname(host);                             //如果是域名就用DNS解析出IP地址   
		if(hp)   
		{
			memcpy(&hAddr,hp->h_addr_list,hp->h_length);             //IP地址   
		}
		else
		{   
			return false;   
		}   
	}   
	HANDLE hIp=pIcmpCreateFile();   
	IP_OPTION_INFORMATION ipoi;   
	memset(&ipoi,0,sizeof(IP_OPTION_INFORMATION));   
	ipoi.Ttl =128;                  //Time-To-Live   

	unsigned char pSend[36];                                                                   //发送包   
	memset(pSend,'E',32);   

	int repSize=sizeof(ICMP_ECHO_REPLY)+32;   
	unsigned char pReply[100];                                                                 //接收包   
	ICMP_ECHO_REPLY* pEchoReply=(ICMP_ECHO_REPLY*)pReply;   

	DWORD nPackets=pIcmpSendEcho(hIp,hAddr,pSend,32,&ipoi,pReply,repSize,timeOut);             //发送ICMP数据报文   

	if(0 != pEchoReply->Status)                                                                  //超时，可能是主机禁用了ICMP 或者目标主机不存在  
	{   
		pIcmpCloseHandle(hIp);   
		return false;   
	}   

	pIcmpCloseHandle(hIp);   
	return true;   
}

void DetectRadar(const char* radarIp)
{
	BOOL pingResult = FALSE;
	int pingTimes = 0;
	do 
	{
		pingResult = ICMPPing(radarIp);
		pingTimes++;
		if (3 < pingTimes)
		{
			printf("网络不通\n");
			break;
		}

	} while (FALSE == pingResult);

	if (TRUE == pingResult)
	{
		printf("网络正常\n");
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	InitIcmp();

	
    ifstream infile; 
    infile.open(".\\ip.txt");   //将文件流对象与文件连接起来 
    if (false == infile.is_open())  //若失败,则输出错误消息,并终止程序运行 
	{
		printf("ip.txt文件缺少，执行默认检测\n");
		printf("正在检测江宁雷达站（192.168.108.81）：");
		DetectRadar("192.168.108.81");
		printf("正在检测桥林雷达站（192.168.108.82）：");
		DetectRadar("192.168.108.82");
		printf("正在检测江铺雷达站（192.168.108.83）：");
		DetectRadar("192.168.108.83");
		printf("正在检测高铁桥雷达站（192.168.110.84）：");
		DetectRadar("192.168.110.84");
		printf("正在检测大桥雷达站（192.168.108.85）：");
		DetectRadar("192.168.108.85");
		printf("正在检测西方角雷达站（192.168.108.86）：");
		DetectRadar("192.168.108.86");
		printf("正在检测天河口雷达站（192.168.108.87）：");
		DetectRadar("192.168.108.87");
		printf("正在检测摄山雷达站(192.168.108.88)：");
		DetectRadar("192.168.108.88");
		printf("正在检测仪征雷达站(192.168.111.89)：");
		DetectRadar("192.168.111.89");
	}

    string s;
	int pos;
	int len;
	string name;
	string ip;
    while(getline(infile,s))
    {
		pos = s.find(':');
		len = s.size();
		if (-1 == pos)
		{
			pos = s.find('：');
		}
		if (0 < pos && pos < len)
		{
			name = s.substr(0,pos);
			ip = s.substr(pos + 1, len);
			printf("正在检测%s（%s)：", name.c_str(), ip.c_str());
			DetectRadar(ip.c_str());
		}
    }
    infile.close();             //关闭文件输入流 
	printf("检测完成\n");
	
	getchar();
	return 0;
}

