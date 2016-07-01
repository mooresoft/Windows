// DlgSetAjust.cpp : implementation file
//

#include "stdafx.h"
#include "Rangekit.h"
#include "DlgSetAjust.h"
#include "afxdialogex.h"


// CDlgSetAjust dialog

IMPLEMENT_DYNAMIC(CDlgSetAjust, CDialogEx)

CDlgSetAjust::CDlgSetAjust(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetAjust::IDD, pParent)
{

}

CDlgSetAjust::~CDlgSetAjust()
{
}

void CDlgSetAjust::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_AJUST, m_lstedtAjust);
}


BEGIN_MESSAGE_MAP(CDlgSetAjust, CDialogEx)
END_MESSAGE_MAP()


// CDlgSetAjust message handlers
