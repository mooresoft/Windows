
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Rangekit.h"
#include "MainFrm.h"
#include "DlgSetSleepTime.h"
#include "DlgSetCustom.h"
#include "DlgSetCoordinate.h"
#include "DlgSetAntennaDelay.h"
#include "Ymodem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_WM_CLOSE()
	ON_COMMAND(ID_CONNECT_OPEN, &CMainFrame::OnConnectOpen)
	ON_COMMAND(ID_CONNECT_REOPEN, &CMainFrame::OnConnectClose)
	ON_COMMAND(ID_VIEW_OPERATORMODE, &CMainFrame::OnViewOperatormode)
	ON_COMMAND(ID_VIEW_ANCHORLIST, &CMainFrame::OnViewAnchorlist)
	ON_COMMAND(ID_VIEW_MAC, &CMainFrame::OnViewMac)
	ON_COMMAND(ID_VIEW_RFMODE, &CMainFrame::OnViewRfmode)
	ON_MESSAGE(WM_WRITE_SERAIL, OnWriteSerial)
	ON_WM_TIMER()
	ON_COMMAND(ID_SETTING_MODE, &CMainFrame::OnSettingMode)
	ON_COMMAND(ID_SETTING_ANCHOR, &CMainFrame::OnSettingAnchor)
	ON_COMMAND(ID_SETTING_RFMODE, &CMainFrame::OnSettingRfmode)
	ON_COMMAND(ID_SETTING_REBOOT, &CMainFrame::OnSettingReboot)
	ON_COMMAND(ID_SETTING_RESET, &CMainFrame::OnSettingReset)
	ON_COMMAND(ID_HELP_CLEAR, &CMainFrame::OnHelpClear)
	ON_COMMAND(ID_VIEW_SLEEPTIME, &CMainFrame::OnViewSleeptime)
	ON_COMMAND(ID_SETTING_SLEEPTIME, &CMainFrame::OnSettingSleeptime)
	ON_COMMAND(ID_SETTING_CUSTOM, &CMainFrame::OnSettingCustom)
	ON_COMMAND(ID_SETTING_COORDINATE, &CMainFrame::OnSettingCoordinate)
	ON_COMMAND(ID_VIEW_COORDINATE, &CMainFrame::OnViewCoordinate)
	ON_COMMAND(ID_VIEW_ANTENNADELAY, &CMainFrame::OnViewAntennadelay)
	ON_COMMAND(ID_SETTING_ANTENNADELAY, &CMainFrame::OnSettingAntennadelay)
	ON_COMMAND(ID_TRANSFER_SENDYMODEM, &CMainFrame::OnTransferSendymodem)
	ON_MESSAGE(WM_TRANSMIT_YMODEM, OnTransmitYmodem)
	ON_COMMAND(ID_HELP_LOGFOLDER, &CMainFrame::OnHelpLogfolder)
	ON_COMMAND(ID_TEST_POWER, &CMainFrame::OnTestPower)
	ON_UPDATE_COMMAND_UI(ID_TEST_POWER, &CMainFrame::OnUpdateTestPower)
	ON_COMMAND(ID_SETTING_AJUSTLIST, &CMainFrame::OnSettingAjustlist)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction
DWORD __stdcall ReadSerialThrd(LPVOID pParam)
{
	CMainFrame* pMainFrame = NULL;

	if (NULL != pParam)
	{
		pMainFrame = (CMainFrame*)pParam;
		while (TRUE == pMainFrame->m_bReadSerial && NULL != pMainFrame->m_hCom)
		{
			pMainFrame->ReadSerial();
			Sleep(2);
		}
	}

	return 0;
} 

