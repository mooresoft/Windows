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

//��������ָ�뺯��   
ICMPCREATEFILE pIcmpCreateFile;   
ICMPCLOSEHANDLE pIcmpCloseHandle;   
ICMPSENDECHO pIcmpSendEcho; 
// �������ܣ���ʼ��ICMP������
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

// �������ܣ��ж��Ƿ���pingͨIP
// ����������IP��ַ������
BOOL ICMPPing(const char* host) 
{   
	DWORD timeOut=1000;                                              //���ó�ʱ   
	ULONG hAddr=inet_addr(host);                                     //�����IP��ַ��ֱ��ת��   
	if(hAddr==INADDR_NONE)   
	{   
		hostent* hp=gethostbyname(host);                             //�������������DNS������IP��ַ   
		if(hp)   
		{
			memcpy(&hAddr,hp->h_addr_list,hp->h_length);             //IP��ַ   
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

	unsigned char pSend[36];                                                                   //���Ͱ�   
	memset(pSend,'E',32);   

	int repSize=sizeof(ICMP_ECHO_REPLY)+32;   
	unsigned char pReply[100];                                                                 //���հ�   
	ICMP_ECHO_REPLY* pEchoReply=(ICMP_ECHO_REPLY*)pReply;   

	DWORD nPackets=pIcmpSendEcho(hIp,hAddr,pSend,32,&ipoi,pReply,repSize,timeOut);             //����ICMP���ݱ���   

	if(0 != pEchoReply->Status)                                                                  //��ʱ������������������ICMP ����Ŀ������������  
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
			printf("���粻ͨ\n");
			break;
		}

	} while (FALSE == pingResult);

	if (TRUE == pingResult)
	{
		printf("��������\n");
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	InitIcmp();

	
    ifstream infile; 
    infile.open(".\\ip.txt");   //���ļ����������ļ��������� 
    if (false == infile.is_open())  //��ʧ��,�����������Ϣ,����ֹ�������� 
	{
		printf("ip.txt�ļ�ȱ�٣�ִ��Ĭ�ϼ��\n");
		printf("���ڼ�⽭���״�վ��192.168.108.81����");
		DetectRadar("192.168.108.81");
		printf("���ڼ�������״�վ��192.168.108.82����");
		DetectRadar("192.168.108.82");
		printf("���ڼ�⽭���״�վ��192.168.108.83����");
		DetectRadar("192.168.108.83");
		printf("���ڼ��������״�վ��192.168.110.84����");
		DetectRadar("192.168.110.84");
		printf("���ڼ������״�վ��192.168.108.85����");
		DetectRadar("192.168.108.85");
		printf("���ڼ���������״�վ��192.168.108.86����");
		DetectRadar("192.168.108.86");
		printf("���ڼ����ӿ��״�վ��192.168.108.87����");
		DetectRadar("192.168.108.87");
		printf("���ڼ����ɽ�״�վ(192.168.108.88)��");
		DetectRadar("192.168.108.88");
		printf("���ڼ�������״�վ(192.168.111.89)��");
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
			pos = s.find('��');
		}
		if (0 < pos && pos < len)
		{
			name = s.substr(0,pos);
			ip = s.substr(pos + 1, len);
			printf("���ڼ��%s��%s)��", name.c_str(), ip.c_str());
			DetectRadar(ip.c_str());
		}
    }
    infile.close();             //�ر��ļ������� 
	printf("������\n");
	
	getchar();
	return 0;
}

