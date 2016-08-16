#pragma once

#include "TopologyXDoc.h"

// CDevList view

class CDevList : public CListView
{
	DECLARE_DYNCREATE(CDevList)

protected:
	CDevList();           // protected constructor used by dynamic creation
	virtual ~CDevList();

	// 特性
public:
	CTopologyXDoc* GetDocument() const;
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};


#ifndef _DEBUG  // DevList.cpp 中的调试版本
inline CTopologyXDoc* CDevList::GetDocument() const
{ return reinterpret_cast<CTopologyXDoc*>(m_pDocument); }
#endif