DWORD __stdcall TransmitYmodemThrd(LPVOID pParam)
{
	CMainFrame* pMainFrame = NULL;

	if (NULL != pParam)
	{
		pMainFrame = (CMainFrame*)pParam;

		pMainFrame->SendYmodem();

	}

	return 0;
}

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_hCom = NULL;
	m_bReadSerial = FALSE;
	m_hReadSerial = NULL;
	m_bUpgrade = FALSE;

	memset(&m_osRead, 0, sizeof(OVERLAPPED));
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	m_osRead.hEvent = NULL;
	m_pView = NULL;
}

CMainFrame::~CMainFrame()
{
	if (NULL != m_hCom)
	{
		CloseHandle(m_hCom);
		m_hCom = NULL;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	HICON hicon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	SetIcon(hicon,true);
	SetIcon(hicon,false);
	
	SetTimer(1, 100, NULL);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style = WS_OVERLAPPEDWINDOW | WS_MAXIMIZE;
	
	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

void CMainFrame::OpenSerial()
{
	CString strPostName;
	if (NULL != m_hCom)
	{
		CloseHandle(m_hCom);
		m_hCom = NULL;
		Sleep(10);
	}

	if (m_strPostName.GetLength() > 4)
	{
		strPostName = _T("\\\\.\\") ;
		strPostName += m_strPostName;
	}
	else
	{
		strPostName = m_strPostName;
	}

	m_hCom = CreateFile(strPostName, 
		GENERIC_READ|GENERIC_WRITE, 
		0, 
		NULL,
		OPEN_EXISTING, 
		FILE_FLAG_OVERLAPPED, 
		NULL);
	if((HANDLE)-1 == m_hCom)
	{
		AfxMessageBox(_T("Open Serial Fail!"));
		return;
	}
	SetWindowText(_T("Rangekit-") + m_strPostName);
	SetupComm(m_hCom, SERIAL_BUF_SIZE, SERIAL_BUF_SIZE);

	COMMTIMEOUTS TimeOuts;

	TimeOuts.ReadIntervalTimeout=MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier=0;
	TimeOuts.ReadTotalTimeoutConstant=0;

	TimeOuts.WriteTotalTimeoutMultiplier=100;
	TimeOuts.WriteTotalTimeoutConstant=500;
	SetCommTimeouts(m_hCom,&TimeOuts); 

	DCB dcb;
	GetCommState(m_hCom,&dcb);
	dcb.BaudRate= m_nBoudRate; 
	dcb.ByteSize=8; 
	dcb.Parity=NOPARITY; 
	dcb.StopBits = m_nStopBits;
	SetCommState(m_hCom,&dcb);

	PurgeComm(m_hCom,PURGE_TXCLEAR|PURGE_RXCLEAR);

	if (m_osRead.hEvent != NULL)
	{
		ResetEvent(m_osRead.hEvent);
	}

	m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

void CMainFrame::OpenSerial(UINT bondrate)
{
	CString strPostName;
	if (NULL != m_hCom)
	{
		CloseHandle(m_hCom);
		m_hCom = NULL;
		Sleep(10);
	}

	if (m_strPostName.GetLength() > 4)
	{
		strPostName = _T("\\\\.\\");
		strPostName += m_strPostName;
	}
	else
	{
		strPostName = m_strPostName;
	}

	m_hCom = CreateFile(strPostName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		NULL);
	if ((HANDLE)-1 == m_hCom)
	{
		AfxMessageBox(_T("Open Serial Fail!"));
		return;
	}
	SetWindowText(_T("Rangekit-") + m_strPostName);
	SetupComm(m_hCom, SERIAL_BUF_SIZE, SERIAL_BUF_SIZE);

	COMMTIMEOUTS TimeOuts;

	TimeOuts.ReadIntervalTimeout = MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;

	TimeOuts.WriteTotalTimeoutMultiplier = 100;
	TimeOuts.WriteTotalTimeoutConstant = 500;
	SetCommTimeouts(m_hCom, &TimeOuts);

	DCB dcb;
	GetCommState(m_hCom, &dcb);
	dcb.BaudRate = bondrate;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = m_nStopBits;
	SetCommState(m_hCom, &dcb);

	PurgeComm(m_hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);

	if (m_osRead.hEvent != NULL)
	{
		ResetEvent(m_osRead.hEvent);
	}

	m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

void CMainFrame::WriteSerial(const CString& strCmd)
{
	OVERLAPPED m_osWrite;
	memset(&m_osWrite,0,sizeof(OVERLAPPED));
	m_osWrite.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	char lpOutBuffer[MAX_PATH] = {0};
	
	DWORD dwBytesWrite = theApp.CStringToChar(strCmd, lpOutBuffer, MAX_PATH);
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;
	ClearCommError(m_hCom, &dwErrorFlags, &ComStat);
	bWriteStat=WriteFile(m_hCom,lpOutBuffer,
		dwBytesWrite,&dwBytesWrite,&m_osWrite);

	if(!bWriteStat)
	{
		if(GetLastError()==ERROR_IO_PENDING)
		{
			WaitForSingleObject(m_osWrite.hEvent,1000);
		}
	}
	CloseHandle(m_osWrite.hEvent);

	return;
}

void CMainFrame::ReadSerial()
{
	COMSTAT ComStat;
	DWORD dwErrorFlags;

	static unsigned char szReadBuffer[SERIAL_BUF_SIZE];
	memset(szReadBuffer, '\0', SERIAL_BUF_SIZE);
	DWORD dwBytesRead = SERIAL_BUF_SIZE;
	BOOL bReadStat;

	ClearCommError(m_hCom,&dwErrorFlags,&ComStat);
	dwBytesRead=min(dwBytesRead, (DWORD)ComStat.cbInQue);
	bReadStat=ReadFile(m_hCom, szReadBuffer,
		dwBytesRead, &dwBytesRead, &m_osRead);
	if(FALSE == bReadStat)
	{
		if(GetLastError()==ERROR_IO_PENDING)
		{
			WaitForSingleObject(m_osRead.hEvent,2000);
		}
		return;
	}

	//PurgeComm(m_hCom, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	PurgeComm(m_hCom, PURGE_RXABORT);
	
	if (0 < dwBytesRead)
	{
		//szReadBuffer[dwBytesRead] = '\0';
		
		if (NULL == m_pView)
		{
			m_pView = GetActiveView();
		}
		
		if (NULL != m_pView)
		{
			m_pView->SendMessage(WM_ADD_TEXT, (WPARAM)szReadBuffer, (LPARAM)dwBytesRead);
		}

		if (TRUE == m_bUpgrade)
		{
			CString strUpgrade(szReadBuffer);
			if (-1 != strUpgrade.Find(_T("C")))
			{
				m_bReadSerial = FALSE;
				PostMessage(WM_TRANSMIT_YMODEM);
			}
		}
	}
}

void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	OnConnectClose();
	CFrameWndEx::OnClose();
}


void CMainFrame::OnConnectOpen()
{

	if (IDOK != m_dlgSerialSet.DoModal())
	{
		//PostMessage(WM_CLOSE);
	}
	else
	{
		OnConnectClose();

		m_strPostName = m_dlgSerialSet.m_strPortName;
		m_nBoudRate = m_dlgSerialSet.m_nBoudRate;
		m_nStopBits = m_dlgSerialSet.m_nStopBits;
		OpenSerial();
		m_bReadSerial = TRUE;
		m_hReadSerial = CreateThread(NULL, 0, ReadSerialThrd, this, 0, NULL);
		WriteSerial(_T("\r\n"));
	}

}


void CMainFrame::OnConnectClose()
{
	if (NULL != m_hReadSerial)
	{
		m_bReadSerial = FALSE;
		WaitForSingleObject(m_hReadSerial, INFINITE);
		CloseHandle(m_hReadSerial);
		CloseHandle(m_osRead.hEvent);
		m_hReadSerial = NULL;
		theApp.InitLogPath();
	}
}


void CMainFrame::OnViewOperatormode()
{
	WriteSerial(_T("Get operator_mode\r\n"));
}


void CMainFrame::OnViewAnchorlist()
{
	WriteSerial(_T("AnchorListShow\r\n"));
	/*WriteSerial(_T("AnchorListSet 0 133A\r\n"));
	Sleep(100);
	WriteSerial(_T("AnchorListSet 1 133B\r\n"));
	Sleep(100);
	WriteSerial(_T("AnchorListSet 2 133C\r\n"));*/
}


void CMainFrame::OnViewMac()
{
	WriteSerial(_T("Get mac\r\n"));
}


void CMainFrame::OnViewRfmode()
{
	WriteSerial(_T("Get rf_mode\r\n"));
}

LRESULT CMainFrame::OnWriteSerial(WPARAM wParam, LPARAM lParam)
{
	char szCmd = (char)wParam;

	if (VK_RETURN == szCmd)
	{
		m_strCmd += _T("\r\n");
		WriteSerial(m_strCmd);
		m_strCmd.Empty();
	}
	else if (VK_BACK == szCmd)
	{
		if (!m_strCmd.IsEmpty())
		{
			m_strCmd = m_strCmd.Left(m_strCmd.GetLength() - 1);
		}	
	}
	else
	{
		m_strCmd += szCmd;
	}

	return 0;
}

void CMainFrame::SendYmodem(void)
{
	CYmodem ymodem(this, m_hCom);
	char szFileName[MAX_PATH] = { 0 };
	theApp.CStringToChar(m_strUpFileName, szFileName, MAX_PATH);
	unsigned char transmited = 0xFF;

	try 
	{
		CFile file(m_strUpgradeFile, CFile::modeRead);
		ULONGLONG FileSize = file.GetLength();
		BYTE* pData = new BYTE[FileSize];
		file.Read(pData, FileSize);
		file.Close();

		transmited = ymodem.Transmit(pData, (unsigned char*)szFileName, FileSize);
		m_dlgUpgrade.PostMessage(WM_CLOSE);

		delete [] pData;
		if (0 == transmited)
		{
			//MessageBox(_T("Upgrade Success!"));
			m_bReadSerial = TRUE;
			m_hReadSerial = CreateThread(NULL, 0, ReadSerialThrd, this, 0, NULL);
		}
		else
		{
			MessageBox(_T("Upgrade Failed!"));
		}
		m_bUpgrade = FALSE;
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}

	
	return;
}

LRESULT CMainFrame::OnTransmitYmodem(WPARAM wParam, LPARAM lParam)
{
	WaitForSingleObject(m_hReadSerial, INFINITE);
	CloseHandle(m_hReadSerial);
	m_hReadSerial = NULL;

	CreateThread(NULL, 0, TransmitYmodemThrd, this, 0, NULL);

	m_dlgUpgrade.DoModal();

	return 0;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (1 == nIDEvent)
	{
		KillTimer(1);
		OnConnectOpen();
	}
	
	CFrameWndEx::OnTimer(nIDEvent);
}


void CMainFrame::OnSettingMode()
{
	CString strCmd;
	
	if (IDOK == m_dlgSetOperaMode.DoModal())
	{
		strCmd.Format(_T("Set operator_mode %u\r\n"), 
			m_dlgSetOperaMode.m_uOperaMode);
		WriteSerial(strCmd);
	}
}


void CMainFrame::OnSettingAnchor()
{
	CString strCmd;

	if (IDOK == m_dlgAnchorListSet.DoModal())
	{
		strCmd.Format(_T("AnchorListSet %u %s\r\n"), 
			m_dlgAnchorListSet.m_uAnchorId, 
			m_dlgAnchorListSet.m_strAnchorMac);
		WriteSerial(strCmd);
	}
}


void CMainFrame::OnSettingRfmode()
{
	CString strCmd;
	
	if (IDOK == m_dlgSetRfMode.DoModal())
	{
		strCmd.Format(_T("Set rf_mode %u\r\n"), m_dlgSetRfMode.m_uRfMode);
		WriteSerial(strCmd);
	}
}


void CMainFrame::OnSettingReboot()
{
	WriteSerial(_T("Reboot\r\n"));
}


void CMainFrame::OnSettingReset()
{
	WriteSerial(_T("Reset\r\n"));
}


void CMainFrame::OnHelpClear()
{
	CView* pView = GetActiveView();
	if (NULL != pView)
	{
		((CEditView*)pView)->GetEditCtrl().SetSel(0, -1);
		((CEditView*)pView)->GetEditCtrl().Clear();	
	}
}


void CMainFrame::OnViewSleeptime()
{
	WriteSerial(_T("Get sleep_time\r\n"));
}


void CMainFrame::OnSettingSleeptime()
{
	CDlgSetSleepTime dlg;
	CString strCmd;

	if (IDOK == dlg.DoModal())
	{
		strCmd.Format(_T("Set sleep_time %u\r\n"), dlg.m_uSleepTime);
		WriteSerial(strCmd);
	}
}


void CMainFrame::OnSettingCustom()
{
	CDlgSetCustom dlg;

	if (IDOK == dlg.DoModal())
	{
		WriteSerial(dlg.m_strCommand + _T("\r\n"));
	}
}


void CMainFrame::OnSettingCoordinate()
{
	CString strCmd;
	CDlgSetCoordinate dlg;

	if (IDOK == dlg.DoModal())
	{
		strCmd.Format(_T("CoordinateSet %.2f %.2f\r\n"), dlg.m_dCoorX, dlg.m_dCoorY);
		WriteSerial(strCmd);
	}
}


void CMainFrame::OnViewCoordinate()
{
	WriteSerial(_T("CoordinateShow\r\n"));
}


void CMainFrame::OnViewAntennadelay()
{
	WriteSerial(_T("Get antenna_delay\r\n"));
}


void CMainFrame::OnSettingAntennadelay()
{
	CString strCmd;
	CDlgSetAntennaDelay dlg;

	if (IDOK == dlg.DoModal())
	{
		strCmd.Format(_T("Set antenna_delay %u\r\n"), dlg.m_uAntennaDelay);
		WriteSerial(strCmd);
	}
}


void CMainFrame::OnTransferSendymodem()
{
	BOOL isOpen = TRUE;                
	CString defaultDir = _T("E:\\");  
	CString fileName;                  
	CString filter = _T("File (*.bin)|*.bin||");
	TCHAR path[MAX_PATH] = { 0 };

	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);

	if (m_strUpgradeFile.IsEmpty())
	{
		GetCurrentDirectory(MAX_PATH, path);
		openFileDlg.GetOFN().lpstrInitialDir = path;
	}
	else
	{
		openFileDlg.GetOFN().lpstrInitialDir = m_strUpgradeFile;
	}
	
	if (IDOK == openFileDlg.DoModal())
	{
		m_strUpgradeFile = openFileDlg.GetPathName();
		m_strUpFileName = openFileDlg.GetFileName();
		WriteSerial(_T("Upgrade\r\n"));
		m_bUpgrade = TRUE;
	}

}


void CMainFrame::OnHelpLogfolder()
{
	theApp.OpenLogFolder();
}


void CMainFrame::OnTestPower()
{
	WriteSerial(_T("PowerTest\r\n"));
}



void CMainFrame::OnUpdateTestPower(CCmdUI *pCmdUI)
{
	//pCmdUI->SetCheck(m_powertest);
}


void CMainFrame::OnSettingAjustlist()
{
	CString strCmd;

	if (IDOK == m_dlgSetAust.DoModal())
	{
		strCmd.Format(_T("AnchorListSet %u %s\r\n"), 
			m_dlgAnchorListSet.m_uAnchorId, 
			m_dlgAnchorListSet.m_strAnchorMac);
		WriteSerial(strCmd);
	}
}
