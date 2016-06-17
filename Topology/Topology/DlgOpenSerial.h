#pragma once
#include "afxwin.h"


// CDlgOpenSerial dialog

class CDlgOpenSerial : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgOpenSerial)

public:
	CDlgOpenSerial(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgOpenSerial();

// Dialog Data
	enum { IDD = IDD_DLG_OPEN_SERIAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbPorts;
	afx_msg void OnBnClickedBtnRefreshPort();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeCmbPorts();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	BOOL ReadPort(CStringArray& caPorts);
	BOOL CheckPorts(CStringArray& caPorts);
	void SavePorts(CStringArray& caPorts);

	void InitPortCmb(BOOL bAutoOpen = TRUE);

	int m_nCountDown;
	CStringArray m_caPorts;
public:
	int m_nPort;
	int m_nBoudRate;
};
