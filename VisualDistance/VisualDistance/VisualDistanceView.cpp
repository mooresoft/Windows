
// VisualDistanceView.cpp : implementation of the CVisualDistanceView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "VisualDistance.h"
#endif

#include "VisualDistanceDoc.h"
#include "VisualDistanceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVisualDistanceView

IMPLEMENT_DYNCREATE(CVisualDistanceView, CListView)

BEGIN_MESSAGE_MAP(CVisualDistanceView, CListView)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CVisualDistanceView construction/destruction

CVisualDistanceView::CVisualDistanceView()
{
	// TODO: add construction code here
	m_bRefresh = FALSE;

}

CVisualDistanceView::~CVisualDistanceView()
{
	Stop();
}

BOOL CVisualDistanceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

void CVisualDistanceView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
}


// CVisualDistanceView diagnostics

#ifdef _DEBUG
void CVisualDistanceView::AssertValid() const
{
	CListView::AssertValid();
}

void CVisualDistanceView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CVisualDistanceDoc* CVisualDistanceView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVisualDistanceDoc)));
	return (CVisualDistanceDoc*)m_pDocument;
}
#endif //_DEBUG


// CVisualDistanceView message handlers

DWORD __stdcall RefreshThrd(LPVOID pParam)
{
	CVisualDistanceView* pView = NULL;
	int refreshcount = 0;

	if (NULL != pParam)
	{
		pView = (CVisualDistanceView*)pParam;
		while (TRUE == pView->m_bRefresh)
		{
			if (++refreshcount > 9)
			{
				pView->RefreshView();
				refreshcount = 0;
			}
			Sleep(100);
		}
	}

	return 0;
}

int CVisualDistanceView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
		
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
	
	m_list.InsertColumn(0, _T("id"), LVCFMT_CENTER, 80);

	m_list.InsertColumn(1, _T("TMAC"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(2, _T("AMAC"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(3, _T("AVERAGE"), LVCFMT_CENTER, 110);
	m_list.InsertColumn(4, _T("STDEV"), LVCFMT_CENTER, 110);
	m_list.InsertColumn(5, _T("MAX"), LVCFMT_CENTER, 110);
	m_list.InsertColumn(6, _T("MIN"), LVCFMT_CENTER, 110);
	m_list.InsertColumn(7, _T("COUNT"), LVCFMT_CENTER, 110);
	m_list.InsertColumn(8, _T("LOST"), LVCFMT_CENTER, 110);
	m_list.InsertColumn(9, _T("LOST(%)"), LVCFMT_CENTER, 110);
	m_list.InsertColumn(10, _T("TERRIBLE"), LVCFMT_CENTER, 110);
	m_list.InsertColumn(11, _T("TERRIBLE(%)"), LVCFMT_CENTER, 110);
	m_list.DeleteColumn(0);	

	Start();
	return 0;
}

void stdev(list<unsigned long>& distances, const unsigned long averages, CString& strText)
{
	list<unsigned long>::iterator i;
	double sum = 0.0;
	int xi;
	int x = averages;
	int j = 0;

	for (i = distances.begin(); i != distances.end(); i++) 
	{
		xi = (*i);
		sum += pow((xi - x), 2);
		j++;
	}

	if (j > 0)
	{
		sum /= j;
		strText.Format(_T("%f"), sqrt(sum));
	}

	return;
}

void CVisualDistanceView::RefreshView()
{
	if (NULL != theApp.m_pMainFrame)
	{
		DISTANCE_LIST::iterator i;
		int j = 0;
		CListCtrl& m_list = GetListCtrl();
		CString strText;
		unsigned long losts;

		m_list.SetRedraw(FALSE);
		DISTANCE_LIST& dis_rep = theApp.m_pMainFrame->GetDistances();

		for (i = dis_rep.begin(); i != dis_rep.end(); i++)
		{
			if (j >= m_list.GetItemCount())
			{
				m_list.InsertItem(j, _T(""));
			}
			strText = (*i).tmac;
			m_list.SetItemText(j, 0, strText);
			strText = (*i).amac;
			m_list.SetItemText(j, 1, strText);

			strText.Format(_T("%d"), (*i).averages);
			m_list.SetItemText(j, 2, strText);

			stdev((*i).distances, (*i).averages, strText);
			m_list.SetItemText(j, 3, strText);

			strText.Format(_T("%d"), (*i).maxv);
			m_list.SetItemText(j, 4, strText);

			strText.Format(_T("%d"), (*i).minv);
			m_list.SetItemText(j, 5, strText);

			strText.Format(_T("%d"), (*i).index);
			m_list.SetItemText(j, 6, strText);
			losts = (*i).index - (*i).counts;
			strText.Format(_T("%d"), losts);
			m_list.SetItemText(j, 7, strText);
			strText.Format(_T("%f"), (losts * 100.0) / (*i).index);
			m_list.SetItemText(j, 8, strText);
			strText.Format(_T("%d"), (*i).terribles);
			m_list.SetItemText(j, 9, strText);
			strText.Format(_T("%f"), ((*i).terribles * 100.0) / (*i).index);
			m_list.SetItemText(j, 10, strText);
			j++;
		}
		theApp.m_pMainFrame->ReleaseDistances();
		m_list.SetRedraw(TRUE);
		
		theApp.m_pMainFrame->PostMessage(WM_UPDATE_INDICATOR);
	}
}

void CVisualDistanceView::Start()
{
	m_bRefresh = TRUE;
	m_hRefreshThrd = CreateThread(NULL, 0, RefreshThrd, this, 0, NULL);
}

void CVisualDistanceView::Stop()
{
	m_bRefresh = FALSE;
	if (NULL != m_hRefreshThrd)
	{
		WaitForSingleObject(m_hRefreshThrd, INFINITE);
		CloseHandle(m_hRefreshThrd);
		m_hRefreshThrd = NULL;
	}
}
