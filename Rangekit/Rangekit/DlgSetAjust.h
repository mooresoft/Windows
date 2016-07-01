#pragma once
#include "listeditctrl.h"


// CDlgSetAjust dialog

class CDlgSetAjust : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetAjust)

public:
	CDlgSetAjust(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSetAjust();

// Dialog Data
	enum { IDD = IDD_DLG_AJUST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListEditCtrl m_lstedtAjust;
};
