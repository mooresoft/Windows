
// RangekitView.cpp : implementation of the CRangekitView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Rangekit.h"
#endif

#include "RangekitDoc.h"
#include "RangekitView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const CString g_strVisibal = _T("!@#$%^&*(),.<>;:[]{}|? _+-=");

// CRangekitView

IMPLEMENT_DYNCREATE(CRangekitView, CEditView)

BEGIN_MESSAGE_MAP(CRangekitView, CEditView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CRangekitView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_MESSAGE(WM_ADD_TEXT, OnAddText)
	ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CREATE()
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CRangekitView construction/destruction

CRangekitView::CRangekitView()
{
	// TODO: add construction code here
	m_clrText = RGB(210, 210, 210); 
	m_clrTxtBkgnd = RGB(0, 0, 0); 
	m_brBkgnd.CreateSolidBrush(m_clrTxtBkgnd); 
	m_hFont = CreateFont(16,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,
		_T("Courier New"));
}

CRangekitView::~CRangekitView()
{
}

BOOL CRangekitView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}


// CRangekitView printing


void CRangekitView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CRangekitView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CRangekitView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CRangekitView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

void CRangekitView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	return;
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRangekitView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CRangekitView diagnostics

#ifdef _DEBUG
void CRangekitView::AssertValid() const
{
	CEditView::AssertValid();
}

void CRangekitView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CRangekitDoc* CRangekitView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRangekitDoc)));
	return (CRangekitDoc*)m_pDocument;
}
#endif //_DEBUG


LRESULT CRangekitView::OnAddText(WPARAM wParam, LPARAM lParam)
{
	static CString strText;
	static char szText[SERIAL_BUF_SIZE + 1];
	char* pText = (char*)wParam;
	DWORD txtlen = (DWORD)lParam;
	DWORD j = 0;

	memset(szText, '\0', SERIAL_BUF_SIZE + 1);
	for (DWORD i = 0; i < txtlen; i++)
	{
		if ('\0' != pText[i])
		{
			szText[j++] = pText[i];
		}
	}
	
	GetEditCtrl().SetSel(-1);

	strText = szText;
	
	GetEditCtrl().ReplaceSel(strText);
	
	UINT uBufLen = GetBufferLength();
	
	if (m_uTxtMaxlen / 20 < uBufLen)
	{
		GetEditCtrl().SetSel(0, uBufLen / 2);
		GetEditCtrl().Clear();
	}
	theApp.writelog(szText);
	return 0;
}



void CRangekitView::OnEnChange()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEditView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetEditCtrl().SetModify(FALSE);
}


void CRangekitView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//return;
	CEditView::OnLButtonDown(nFlags, point);
}


void CRangekitView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//return;
	CEditView::OnLButtonUp(nFlags, point);
}


void CRangekitView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//return;
	CEditView::OnLButtonDblClk(nFlags, point);
}


HBRUSH CRangekitView::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC->SetTextColor(m_clrText);
	pDC->SetBkColor(m_clrTxtBkgnd);
	SelectObject(pDC->m_hDC, m_hFont);
	return m_brBkgnd;
}


BOOL CRangekitView::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		if(VK_PRIOR <= pMsg->wParam && VK_DOWN >= pMsg->wParam)
		{
			return TRUE;
		}
	}

	return CEditView::PreTranslateMessage(pMsg);
}


int CRangekitView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_uTxtMaxlen = GetEditCtrl().GetLimitText();
	return 0;
}

void CRangekitView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	char szWord;
	szWord = nChar;

	if (isVisibleChar(szWord))
	{
		CWnd* pMain = AfxGetMainWnd();
		if (NULL != pMain)
		{
			pMain->PostMessage(WM_WRITE_SERAIL, (WPARAM)szWord, NULL);
		}
	}

	CEditView::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CRangekitView::isVisibleChar(const char szWord)
{
	BOOL bVisible = FALSE;

	if (VK_RETURN == szWord || VK_BACK == szWord 
		|| 'A' <= szWord && 'Z'  >= szWord
		|| 'a' <= szWord && 'z'  >= szWord
		|| '0' <= szWord && '9'  >= szWord
		|| '_' == szWord || ' ' == szWord)
	{
		bVisible = TRUE;
	}
	else
	{
		for (int i = 0; i < g_strVisibal.GetLength(); i++)
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
