
// Rangekit.h : main header file for the Rangekit application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include <fstream>
using namespace std;
// CRangekitApp:
// See Rangekit.cpp for the implementation of this class
//
#define WM_ADD_TEXT			WM_USER + 0x100
#define WM_WRITE_SERAIL	    WM_USER + 0x101
#define WM_TRANSMIT_YMODEM	WM_USER + 0x102

#define SERIAL_BUF_SIZE			4096

class CRangekitApp : public CWinAppEx
{
public:
	CRangekitApp();

	int CStringToChar(const CString& strSrc, CHAR *szDst, INT32 nDstLen);
	VOID OpenLogFolder(VOID);
private:
	VOID DeleteFiles(VOID);


	fstream m_file;
	char    m_filename[MAX_PATH];
	char    m_filepath[MAX_PATH];
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	VOID InitLogPath(VOID);
	VOID writelog(const char* szLogs);
// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRangekitApp theApp;
