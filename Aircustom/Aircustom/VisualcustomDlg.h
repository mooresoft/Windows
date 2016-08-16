
// VisualcustomDlg.h : header file
//

#pragma once

#include "DlgOpenSerial.h"
#include "serialport.h"
#include "UsartConf.h"
#include "afxwin.h"
#include "listeditctrl.h"
#include <queue>
#include <string>
#include "afxcmn.h"
using namespace std;

typedef enum _ENUM_OTA_MGMT
{
	OTA_SUC         = 0x00,
	OTA_SEQ_ERR     = 0x01,
	WRITE_FLASH_ERR = 0x02,
	DATA_ERR        = 0x03,
	OTA_UNKNOWN     = 0xFF,
}OTA_MGMT_TYPE;
// CVisualcustomDlg dialog
class CVisualcustomDlg : public CDialogEx
{
// Construction
public:
	CVisualcustomDlg(CWnd* pParent = NULL);	// standard constructor
	~CVisualcustomDlg();
	void HandleRx(void);
	void UpgradeDevice(void);
// Dialog Data
	enum { IDD = IDD_VISUALCUSTOM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


private:
	void EnableButtons(BOOL enabled = TRUE);
	void CloseSerial(void);
	void InitDevList(void);
	void InitAnchorList(void);
	void AddDevList(void);
	void AddMgmtInfo(CString strInfo);
	void openupfile(void);
	void readupfile(void);

	CRITICAL_SECTION  m_csRecvSerail;

	HANDLE         m_hUpgradeThrd;
	HANDLE		   m_hHandleRxThrd;
	DEVICE_PARAM   m_applyparam;
	unsigned char  m_dstmac[2];
	CSerialPort    m_serial;
	CDlgOpenSerial m_dlgOpenSerial;
	CUsartConf     m_usartconf;
	queue<string>  m_qsCmd;
	string         m_sCmd;
	UINT           m_uRfModeIdx;
	HBRUSH         m_brushEditbk;
	BOOL           m_bStartRecv;
	CString        m_strUpgradeFile;
	CString        m_strUpFileName;
	BYTE*          m_pbin;
	ULONGLONG      m_binsize;
	BOOL           m_bUpgrade;
	ULONGLONG      m_sentbinsize;
	unsigned char  m_sentseq;
	int            m_nUpgradeAck;
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
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnRead();
	afx_msg void OnBnClickedBtnReboot();
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnUpgrade();
	afx_msg void OnBnClickedBtnPower();
	afx_msg void OnBnClickedBtnApply();
	CString       m_strdevid;
	UINT          m_usleeptime;
	CComboBox     m_cmbRfMode;
	UINT          m_uAntennaDelay;
	CListEditCtrl m_listanchors;
	BOOL          m_bSysType;
	afx_msg void OnBnClickedRadioTdoa();
	afx_msg void OnBnClickedRadioTwr();
	CListCtrl m_lstDevice;
	CString m_strDevId;
	afx_msg void OnBnClickedBtnEntercli();
};
