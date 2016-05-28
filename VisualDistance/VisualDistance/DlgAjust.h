#pragma once
#include "afxwin.h"


// CDlgAjust dialog

class CDlgAjust : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAjust)

public:
	CDlgAjust(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAjust();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_AJUST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT m_uStepId;
	virtual BOOL OnInitDialog();
	CComboBox m_cmbStepId;
	int       m_nAjust;
	afx_msg void OnBnClickedOk();
};
