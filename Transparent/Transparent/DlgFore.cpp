// DlgFore.cpp : implementation file
//

#include "stdafx.h"
#include "Transparent.h"
#include "DlgFore.h"
#include "afxdialogex.h"


// CDlgFore dialog

IMPLEMENT_DYNAMIC(CDlgFore, CDialogEx)

CDlgFore::CDlgFore(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgFore::IDD, pParent)
{
	m_textColor = RGB(255, 255, 255);
	m_textBkColor = RGB(0, 0, 0);
	m_bTracked	= FALSE;
}

CDlgFore::~CDlgFore()
{
}

void CDlgFore::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgFore, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()


// CDlgFore message handlers


BOOL CDlgFore::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd,GWL_EXSTYLE) | 0x80000);
	HMODULE  hInst = LoadLibrary(_T("User32.DLL"));    
	if    (hInst != NULL)   
	{   
		typedef BOOL (WINAPI *MYFUNC)(HWND, COLORREF, BYTE, DWORD);   

		MYFUNC pFunc = (MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");   
		if (pFunc != NULL)   
		{   
			//pFunc(m_hWnd, 0, 0, LWA_ALPHA);		//全透明
			//pFunc(m_hWnd, 0, 255, LWA_ALPHA);		//不透明
			//pFunc(m_hWnd, 0, 100, LWA_ALPHA);		//半透明
			pFunc(m_hWnd,RGB(0,255,0),0,LWA_COLORKEY);

		}   
		FreeLibrary(hInst);   
		hInst = NULL;   
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgFore::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	CRect rc;

	GetClientRect(&rc);

	dc.FillSolidRect(rc,RGB(0,255,0));
	//dc.SetBkMode(TRANSPARENT);
	dc.SetBkColor(m_textBkColor);
	dc.SetTextColor(m_textColor);
	dc.TextOut(0,0,_T("afdsafdafds"));
}

void CDlgFore::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialogEx::PostNcDestroy();
	delete this;
}


void CDlgFore::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnClose();
	DestroyWindow();
}


void CDlgFore::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (FALSE == m_bTracked)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = HOVER_DEFAULT;
		m_bTracked=::_TrackMouseEvent(&tme); 

		m_textColor = RGB(255, 255, 0);
		Invalidate();
		TRACE("----OnMouseMove\r\n");
	}
	
	CDialogEx::OnMouseMove(nFlags, point);
}


void CDlgFore::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default
	if (TRUE == m_bTracked)
	{
		m_bTracked = FALSE;
		m_textColor = RGB(0, 255, 255);
		Invalidate();
		TRACE("----OnMouseLeave\r\n");
	}	

	CDialogEx::OnMouseLeave();
}

void CDlgFore::SetTextBkColor(const COLORREF& clr)
{
	m_textBkColor = clr;
}
