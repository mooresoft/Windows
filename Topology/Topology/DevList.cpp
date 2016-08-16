// DevList.cpp : implementation file
//

#include "stdafx.h"
#include "TopologyX.h"
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
	m_list.InsertColumn(0, _T(""), LVCFMT_CENTER, 80);
	m_list.InsertColumn(1, _T("9000"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(2, _T("9001"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(3, _T("9002"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(4, _T("9003"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(5, _T("9004"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(6, _T("X, Y"), LVCFMT_LEFT, 100);

	for (int i = 0; i < 5; i++)
	{
		strContent.Format(_T("%d (X, Y )"),9000 + i);
		m_list.InsertItem(i, _T(""));
		m_list.SetItemText(i, 0, strContent);
	}


	//m_list.DeleteColumn(0);	
	return 0;
}


void CDevList::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
//	return;
	// TODO: 在此添加专用代码和/或调用基类
	CListCtrl &list=GetListCtrl();
	list.SetRedraw(FALSE);

	list.DeleteAllItems();

	INT i,j,nCol = list.GetHeaderCtrl()->GetItemCount();
	for(i=nCol-1; i>=0; i--)
		list.DeleteColumn(i);

	CString str;
	std::vector<UINT> *pids = GetDocument()->GetDevIDs();
	double *rangs = GetDocument()->GetRMatrix();
	HPVEC2D *ppos = GetDocument()->GetPosArr();
	wchar_t wbuf[32];
	nCol = pids->size();
	list.InsertColumn(0, _T(""), LVCFMT_CENTER, 80);
	for(i=0; i<nCol; i++)
	{
		str.Format(L"%X", pids->at(i));
		list.InsertColumn(i+1, str, LVCFMT_CENTER, 80);
	}
	list.InsertColumn(6, _T("X, Y"), LVCFMT_LEFT, 100);

	for(i=0; i<nCol; i++)
	{
		str.Format(L"%X", pids->at(i));
		list.InsertItem(i, str);
	}
	for(i=0; i<nCol; i++)
	{
		for(j=0; j<nCol; j++)
		{
			str.Format(L"%d", (INT)(MAT_ITEM(rangs,i,j)*100));
			list.SetItemText(i, j+1, str);
		}
		str.Format(L"%d, %d", (INT)(ppos[i].x*100),(INT)(ppos[i].y*100));
		list.SetItemText(i, j+1, str);
	}

	list.SetRedraw(TRUE);
	list.Invalidate();
	list.UpdateWindow();

}
