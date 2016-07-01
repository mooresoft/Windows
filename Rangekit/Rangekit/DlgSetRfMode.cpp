// DlgSetRfMode.cpp : implementation file
//

#include "stdafx.h"
#include "Rangekit.h"
#include "DlgSetRfMode.h"
#include "afxdialogex.h"


// CDlgSetRfMode dialog

IMPLEMENT_DYNAMIC(CDlgSetRfMode, CDialogEx)

CDlgSetRfMode::CDlgSetRfMode(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetRfMode::IDD, pParent)
	, m_nRfModeIdx(0)
{
	m_uRfMode = 0;
}

CDlgSetRfMode::~CDlgSetRfMode()
{
}

void CDlgSetRfMode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_RFMODE, m_cmbRfMode);
	DDX_CBIndex(pDX, IDC_CMB_RFMODE, m_nRfModeIdx);
}


BEGIN_MESSAGE_MAP(CDlgSetRfMode, CDialogEx)
	ON_BN_CLICKED(ID_BTN_SET_RFMODE, &CDlgSetRfMode::OnBnClickedBtnSetRfmode)
END_MESSAGE_MAP()


// CDlgSetRfMode message handlers


BOOL CDlgSetRfMode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_cmbRfMode.SetCurSel(m_nRfModeIdx);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSetRfMode::OnBnClickedBtnSetRfmode()
{
	m_uRfMode = GetDlgItemInt(IDC_CMB_RFMODE);
	OnOK();
}
