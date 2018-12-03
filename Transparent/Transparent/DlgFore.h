#pragma once


// CDlgFore dialog

class CDlgFore : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFore)

public:
	CDlgFore(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFore();
	void SetTextBkColor(const COLORREF& clr);

// Dialog Data
	enum { IDD = IDD_DLG_FORE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	COLORREF	m_textColor;
	COLORREF	m_textBkColor;
	BOOL		m_bTracked;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void PostNcDestroy();
	afx_msg void OnClose();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};
