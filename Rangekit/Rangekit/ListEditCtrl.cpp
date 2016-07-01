// ListEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "rangekit.h"
#include "ListEditCtrl.h"


// CListEditCtrl

IMPLEMENT_DYNAMIC(CListEditCtrl, CListCtrl)

CListEditCtrl::CListEditCtrl()
{

}

CListEditCtrl::~CListEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CListEditCtrl, CListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CListEditCtrl message handlers




void CListEditCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_EditItem.m_hWnd != NULL)
	{
		m_EditItem.ShowWindow(0);
		if(m_Row != -1)
		{
			CString ItemText;
			m_EditItem.GetWindowText(ItemText);
			this->SetItemText(m_Row, m_Col, ItemText);
			::PostMessage(GetParent()->m_hWnd, MLSM_ITEMCHANGED, (WPARAM)MAKELONG(m_Row, m_Col), (LPARAM)this->m_hWnd);
		}
	}
	m_Col = m_Row = -1;

	CListCtrl::OnLButtonDown(nFlags, point);
}


void CListEditCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	LVHITTESTINFO hi;
	hi.pt = point;

	if(SubItemHitTest(&hi) != -1)
	{
		m_Row = hi.iItem;
		m_Col = hi.iSubItem;
		if(m_EditItem.m_hWnd == NULL)
		{
			RECT rect;
			rect.left = rect.top = 0;
			rect.bottom = 20;
			rect.right = 100;
			m_EditItem.Create(WS_CHILD | ES_LEFT | WS_BORDER | ES_AUTOHSCROLL | ES_WANTRETURN | ES_MULTILINE, rect, this, IDC_MY_LIST_EDITBOX);
			m_EditItem.SetFont(this->GetFont(), FALSE);
		}
		CRect rect;
		GetSubItemRect(hi.iItem, hi.iSubItem, LVIR_BOUNDS, rect);
		m_EditItem.SetWindowText(this->GetItemText(hi.iItem, hi.iSubItem));
		m_EditItem.MoveWindow(&rect, TRUE);
		m_EditItem.ShowWindow(1);
	}
	CListCtrl::OnLButtonDblClk(nFlags, point);
}


BOOL CListEditCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL bHandledMsg = FALSE;

	if(pMsg->hwnd == m_EditItem.m_hWnd)
	{
		switch (pMsg->message)
		{
		case WM_KEYDOWN:
			{
				switch (pMsg->wParam)
				{
				case VK_RETURN://»Ø³µ
					if(m_Row != -1)
					{
						CString ItemText;
						m_EditItem.GetWindowText(ItemText);
						this->SetItemText(m_Row, m_Col, ItemText);
						::PostMessage(GetParent()->m_hWnd, MLSM_ITEMCHANGED, (WPARAM)MAKELONG(m_Row, m_Col), (LPARAM)this->m_hWnd);
					}
				case VK_ESCAPE://ESC¼ü
					m_EditItem.ShowWindow(0);
					m_Col = m_Row = -1;
					bHandledMsg = TRUE;
					break;
				default:
					break;
				}
			}// case WM_KEYDOWN
			break;
		default:
			break;
		}// switch(pMsg->message)
	}// if(pMsg->hwnd...
	return (bHandledMsg ? TRUE : CListCtrl::PreTranslateMessage(pMsg));
}
