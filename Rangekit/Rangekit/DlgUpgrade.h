#pragma once


// CDlgUpgrade dialog

class CDlgUpgrade : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUpgrade)

public:
	CDlgUpgrade(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgUpgrade();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_UPGRADE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
