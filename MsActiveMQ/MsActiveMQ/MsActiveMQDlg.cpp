
// MsActiveMQDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MsActiveMQ.h"
#include "MsActiveMQDlg.h"
#include "afxdialogex.h"
#include <conio.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMsActiveMQDlg 对话框




CMsActiveMQDlg::CMsActiveMQDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMsActiveMQDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hSendThread = NULL;
	m_bSend = FALSE;
}

void CMsActiveMQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMsActiveMQDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_PRODUCER, &CMsActiveMQDlg::OnBnClickedBtnProducer)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_CONSUMER, &CMsActiveMQDlg::OnBnClickedBtnConsumer)
END_MESSAGE_MAP()


// CMsActiveMQDlg 消息处理程序

BOOL CMsActiveMQDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMsActiveMQDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMsActiveMQDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMsActiveMQDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMsActiveMQDlg::OnBnClickedBtnProducer()
{
	/*
	long long startTime = System::currentTimeMillis();

	Thread producerThread(&theApp.m_producer);
	producerThread.start();

	// Wait for the threads to complete.
	producerThread.join();

	long long endTime = System::currentTimeMillis();
	double totalTime = (double)(endTime - startTime) / 1000.0;

	_cprintf("total time is %f\r\n", totalTime);
	//consumer.close();
	*/

	if (NULL == m_hSendThread)
	{
		DWORD dwThreadID = 0;
		m_bSend = TRUE;
		m_hSendThread = CreateThread(NULL,0,SendThread,this,0,&dwThreadID);
	}	
}

DWORD WINAPI CMsActiveMQDlg::SendThread( LPVOID lpParam )
{
	CMsActiveMQDlg* p = (CMsActiveMQDlg*)lpParam;

	if (NULL == p)
	{
		return 0;
	}

	DWORD dwResult = 0;
	static unsigned char send_buf[800] = {0};
	long starttick = GetTickCount();

#if 0
	while (p->m_bSend)
	{
		theApp.m_pProducer->send((unsigned char*)source_foo, strlen(source_foo));
		//Sleep(0);
	}
#else
	for (UINT i = 0; i < 20000; i++)
	{
		theApp.m_pProducer->send(send_buf, 800);
	}
#endif
	long endtick = GetTickCount();
	long ticks = endtick - starttick;

	TRACE("send time is %ld\n", ticks);

	return 0;
}

void CMsActiveMQDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bSend = FALSE;
	if (NULL != m_hSendThread)
	{
		WaitForSingleObject(m_hSendThread,INFINITE);
		m_hSendThread = NULL;
	}
	CDialogEx::OnClose();
}


void CMsActiveMQDlg::OnBnClickedBtnConsumer()
{
	m_bSend = FALSE;
	if (NULL != m_hSendThread)
	{
		WaitForSingleObject(m_hSendThread,INFINITE);
		m_hSendThread = NULL;
	}
}
