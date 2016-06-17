#pragma once


// CDevList view

class CDevList : public CListView
{
	DECLARE_DYNCREATE(CDevList)

protected:
	CDevList();           // protected constructor used by dynamic creation
	virtual ~CDevList();

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
};


