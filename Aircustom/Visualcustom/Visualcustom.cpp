
// Visualcustom.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Visualcustom.h"
#include "VisualcustomDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVisualcustomApp

BEGIN_MESSAGE_MAP(CVisualcustomApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CVisualcustomApp construction

CVisualcustomApp::CVisualcustomApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CVisualcustomApp object

CVisualcustomApp theApp;


// CVisualcustomApp initialization

BOOL CVisualcustomApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CVisualcustomDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CVisualcustomApp::CStringToChar(const CString& strSrc, CHAR *szDst, int nDstLen)
{
	int lStrCount;
	int lByteCount = -1;
	CHAR *pcTmpStr;

	if ((NULL == szDst) || (0 >= nDstLen))
	{
		return 0;
	}

	memset(szDst, 0, nDstLen);

	lStrCount = strSrc.GetLength();
	if (strSrc.IsEmpty() || (0 == lStrCount))
	{
		return 0;
	}

#ifdef UNICODE
	lByteCount = WideCharToMultiByte(CP_ACP, 0, strSrc, lStrCount, NULL, 0, NULL, NULL);

	if (lByteCount <= 0)
	{
		return lByteCount;
	}

	pcTmpStr = new CHAR[lByteCount];
	memset(pcTmpStr, 0, lByteCount);

	WideCharToMultiByte(CP_ACP, 0, strSrc, lStrCount, pcTmpStr, lByteCount, NULL, NULL);


	if (lByteCount <= nDstLen)
	{
		memcpy_s(szDst, nDstLen, pcTmpStr, lByteCount);
	}

	delete[] pcTmpStr;

	return lByteCount;
#else
	if (lStrCount >= nDstLen)
	{
		return 0;
	}
	else
	{
		memcpy_s(szDst, nDstLen, strSrc, lStrCount);
		return lStrCount;
	}
#endif
}