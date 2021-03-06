
// InvokeProcessDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "InvokeProcess.h"
#include "InvokeProcessDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CInvokeProcessDlg 对话框



CInvokeProcessDlg::CInvokeProcessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INVOKEPROCESS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInvokeProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInvokeProcessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CInvokeProcessDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CInvokeProcessDlg 消息处理程序

BOOL CInvokeProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	//WinExec(_T("SnippingTool.exe"), SW_SHOW);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CInvokeProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CInvokeProcessDlg::OnPaint()
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
HCURSOR CInvokeProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CInvokeProcessDlg::OnBnClickedOk()
{
	CWnd* pWnd = FindWindow(NULL, _T("截图工具"));
	int closeCount = 11;
	while (NULL != pWnd && 0 < closeCount--)
	{
		pWnd->SendMessage(WM_CLOSE);
		Sleep(10);
		pWnd = FindWindow(NULL, _T("截图工具"));
		
	}

	ShellExecute(NULL, _T("open"), _T("SnippingTool.exe"), NULL, NULL, SW_SHOW);
	int waitCount = 11;
	do
	{
		pWnd = FindWindow(NULL, _T("截图工具"));
		if (NULL == pWnd)
		{
			Sleep(10);
		}
	} while (NULL == pWnd && 0 < waitCount--);

	if (NULL != pWnd)
	{
		/*HWND hActive = ::GetForegroundWindow();
		pWnd->SetForegroundWindow();
		keybd_event(VK_CONTROL, 0, 0, 0);
		keybd_event('N', 0, 0, 0);
		Sleep(100);
		keybd_event('N', 0, KEYEVENTF_KEYUP, 0);
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
		::SetForegroundWindow(hActive);*/
		pWnd->PostMessage(WM_CHAR, 0x0E, 0);  //发送Ctrl+N
	}

	//CDialogEx::OnOK();
}
