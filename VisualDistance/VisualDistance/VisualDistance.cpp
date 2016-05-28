
// VisualDistance.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "VisualDistance.h"

#include "VisualDistanceDoc.h"
#include "VisualDistanceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVisualDistanceApp

BEGIN_MESSAGE_MAP(CVisualDistanceApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CVisualDistanceApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CVisualDistanceApp construction

CVisualDistanceApp::CVisualDistanceApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("VisualDistance.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pMainFrame = NULL;
}

// The one and only CVisualDistanceApp object

CVisualDistanceApp theApp;


// CVisualDistanceApp initialization

BOOL CVisualDistanceApp::InitInstance()
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


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Visual Distance"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CVisualDistanceDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CVisualDistanceView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CVisualDistanceApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CVisualDistanceApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CVisualDistanceApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CVisualDistanceApp message handlers
int CVisualDistanceApp::CStringToChar(const CString& strSrc, CHAR *szDst, int nDstLen)
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



