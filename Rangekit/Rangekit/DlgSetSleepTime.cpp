// DlgSetSleepTime.cpp : implementation file
//

#include "stdafx.h"
#include "Rangekit.h"
#include "DlgSetSleepTime.h"
#include "afxdialogex.h"


// CDlgSetSleepTime dialog

IMPLEMENT_DYNAMIC(CDlgSetSleepTime, CDialogEx)

CDlgSetSleepTime::CDlgSetSleepTime(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetSleepTime::IDD, pParent)
	, m_uSleepTime(100)
{

}

CDlgSetSleepTime::~CDlgSetSleepTime()
{
}

void CDlgSetSleepTime::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SLEEP_TIME, m_uSleepTime);
	DDV_MinMaxUInt(pDX, m_uSleepTime, 1, 50000);
}


BEGIN_MESSAGE_MAP(CDlgSetSleepTime, CDialogEx)
	ON_BN_CLICKED(ID_BTN_SET_SLEEP_TIME, &CDlgSetSleepTime::OnBnClickedBtnSetSleepTime)
END_MESSAGE_MAP()


// CDlgSetSleepTime message handlers


BOOL CDlgSetSleepTime::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSetSleepTime::OnBnClickedBtnSetSleepTime()
{
	UpdateData();
	OnOK();
}
