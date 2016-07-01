#pragma once
#include "afxwin.h"


// CDlgSetRfMode dialog

class CDlgSetRfMode : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetRfMode)

public:
	CDlgSetRfMode(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSetRfMode();

// Dialog Data
	enum { IDD = IDD_DLG_SET_RFMODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbRfMode;
	UINT m_uRfMode;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSetRfmode();
	int m_nRfModeIdx;
};
