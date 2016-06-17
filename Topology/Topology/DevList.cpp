// DevList.cpp : implementation file
//

#include "stdafx.h"
#include "Topology.h"
#include "DevList.h"


// CDevList

IMPLEMENT_DYNCREATE(CDevList, CListView)

CDevList::CDevList()
{

}

CDevList::~CDevList()
{
}

BEGIN_MESSAGE_MAP(CDevList, CListView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CDevList diagnostics

#ifdef _DEBUG
void CDevList::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CDevList::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDevList message handlers


int CDevList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CListCtrl& m_list = GetListCtrl();
	LONG lStyle;
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT;

	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);
	DWORD dwStyle = m_list.GetExtendedStyle();

	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_list.SetExtendedStyle(dwStyle);
	//m_list.SetBkColor(RGB(200, 200, 200));
	m_list.SetTextBkColor(RGB(200, 200, 200));
	//m_list.SetTextColor(RGB(10, 10, 80));

	CString strContent;
	m_list.InsertColumn(0, _T(""), LVCFMT_CENTER, 80);
	m_list.InsertColumn(1, _T("9000"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(2, _T("9001"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(3, _T("9002"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(4, _T("9003"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(5, _T("9004"), LVCFMT_CENTER, 80);

	for (int i = 0; i < 5; i++)
	{
		strContent.Format(_T("%d (X, Y )"),9000 + i);
		m_list.InsertItem(i, _T(""));
		m_list.SetItemText(i, 0, strContent);
	}

	//m_list.DeleteColumn(0);	
	return 0;
}
