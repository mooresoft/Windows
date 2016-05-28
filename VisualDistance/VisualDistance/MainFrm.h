
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "serialport.h"
#include "DlgOpenSerial.h"
#include "DlgAjust.h"
#include <queue>
#include <fstream>
#include <list>
using namespace std;

#define WM_UPDATE_INDICATOR				WM_USER+100

#pragma pack(1)
typedef struct
{
	unsigned long  index;
	unsigned short seq;
	unsigned char  tmac[5];
	unsigned char  amac[5];
	unsigned long  averages;
	unsigned long  maxv;
	unsigned long  minv;
	unsigned long  counts;
	unsigned long  terribles;
	list<unsigned long> distances;
}DISTANCE_NODE;

#pragma pack()

typedef list<DISTANCE_NODE> DISTANCE_LIST;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	CSerialPort m_serial;
	
// Attributes
public:
	HANDLE			  m_hReadThrd;
	BOOL			  m_bRead;
	fstream			  m_file;
// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();

	void FillReport(void);
	DISTANCE_LIST& GetDistances();
	void ReleaseDistances(void);
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

	CDlgOpenSerial    m_dlgOpenSerial;
	CDlgAjust         m_dlgAjust;
private:
	BOOL isVisibleChar(const char szWord);
	

	string            m_strDistance;
	queue<string>     m_qsDistance;
	DISTANCE_LIST     m_listDistance;
	CRITICAL_SECTION  m_csDistance;
	CRITICAL_SECTION  m_csRecvSerail;
	CTime             m_StartTime;
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnRecvSerial(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateIndicator(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnFileOpen();
	afx_msg void OnSerialClose();
	afx_msg void OnClose();
	afx_msg void OnEditReset();
	afx_msg void OnEditAjust();
	afx_msg void OnEditAjustall();
};


