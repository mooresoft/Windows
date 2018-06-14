
// MultiLanguage.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MultiLanguage.h"
#include "MultiLanguageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiLanguageApp

BEGIN_MESSAGE_MAP(CMultiLanguageApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMultiLanguageApp ����

CMultiLanguageApp::CMultiLanguageApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_isChinese = TRUE;
	m_LanguageDll = NULL;
}


// Ψһ��һ�� CMultiLanguageApp ����

CMultiLanguageApp theApp;


// CMultiLanguageApp ��ʼ��

BOOL CMultiLanguageApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	if (m_isChinese)
	{
		m_LanguageDll = ::LoadLibrary(_T("chinese.dll"));
	}
	else
	{
		m_LanguageDll = ::LoadLibrary(_T("english.dll"));
	}
	AfxSetResourceHandle(m_LanguageDll);

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CMultiLanguageDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}



int CMultiLanguageApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (NULL != m_LanguageDll)
	{
		AfxFreeLibrary(m_LanguageDll);
	}
	return CWinApp::ExitInstance();
}
