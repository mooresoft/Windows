#pragma once


// CDlgSetAntennaDelay dialog

class CDlgSetAntennaDelay : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetAntennaDelay)

public:
	CDlgSetAntennaDelay(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSetAntennaDelay();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SET_ANTENNA_DELAY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strAntennaDelay;
	unsigned int m_uAntennaDelay;
	afx_msg void OnBnClickedBtnSetAntennaDelay();
	virtual BOOL OnInitDialog();
};
