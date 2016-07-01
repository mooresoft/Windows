#pragma once


// CDlgSetCustom dialog

class CDlgSetCustom : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetCustom)

public:
	CDlgSetCustom(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSetCustom();

// Dialog Data
	enum { IDD = IDD_DLG_SET_CUSTOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strCommand;
	afx_msg void OnBnClickedBtnSetCustom();
};
