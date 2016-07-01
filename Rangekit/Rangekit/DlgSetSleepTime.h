#pragma once


// CDlgSetSleepTime dialog

class CDlgSetSleepTime : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetSleepTime)

public:
	CDlgSetSleepTime(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSetSleepTime();

// Dialog Data
	enum { IDD = IDD_DLG_SET_SLEEP_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT m_uSleepTime;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSetSleepTime();
};
