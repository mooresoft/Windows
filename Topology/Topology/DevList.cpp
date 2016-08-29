// DevList.cpp : implementation file
//

#include "stdafx.h"
#include "TopologyX.h"
#include "DevList.h"
#include "TopologyXDoc.h"

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

CTopologyXDoc* CDevList::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTopologyXDoc)));
	return (CTopologyXDoc*)m_pDocument;
}
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
	m_list.InsertColumn(0, _T("MAC"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(1, _T("X, Y"), LVCFMT_LEFT, 100);


	//m_list.DeleteColumn(0);	
	return 0;
}


void CDevList::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	CTopologyXDoc *pDoc = GetDocument();
	if (pDoc == NULL)
		return;
//	return;
	// TODO: 在此添加专用代码和/或调用基类
	CListCtrl &list=GetListCtrl();
	list.SetRedraw(FALSE);

	list.DeleteAllItems();

	INT row;

	row = 0;
	CString str;
	std::map<UINT64, CTopoDev> *pDevs = pDoc->GetDevMap();
	for (std::map<UINT64, CTopoDev>::iterator itr = pDevs->begin(); itr != pDevs->end(); itr++)
	{
		CTopoDev &dev = itr->second;
		str.Format(_T("%X"), dev.m_id);
		list.InsertItem(row, str);

		str.Format(_T("%.2f,%.2f"), dev.m_pos.x, dev.m_pos.y);
		list.SetItemText(row, 1, str);
		row++;
	}

	list.SetRedraw(TRUE);
	list.Invalidate();
	list.UpdateWindow();

}
