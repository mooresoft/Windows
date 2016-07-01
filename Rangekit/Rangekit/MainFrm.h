
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "DlgSerialSet.h"
#include "DlgSetOperaMode.h"
#include "DlgSetAnchorList.h"
#include "DlgSetRfMode.h"
#include "DlgUpgrade.h"
#include "DlgSetAjust.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
	void ReadSerial();
	void SendYmodem(void);

	BOOL m_bReadSerial;
	HANDLE m_hCom;
	HANDLE m_hReadSerial;

	CDlgUpgrade m_dlgUpgrade;
// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

private:
	void OpenSerial();
	void OpenSerial(UINT bondrate);
	void WriteSerial(const CString& strCmd);
	

	CString m_strPostName;
	int m_nBoudRate;
	int m_nStopBits;
	CDlgSerialSet m_dlgSerialSet;
	CDlgSetAnchorList m_dlgAnchorListSet;
	CDlgSetRfMode m_dlgSetRfMode;
	CDlgSetOperaMode m_dlgSetOperaMode;
	CDlgSetAjust     m_dlgSetAust;
	CString m_strCmd;
	BOOL m_bUpgrade;
	CString m_strUpgradeFile;
	CString m_strUpFileName;
	OVERLAPPED m_osRead;
	CView*     m_pView;
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnWriteSerial(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTransmitYmodem(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
	afx_msg void OnConnectOpen();
	afx_msg void OnConnectClose();
	afx_msg void OnViewOperatormode();
	afx_msg void OnViewAnchorlist();
	afx_msg void OnViewMac();
	afx_msg void OnViewRfmode();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSettingMode();
	afx_msg void OnSettingAnchor();
	afx_msg void OnSettingRfmode();
	afx_msg void OnSettingReboot();
	afx_msg void OnSettingReset();
	afx_msg void OnHelpClear();
	afx_msg void OnViewSleeptime();
	afx_msg void OnSettingSleeptime();
	afx_msg void OnSettingCustom();
	afx_msg void OnSettingCoordinate();
	afx_msg void OnViewCoordinate();
	afx_msg void OnViewAntennadelay();
	afx_msg void OnSettingAntennadelay();
	afx_msg void OnTransferSendymodem();
	afx_msg void OnHelpLogfolder();
	afx_msg void OnTestPower();
	afx_msg void OnUpdateTestPower(CCmdUI *pCmdUI);
	afx_msg void OnSettingAjustlist();
};


