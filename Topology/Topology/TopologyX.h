
// TopologyX.h : TopologyX Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CTopologyXApp:
// �йش����ʵ�֣������ TopologyX.cpp
//

class CTopologyXApp : public CWinApp
{
public:
	CTopologyXApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	int CStringToChar(const CString& strSrc, CHAR *szDst, int nDstLen);
};

extern CTopologyXApp theApp;
