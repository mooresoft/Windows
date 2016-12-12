
// MsActiveMQDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MsActiveMQ.h"
#include "MsActiveMQDlg.h"
#include "afxdialogex.h"
#include <conio.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMsActiveMQDlg �Ի���




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


// CMsActiveMQDlg ��Ϣ�������

BOOL CMsActiveMQDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMsActiveMQDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
