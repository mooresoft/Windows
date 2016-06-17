
// Topology.h : main header file for the Topology application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "MainFrm.h"

// CTopologyApp:
// See Topology.cpp for the implementation of this class
//

class CTopologyApp : public CWinAppEx
{
public:
	CTopologyApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	int CStringToChar(const CString& strSrc, CHAR *szDst, int nDstLen);
// Implementation
	BOOL  m_bHiColorIcons;
	CMainFrame* m_pMainFrame;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTopologyApp theApp;
