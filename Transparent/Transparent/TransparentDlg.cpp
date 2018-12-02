
// TransparentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Transparent.h"
#include "TransparentDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CTransparentDlg dialog




CTransparentDlg::CTransparentDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTransparentDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTransparentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTransparentDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CTransparentDlg message handlers

BOOL CTransparentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd,GWL_EXSTYLE) | 0x80000);
	HMODULE  hInst = LoadLibrary(_T("User32.DLL"));    
	if    (hInst != NULL)   
	{   
		typedef BOOL (WINAPI *MYFUNC)(HWND, COLORREF, BYTE, DWORD);   

		MYFUNC pFunc = (MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");   
		if (pFunc != NULL)   
		{   
			//pFunc(m_hWnd, 0, 0, LWA_ALPHA);		//ȫ͸��
			//pFunc(m_hWnd, 0, 255, LWA_ALPHA);		//��͸��
			//pFunc(m_hWnd, 0, 100, LWA_ALPHA);		//��͸��
			pFunc(m_hWnd,RGB(0,255,0),0,LWA_COLORKEY);
			
		}   
		FreeLibrary(hInst);   
		hInst = NULL;   
	}

	/*
	���躯��ԭ�ͣ�

	��������BOOL WINAPI SetLayeredWindowAttributes(HWND hWnd,  COLORREFcrKey,  BYTE bAlpha,  DWORD flag);
		�������壺
		hWnd: ��Ҫ���õĴ��ھ����
		flag:    ͸����ʽ����ȡ����ֵ��LWA_COLORKEY����ֵ����Ϊ0x1����LWA_ALPHA����ֵ����Ϊ0x2������ȡ���ߵ���ϡ�
		crKey: ������ɫ����flagȡֵΪLWA_COLORKEYʱ����������ɫֵ����crKey�ĵط�����Ϊȫ͸��������˲�����Ч��
		bAlpha�����ڵ�͸���ȣ�ȡֵ��Χ0��255����flag����LWA_ALPHA��ʶʱ��Ч��

		��MFC�еĵ��÷�����SetLayeredWindowAttributes(m_hWnd, RGB(0, 0, 0), 200, 0x1 | 0x2);
	*/

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTransparentDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTransparentDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // device context for painting
		CRect rc;

		GetClientRect(&rc);

		dc.FillSolidRect(rc,RGB(0,255,0));
		dc.SetBkMode(TRANSPARENT);
		dc.TextOut(0,0,_T("afdsafdafds"));
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTransparentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

