#pragma once
#include "afxwin.h"


// CDlgSetOperaMode dialog

class CDlgSetOperaMode : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetOperaMode)

public:
	CDlgSetOperaMode(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSetOperaMode();

// Dialog Data
	enum { IDD = IDD_DLG_SET_OPERA_MODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbOperaMode;
	UINT m_uOperaMode;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSetOperaMode();
	int m_nOperaIdx;
};
