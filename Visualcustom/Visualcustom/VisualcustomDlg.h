
// VisualcustomDlg.h : header file
//

#pragma once

#include "DlgOpenSerial.h"
#include "serialport.h"
#include "UsartConf.h"
#include "afxwin.h"
#include "listeditctrl.h"

// CVisualcustomDlg dialog
class CVisualcustomDlg : public CDialogEx
{
// Construction
public:
	CVisualcustomDlg(CWnd* pParent = NULL);	// standard constructor
	~CVisualcustomDlg();

// Dialog Data
	enum { IDD = IDD_VISUALCUSTOM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


private:
	void EnableButtons(BOOL enabled = TRUE);
	void CloseSerial(void);
	void InitAnchorList(void);

	CRITICAL_SECTION  m_csRecvSerail;

	DEVICE_PARAM   m_applyparam;
	unsigned char  m_dstmac[2];
	CSerialPort    m_serial;
	CDlgOpenSerial m_dlgOpenSerial;
	CUsartConf     m_usartconf;
	CString        m_strcommand;
	UINT           m_uRfModeIdx;
	HBRUSH         m_brushEditbk;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnRecvSerial(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnOpenPort();

	BOOL  m_bOpenPort;
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnRead();
	afx_msg void OnBnClickedBtnReboot();
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnUpgrade();
	afx_msg void OnBnClickedBtnPower();
	afx_msg void OnBnClickedBtnApply();
	BOOL m_bSysType;
	CString m_strdevid;
	UINT m_usleeptime;
	CComboBox m_cmbRfMode;
	UINT m_uAntennaDelay;
	CListEditCtrl m_listanchors;
	afx_msg void OnDeltaposSpinSpectrum(NMHDR *pNMHDR, LRESULT *pResult);
};
