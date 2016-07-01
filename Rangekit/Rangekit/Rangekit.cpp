
// Rangekit.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Rangekit.h"
#include "MainFrm.h"
#include "RangekitDoc.h"
#include "RangekitView.h"
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRangekitApp

BEGIN_MESSAGE_MAP(CRangekitApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CRangekitApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CRangekitApp construction

CRangekitApp::CRangekitApp()
{
	m_bHiColorIcons = TRUE;

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
	SetAppID(_T("Rangekit.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	InitLogPath();
}

// The one and only CRangekitApp object

CRangekitApp theApp;


// CRangekitApp initialization

BOOL CRangekitApp::InitInstance()
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

	CWinAppEx::InitInstance();

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
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CRangekitDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CRangekitView));
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
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

int CRangekitApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);
	if (true == m_file.is_open())
	{
		m_file.close();
	}
	return CWinAppEx::ExitInstance();
}

// CRangekitApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CRangekitApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CRangekitApp customization load/save methods

void CRangekitApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CRangekitApp::LoadCustomState()
{
}

void CRangekitApp::SaveCustomState()
{
}

int FindFile(CString strDir, CStringArray& caFiles)
{
	int nFileCnts = 0;

	HANDLE hfile;
	WIN32_FIND_DATA wfdata;
	BOOL IsOver = false;
	CString strname;
	CString strfull;
	CString str = strDir + _T("\\*.*");
	hfile = FindFirstFile(str, &wfdata);
	if (hfile == INVALID_HANDLE_VALUE)
		IsOver = true;
	while (!IsOver)
	{
		strname.Format(_T("%s"), wfdata.cFileName);
		strfull = strDir + _T("\\") + strname;
		if ((wfdata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) && (wfdata.cFileName[0] != _TEXT('.')))
		{
			SetCurrentDirectory(wfdata.cFileName);
			nFileCnts += FindFile(strfull, caFiles);
			SetCurrentDirectory(_T(".."));
		}
		else if (wfdata.cFileName[0] != _TEXT('.'))
		{
			if (-1 != strfull.Find(_T(".log"))
				&& -1 != strfull.Find(_T("wxrangekit")))
			{
				caFiles.Add(strfull);
				nFileCnts++;
			}
		}
		IsOver = !FindNextFile(hfile, &wfdata);
	}
	FindClose(hfile);

	return nFileCnts;
}

VOID CRangekitApp::DeleteFiles(VOID)
{
	CString FilePath;
	CStringArray caFiles;
	CFileStatus rStatus;
	CTime curTime;
	curTime = CTime::GetCurrentTime();
	CTimeSpan timeSpan;
	LONGLONG days;

	FilePath = m_filepath;
	int nFiles = FindFile(FilePath, caFiles);
	
	for (int i = 0; i < nFiles; i++)
	{
		if (CFile::GetStatus(caFiles[i], rStatus))
		{
			if (curTime > rStatus.m_ctime)
			{
				timeSpan = curTime - rStatus.m_ctime;
				days = timeSpan.GetDays();
				if (7 < days)
				{
					DeleteFile(caFiles[i]);
				}
			}
		}
	}
	return;
}

VOID CRangekitApp::InitLogPath(VOID)
{
	CString filepath;
	SYSTEMTIME sys;
	CString strMsg;
	GetLocalTime(&sys);

	SHGetSpecialFolderPathA(NULL, m_filepath, CSIDL_APPDATA, NULL);

	filepath = m_filepath;
	filepath += "\\wxlog";

	strcat(m_filepath, "\\wxlog");

	if (!PathIsDirectory(filepath))
	{
		if (!CreateDirectory(filepath, NULL))
		{
			strMsg.Format(_T("Create folder \"%s\" fail?"), filepath);
			AfxMessageBox(strMsg);
			return;
		}
	}

	DeleteFiles();

	sprintf(m_filename,
		"%s\\wxrangekit%4d-%02d-%02d_%02d-%02d-%02d.log",
		m_filepath,
		sys.wYear, sys.wMonth, sys.wDay, sys.wHour,
		sys.wMinute, sys.wSecond);

	if (true == m_file.is_open())
	{
		m_file.close();
	}

	return;
}

VOID CRangekitApp::writelog(const char* szLogs)
{
	if (false == m_file.is_open())
	{
		m_file.open(m_filename, ios::in | ios::out | ios::app | ios::binary);
	}

	if (true == m_file.is_open())
	{
		m_file << (char*)szLogs;
		m_file.flush();
	}
	
	return;
}

VOID CRangekitApp::OpenLogFolder(VOID)
{
	ShellExecuteA(NULL, "open", m_filepath, NULL, NULL, SW_SHOWNORMAL);
}

// CrangekitApp message handlers
INT32 CRangekitApp::CStringToChar(const CString& strSrc, CHAR *szDst, INT32 nDstLen)
{
	INT32 lStrCount;
	INT32 lByteCount = -1;
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

	delete [] pcTmpStr;

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


