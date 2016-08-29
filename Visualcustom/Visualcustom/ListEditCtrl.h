#pragma once

#define IDC_MY_LIST_EDITBOX 0xffff
#define MLSM_ITEMCHANGED (WM_USER + 200)

// CListEditCtrl

class CListEditCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CListEditCtrl)

public:
	CListEditCtrl();
	virtual ~CListEditCtrl();

protected:
	CEdit m_EditItem;
	int   m_Row;
	int   m_Col;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


