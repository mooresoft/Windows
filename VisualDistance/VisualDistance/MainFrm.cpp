
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "VisualDistance.h"
#include "MainFrm.h"
//#include "VisualDistanceView.h"
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
const unsigned char g_strVisibal[] = ",:";

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
	ON_COMMAND(ID_SERIAL_CLOSE, &CMainFrame::OnSerialClose)
	ON_MESSAGE(WM_COMM_RXCHAR, OnRecvSerial)
	ON_MESSAGE(WM_UPDATE_INDICATOR, OnUpdateIndicator)
	ON_WM_CLOSE()
	ON_COMMAND(ID_EDIT_RESET, &CMainFrame::OnEditReset)
	ON_COMMAND(ID_EDIT_AJUST, &CMainFrame::OnEditAjust)
	ON_COMMAND(ID_EDIT_AJUSTALL, &CMainFrame::OnEditAjustall)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
	ID_INDICATOR_COUNT,
	ID_INDICATOR_TIME,
};

// CMainFrame construction/destruction

DWORD __stdcall ReadThrd(LPVOID pParam)
{
	CMainFrame* pMainFrame = NULL;
	
	if (NULL != pParam)
	{
		pMainFrame = (CMainFrame*)pParam;
		while (TRUE == pMainFrame->m_bRead)
		{			
			pMainFrame->FillReport();
			Sleep(1);
		}
	}

	return 0;
}

CMainFrame::CMainFrame()
{
	m_bRead = FALSE;
	m_hReadThrd = NULL;
	InitializeCriticalSectionAndSpinCount(&m_csDistance, 400);
	InitializeCriticalSectionAndSpinCount(&m_csRecvSerail, 400);
}

CMainFrame::~CMainFrame()
{
	if (m_file.is_open())
	{
		m_file.close();
	}

	DeleteCriticalSection(&m_csDistance); 
	DeleteCriticalSection(&m_csRecvSerail);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	if (false == m_file.is_open())
	{
		m_file.open("E:\\woxumain.log", ios::out | ios::binary);
	}

	SetWindowText(_T("Visual Distance"));
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
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
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers
const char distance_table[] = "create table IF NOT EXISTS distances(id int(4) unsigned primary key not null auto_increment,\
							 seq int(4) unsigned not null,tmac varchar(4) not null,\
                             amac varchar(4) not null,distance int(2) unsigned not null);";

void CMainFrame::OnFileOpen()
{
	if (IDOK == m_dlgOpenSerial.DoModal())
	{
		CString strWindowText;
		OnSerialClose();
		Sleep(10);
		if (TRUE == m_serial.InitPort(this, m_dlgOpenSerial.m_nPort, m_dlgOpenSerial.m_nBoudRate))
		{
			m_bRead		= TRUE;
			m_hReadThrd	= CreateThread(NULL, 0, ReadThrd, this, 0, NULL);
			theApp.m_pMainFrame = this;
			m_serial.StartMonitor();
			m_StartTime = CTime::GetCurrentTime();

			strWindowText.Format(_T("Visual Distance - COM%d"), m_dlgOpenSerial.m_nPort);
			SetWindowText(strWindowText);
		}
		else
		{
			MessageBox(_T("Open Serial Port Fail!"));
		}
	}
}


void CMainFrame::OnSerialClose()
{
	m_serial.UninitPort();
	m_bRead = FALSE;
	if (NULL != m_hReadThrd)
	{
		WaitForSingleObject(m_hReadThrd, INFINITE);
		CloseHandle(m_hReadThrd);
		m_hReadThrd = NULL;
	}
	return;
}

bool substr(const string& str, const string& strs, const string& stre, string& values)
{
	bool bRet = true;
	stringstream ss;
	string strleft;
	size_t offset = 0;
	size_t starts;
	size_t ends;

	for (size_t i = 0; i < strs.length(); i++)
	{
		starts = str.find_first_of(strs[i], offset);
		if (string::npos == starts)
		{
			return false;
		}
		offset = starts;
	}

	starts++;
	offset = starts;

	for (size_t i = 0; i < stre.length(); i++)
	{
		ends = str.find_first_of(stre[i], offset);
		if (string::npos == ends)
		{
			return false;
		}
		offset = ends;
	}

	strleft = str.substr(starts, ends - starts);
	ss << strleft;
	ss >> values;

	if (ss.fail())
	{
		bRet = false;
	}

	return bRet;
}

bool substr(const string& str, const string& strs, const string& stre, unsigned long& values)
{
	bool bRet = true;
	stringstream ss;
	string strValues;

	if (false == substr(str, strs, stre, strValues))
	{
		return false;
	}
		
	ss << strValues;
	ss >> values;

	if (ss.fail())
	{
		bRet = false;
	}

	return bRet;
}

bool substr(const string& str, const string& strs, const string& stre, unsigned short& values)
{
	bool bRet = true;
	stringstream ss;
	string strValues;

	if (false == substr(str, strs, stre, strValues))
	{
		return false;
	}

	ss << strValues;
	ss >> values;

	if (ss.fail())
	{
		bRet = false;
	}

	return bRet;
}

DISTANCE_LIST&  CMainFrame::GetDistances()
{
	EnterCriticalSection(&m_csDistance);
	return m_listDistance;
}

void CMainFrame::ReleaseDistances(void)
{
	LeaveCriticalSection(&m_csDistance);
	return;
}

BOOL CMainFrame::isVisibleChar(const char szWord)
{
	BOOL bVisible = FALSE;

	if ('A' <= szWord && 'Z' >= szWord
		|| 'a' <= szWord && 'z' >= szWord
		|| '0' <= szWord && '9' >= szWord)
	{
		bVisible = TRUE;
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			if (szWord == g_strVisibal[i])
			{
				bVisible = TRUE;
				break;
			}
		}
	}

	return bVisible;
}

