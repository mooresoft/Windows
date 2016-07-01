#pragma once


// CDlgSetCoordinate dialog

class CDlgSetCoordinate : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetCoordinate)

public:
	CDlgSetCoordinate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSetCoordinate();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SET_COORDINATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_dCoorX;
	double m_dCoorY;
	CString m_strCoorX;
	CString m_strCoorY;
	afx_msg void OnBnClickedOk();
};
