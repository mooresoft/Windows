#pragma once
#include "afxwin.h"


// CDlgSerialSet dialog

class CDlgSerialSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSerialSet)

public:
	CDlgSerialSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSerialSet();

// Dialog Data
	enum { IDD = IDD_DLG_SERIAL_SET };

private:
	BOOL ReadPort(CStringArray& caPorts);
	BOOL CheckPorts(CStringArray& caPorts);
	void SavePorts(CStringArray& caPorts);
	void InitPortCmb(BOOL bAutoOpen = TRUE);

	int m_nCountDown;
	CStringArray m_caPorts;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_cmbPort;
	CComboBox m_cmbBoudRate;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOpenSerial();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeCmbPort();
	afx_msg void OnCbnSelchangeCmbBoudrate();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnRefresh();
	CString m_strPortName;
	int m_nBoudRate;
	int m_nStopBits;
	int m_nBoudRateIndex;
	CComboBox m_cmbStopBits;
	int m_nStopBitsIdx;
};
