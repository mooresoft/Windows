
// VlcDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VlcDemo.h"
#include "VlcDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CVlcDemoDlg �Ի���



CVlcDemoDlg::CVlcDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VLCDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_vlcMedia = NULL;
	m_vlcMplay = NULL;
	m_vlcInst = NULL;
}

void CVlcDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVlcDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_BTN_STOP, &CVlcDemoDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(ID_BTN_PLAY, &CVlcDemoDlg::OnBnClickedBtnPlay)
END_MESSAGE_MAP()


// CVlcDemoDlg ��Ϣ�������

BOOL CVlcDemoDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CVlcDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVlcDemoDlg::OnPaint()
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
HCURSOR CVlcDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVlcDemoDlg::OnBnClickedBtnStop()
{

	if (NULL != m_vlcMplay)
	{
		libvlc_media_player_stop(m_vlcMplay);
		libvlc_media_player_release(m_vlcMplay);
		m_vlcMplay = NULL;
	}
	if (NULL != m_vlcMedia)
	{
		libvlc_media_release(m_vlcMedia);
		m_vlcMedia = NULL;
	}
}


void CVlcDemoDlg::OnBnClickedBtnPlay()
{
	const char * const vlc_args[] = {
		"--demux=h264",
		"--ipv4",
		"--no-prefer-system-codecs",
		"--rtsp-caching=300",
		"--network-caching=500",
		"--rtsp-tcp",
		//"--verbose=2",
		//"--extraintf=logger"
	};

	if (NULL == m_vlcInst)
	{
		m_vlcInst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
	}
	if (NULL == m_vlcMedia)
	{
		//  "rtsp://127.0.0.1:1234/vedio"
		m_vlcMedia = libvlc_media_new_location(m_vlcInst, "rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov");
	}
	if (NULL == m_vlcMplay)
	{
		m_vlcMplay = libvlc_media_player_new_from_media(m_vlcMedia);
	}

	HWND hWndVedio = GetDlgItem(IDC_STATIC_VEDIO)->GetSafeHwnd();
	libvlc_media_player_set_hwnd(m_vlcMplay, hWndVedio);
	libvlc_media_player_play(m_vlcMplay);
}
