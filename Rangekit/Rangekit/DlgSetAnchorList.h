#pragma once
#include "afxwin.h"


// CDlgSetAnchorList dialog

class CDlgSetAnchorList : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetAnchorList)

public:
	CDlgSetAnchorList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSetAnchorList();

// Dialog Data
	enum { IDD = IDD_DLG_SET_ANCHOR_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_cmbAnchorId;
	UINT m_uAnchorId;
	CString m_strAnchorMac;
	afx_msg void OnBnClickedBtnSetAnchorList();
	int m_nAnchorIndex;
};
