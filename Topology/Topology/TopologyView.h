
// TopologyView.h : interface of the CTopologyView class
//

#pragma once

class CTopologyDoc;
class CTopologyView : public CView
{
protected: // create from serialization only
	CTopologyView();
	DECLARE_DYNCREATE(CTopologyView)

// Attributes
public:
	CTopologyDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CTopologyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
};

#ifndef _DEBUG  // debug version in TopologyView.cpp
inline CTopologyDoc* CTopologyView::GetDocument() const
   { return reinterpret_cast<CTopologyDoc*>(m_pDocument); }
#endif

