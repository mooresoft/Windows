
// VisualDistance.h : main header file for the VisualDistance application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "MainFrm.h"
#include "VisualDistanceView.h"

// CVisualDistanceApp:
// See VisualDistance.cpp for the implementation of this class
//

class CVisualDistanceApp : public CWinApp
{
public:
	CVisualDistanceApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	CMainFrame* m_pMainFrame;
	CVisualDistanceView* m_pVisualView;
	int CStringToChar(const CString& strSrc, CHAR *szDst, int nDstLen);

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CVisualDistanceApp theApp;
