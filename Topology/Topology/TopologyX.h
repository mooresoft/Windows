
// TopologyX.h : TopologyX 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CTopologyXApp:
// 有关此类的实现，请参阅 TopologyX.cpp
//

class CTopologyXApp : public CWinApp
{
public:
	CTopologyXApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	int CStringToChar(const CString& strSrc, CHAR *szDst, int nDstLen);
};

extern CTopologyXApp theApp;
