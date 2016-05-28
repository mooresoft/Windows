// DistanceListView.cpp : implementation file
//

#include "stdafx.h"
#include "VisualDistance.h"
#include "DistanceListView.h"


// CDistanceListView

IMPLEMENT_DYNCREATE(CDistanceListView, CListView)

CDistanceListView::CDistanceListView()
{

}

CDistanceListView::~CDistanceListView()
{
}

BEGIN_MESSAGE_MAP(CDistanceListView, CListView)
END_MESSAGE_MAP()


// CDistanceListView diagnostics

#ifdef _DEBUG
void CDistanceListView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CDistanceListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDistanceListView message handlers
