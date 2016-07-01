// DlgSetCustom.cpp : implementation file
//

#include "stdafx.h"
#include "Rangekit.h"
#include "DlgSetCustom.h"
#include "afxdialogex.h"


// CDlgSetCustom dialog

IMPLEMENT_DYNAMIC(CDlgSetCustom, CDialogEx)

CDlgSetCustom::CDlgSetCustom(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetCustom::IDD, pParent)
	, m_strCommand(_T(""))
{

}

CDlgSetCustom::~CDlgSetCustom()
{
}

void CDlgSetCustom::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_COMMAND, m_strCommand);
	DDV_MaxChars(pDX, m_strCommand, 255);
}


BEGIN_MESSAGE_MAP(CDlgSetCustom, CDialogEx)
	ON_BN_CLICKED(ID_BTN_SET_CUSTOM, &CDlgSetCustom::OnBnClickedBtnSetCustom)
END_MESSAGE_MAP()


// CDlgSetCustom message handlers


BOOL CDlgSetCustom::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSetCustom::OnBnClickedBtnSetCustom()
{
	UpdateData();
	OnOK();
}
