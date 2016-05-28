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
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_OPEN_SERIAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
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
	int m_nStopBits;
	CComboBox m_cmbPort;
	int m_nBoudRateIdx;
	CComboBox m_cmbBoudRate;
	CComboBox m_cmbStopBits;
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnRefresh();
	afx_msg void OnCbnSelchangeCmbPort();
	afx_msg void OnCbnSelchangeCmbBoudrate();
	afx_msg void OnCbnSelchangeCmbStopbits();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