void CMainFrame::FillReport(void)
{
	BOOL bFind;
	string str;
	unsigned short seq;
	string tmac;
	string amac;
	unsigned long distance;
	DISTANCE_LIST::iterator i;
	DISTANCE_NODE dis_node;
	unsigned long offset;

	if (false == m_qsDistance.empty())
	{
		EnterCriticalSection(&m_csRecvSerail);
		str = m_qsDistance.front();
		m_qsDistance.pop();
		LeaveCriticalSection(&m_csRecvSerail);
		if (true == substr(str, "S:", ",", seq)
			&& true == substr(str, "T:", ",", tmac)
			&& true == substr(str, "A:", ",", amac)
			&& true == substr(str, "R:", "cm", distance))
		{
			bFind = FALSE;
			EnterCriticalSection(&m_csDistance);
			for (i = m_listDistance.begin(); i != m_listDistance.end(); i++)
			{
				if (0 == memcmp((*i).tmac, tmac.c_str(), 4)
					&& 0 == memcmp((*i).amac, amac.c_str(), 4))
				{
					bFind = TRUE;

					if (seq > (*i).seq)
					{
						(*i).index += (seq - (*i).seq);
					}
					else
					{
						(*i).index++;
					}

					(*i).seq = seq;

					if ((*i).averages > distance)
					{
						offset = (*i).averages - distance;
					}
					else
					{
						offset = distance - (*i).averages;
					}

					if (offset < 100)
					{
						(*i).averages = ((*i).averages + distance) / 2;
						if ((*i).maxv < distance)
						{
							(*i).maxv = distance;
						}
						if ((*i).minv > distance)
						{
							(*i).minv = distance;
						}

						while ((*i).distances.size() > 1000)
						{
							(*i).distances.pop_front();
						}
						(*i).distances.push_back(distance);
					}
					else
					{
						(*i).terribles++;
					}
					
					(*i).counts++;

					break;
				}
			}

			if (FALSE == bFind)
			{
				dis_node.index = 1;
				dis_node.seq = seq;
				dis_node.averages = distance;
				dis_node.counts = 1;
				dis_node.maxv = distance;
				dis_node.minv = distance;
				memset(dis_node.tmac, 0, 5);
				memcpy(dis_node.tmac, tmac.c_str(), 4);
				memset(dis_node.amac, 0, 5);
				memcpy(dis_node.amac, amac.c_str(), 4);
				dis_node.terribles = 0;

				dis_node.distances.push_back(distance);

				m_listDistance.push_back(dis_node);
			}
			LeaveCriticalSection(&m_csDistance);
		}
	}	
	
	return;
}

LRESULT CMainFrame::OnRecvSerial(WPARAM wParam, LPARAM lParam)
{
	char RecvChar = (char)wParam;

	if (TRUE == isVisibleChar(RecvChar))
	{
		//m_file << RecvChar;
		//m_file.flush();
		m_strDistance += RecvChar;

		if (true == m_file.is_open())
		{
			m_file << RecvChar;
			m_file.flush();
		}
	}
	else if('\r' == RecvChar || '\n' == RecvChar)
	{
		EnterCriticalSection(&m_csRecvSerail);
		m_qsDistance.push(m_strDistance);
		LeaveCriticalSection(&m_csRecvSerail);
		m_strDistance.clear();
		if (true == m_file.is_open())
		{
			m_file << RecvChar;
			m_file.flush();
		}
	}

	return 0;
}

void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	OnSerialClose();
	CFrameWnd::OnClose();
}

LRESULT CMainFrame::OnUpdateIndicator(WPARAM wParam, LPARAM lParam)
{
	CString strCount;
	DISTANCE_LIST& dis_rep = GetDistances();
	strCount.Format(_T(" %d "), dis_rep.size());
	ReleaseDistances();
	m_wndStatusBar.SetPaneText(1, strCount);

	CTimeSpan timespan = CTime::GetCurrentTime() - m_StartTime;
	CString strTime;
	if (timespan.GetDays() > 0)
	{
		strTime = timespan.Format(_T("%D Day %H:%M:%S"));
	}
	else
	{
		strTime = timespan.Format(_T("      %H:%M:%S"));
	}

	m_wndStatusBar.SetPaneText(2, strTime);

	return 0;
}

void CMainFrame::OnEditReset()
{
	EnterCriticalSection(&m_csDistance);
	m_listDistance.clear();
	LeaveCriticalSection(&m_csDistance);
}


void CMainFrame::OnEditAjust()
{
	CString strCmd;
	if (IDOK == m_dlgAjust.DoModal())
	{
		strCmd.Format(_T("AjustSet %d %d\r\n"), m_dlgAjust.m_uStepId - 1, m_dlgAjust.m_nAjust);
		m_serial.WriteSerial(strCmd);
		OnEditReset();
	}
}


void CMainFrame::OnEditAjustall()
{
	
}
