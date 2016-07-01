// DlgSetAntennaDelay.cpp : implementation file
//

#include "stdafx.h"
#include "Rangekit.h"
#include "DlgSetAntennaDelay.h"
#include "afxdialogex.h"


// CDlgSetAntennaDelay dialog

IMPLEMENT_DYNAMIC(CDlgSetAntennaDelay, CDialogEx)

CDlgSetAntennaDelay::CDlgSetAntennaDelay(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_SET_ANTENNA_DELAY, pParent)
	, m_strAntennaDelay(_T("16520"))
{
	m_uAntennaDelay = 16520;
}

CDlgSetAntennaDelay::~CDlgSetAntennaDelay()
{
}

void CDlgSetAntennaDelay::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ANTENNA_DELAY, m_strAntennaDelay);
	DDV_MaxChars(pDX, m_strAntennaDelay, 5);
}


BEGIN_MESSAGE_MAP(CDlgSetAntennaDelay, CDialogEx)
	ON_BN_CLICKED(ID_BTN_SET_ANTENNA_DELAY, &CDlgSetAntennaDelay::OnBnClickedBtnSetAntennaDelay)
END_MESSAGE_MAP()


// CDlgSetAntennaDelay message handlers


void CDlgSetAntennaDelay::OnBnClickedBtnSetAntennaDelay()
{
	UpdateData();
	m_uAntennaDelay = GetDlgItemInt(IDC_EDIT_ANTENNA_DELAY);
	CDialogEx::OnOK();
}


BOOL CDlgSetAntennaDelay::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
