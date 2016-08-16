
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "serialport.h"
#include "DlgOpenSerial.h"
#include "HPVec.h"

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
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
// 特性
protected:
	CSplitterWnd m_wndSplitter;
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// 生成的消息映射函数
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


