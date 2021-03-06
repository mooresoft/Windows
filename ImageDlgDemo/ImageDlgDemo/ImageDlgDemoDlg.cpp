
// ImageDlgDemoDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ImageDlgDemo.h"
#include "ImageDlgDemoDlg.h"
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


// CImageDlgDemoDlg 对话框



CImageDlgDemoDlg::CImageDlgDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGEDLGDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageDlgDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImageDlgDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CImageDlgDemoDlg 消息处理程序

BOOL CImageDlgDemoDlg::OnInitDialog()
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

	TCHAR buf[256] = { 0 };
	GetModuleFileName(NULL, buf, sizeof(buf));
	CString exename = buf;
	int pos = exename.ReverseFind('\\');
	CString exePath = exename.Left(pos);	
#ifdef _USE_GDIPLUS
	CString imagePath = exePath + _T("\\ImageDlg.jpg");
	m_img = Image::FromFile(imagePath);
#else
	CString imagePath = exePath + _T("\\Login.bmp");
	HBITMAP m_hBitmap;
	m_hBitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(), imagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	_bitmap.Attach(m_hBitmap);
	_bitmap.GetBitmap(&_bit);
	CDC* pDC = GetDC();
	_memdc.CreateCompatibleDC(pDC);
	_memdc.SelectObject(&_bitmap);
	ReleaseDC(pDC);
	CRect wndRect;
	GetClientRect(wndRect);
	MoveWindow(wndRect.left, wndRect.top, _bit.bmWidth, _bit.bmHeight);
	CenterWindow();
#endif
	SetTimer(1, 100, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CImageDlgDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CImageDlgDemoDlg::OnPaint()
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
		CPaintDC dc(this); // 用于绘制的设备上下文
		CRect rect = { 0 };
		GetClientRect(&rect);   //获取客户区大小
#ifdef _USE_GDIPLUS
		Graphics g(dc);
		g.DrawImage(m_img, 0, 0, rect.Width(), rect.Height());
#else		
		//用客户区的DC绘制所生成的BITMAP，并适应为窗口大小
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &_memdc, 0, 0, _bit.bmWidth, _bit.bmHeight, SRCCOPY);
		dc.SetBkMode(TRANSPARENT);
		CString str;
		SYSTEMTIME st;
		CString strDate, strTime;
		GetLocalTime(&st);
		strDate.Format(_T("%4d-%2d-%2d "), st.wYear, st.wMonth, st.wDay);
		strTime.Format(_T("%02d:%02d:%02d"), st.wHour, st.wMinute, st.wSecond);
		str = strDate + strTime;
		dc.TextOut(0, 0, str);
#endif
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CImageDlgDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CImageDlgDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (1 == nIDEvent)
	{
		Invalidate();
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CImageDlgDemoDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


HBRUSH CImageDlgDemoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC: //该方法在使用皮肤库中不生效
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 0, 0));
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	default:
		break;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
