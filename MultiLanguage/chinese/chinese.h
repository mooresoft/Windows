// chinese.h : chinese DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "..\MultiLanguage\resource.h"		// ������


// CchineseApp
// �йش���ʵ�ֵ���Ϣ������� chinese.cpp
//

class CchineseApp : public CWinApp
{
public:
	CchineseApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
