
// MultiLanguage.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMultiLanguageApp:
// �йش����ʵ�֣������ MultiLanguage.cpp
//

class CMultiLanguageApp : public CWinApp
{
public:
	CMultiLanguageApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_isChinese;
	HINSTANCE m_LanguageDll;
public:
	virtual int ExitInstance();
};

extern CMultiLanguageApp theApp;