
// VisualDistanceView.h : interface of the CVisualDistanceView class
//

#pragma once
class CVisualDistanceDoc;

class CVisualDistanceView : public CListView
{
protected: // create from serialization only
	CVisualDistanceView();
	DECLARE_DYNCREATE(CVisualDistanceView)

// Attributes
public:
	CVisualDistanceDoc* GetDocument() const;

// Operations
public:
	void RefreshView();
	void Start();
	void Stop();
	void ClearView(void);

	BOOL			  m_bRefresh;
	HANDLE			  m_hRefreshThrd;
// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CVisualDistanceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // debug version in VisualDistanceView.cpp
inline CVisualDistanceDoc* CVisualDistanceView::GetDocument() const
   { return reinterpret_cast<CVisualDistanceDoc*>(m_pDocument); }
#endif

