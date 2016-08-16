
// Visualcustom.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CVisualcustomApp:
// See Visualcustom.cpp for the implementation of this class
//

class CVisualcustomApp : public CWinApp
{
public:
	CVisualcustomApp();

// Overrides
public:
	virtual BOOL InitInstance();

	int CStringToChar(const CString& strSrc, CHAR *szDst, int nDstLen);
// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CVisualcustomApp theApp;