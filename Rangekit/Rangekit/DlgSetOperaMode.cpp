// DlgSetOperaMode.cpp : implementation file
//

#include "stdafx.h"
#include "Rangekit.h"
#include "DlgSetOperaMode.h"
#include "afxdialogex.h"


// CDlgSetOperaMode dialog

IMPLEMENT_DYNAMIC(CDlgSetOperaMode, CDialogEx)

CDlgSetOperaMode::CDlgSetOperaMode(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetOperaMode::IDD, pParent)
	, m_nOperaIdx(0)
{
	m_uOperaMode = 0;
}

CDlgSetOperaMode::~CDlgSetOperaMode()
{
}

void CDlgSetOperaMode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_OPERA_MODE, m_cmbOperaMode);
	DDX_CBIndex(pDX, IDC_CMB_OPERA_MODE, m_nOperaIdx);
}


BEGIN_MESSAGE_MAP(CDlgSetOperaMode, CDialogEx)
	ON_BN_CLICKED(ID_BTN_SET_OPERA_MODE, &CDlgSetOperaMode::OnBnClickedBtnSetOperaMode)
END_MESSAGE_MAP()


// CDlgSetOperaMode message handlers


BOOL CDlgSetOperaMode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_cmbOperaMode.SetCurSel(m_nOperaIdx);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSetOperaMode::OnBnClickedBtnSetOperaMode()
{
	m_uOperaMode = GetDlgItemInt(IDC_CMB_OPERA_MODE);
	OnOK();
}
