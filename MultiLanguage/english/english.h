// english.h : english DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "..\MultiLanguage\resource.h"		// ������


// CenglishApp
// �йش���ʵ�ֵ���Ϣ������� english.cpp
//

class CenglishApp : public CWinApp
{
public:
	CenglishApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
