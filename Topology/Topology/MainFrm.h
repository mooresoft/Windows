
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "serialport.h"
#include "DlgOpenSerial.h"
#include "HPVec.h"

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	CSerialPort m_serial;
	// Attributes
public:
	CDlgOpenSerial m_dlgOpenSerial;
	HANDLE			  m_hReadThrd;
	volatile BOOL		  m_bRead;
	std::string            m_sCmd;
	CRITICAL_SECTION  m_csRecvSerail;
	std::list<std::string> m_qsCmd;
// ����
protected:
	CSplitterWnd m_wndSplitter;
public:

// ����
public:

// ��д
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnRecvSerial(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
public:
	afx_msg void OnFileOpen();
	afx_msg void OnEmulate();
	afx_msg void OnSerialClose();
public:
	void     south_parse_msg();
	void     calc_topology(unsigned char* pmsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	UINT16 m_emu_aids[MAX_DIM];
	CHPVec2D m_emu_pos[MAX_DIM];
	UINT16 m_emu_r[MAX_DIM][MAX_DIM];

};


