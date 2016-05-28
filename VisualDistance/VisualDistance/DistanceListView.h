#pragma once


// CDistanceListView view

class CDistanceListView : public CListView
{
	DECLARE_DYNCREATE(CDistanceListView)

protected:
	CDistanceListView();           // protected constructor used by dynamic creation
	virtual ~CDistanceListView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


