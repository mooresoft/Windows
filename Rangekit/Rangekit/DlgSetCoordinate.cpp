// DlgSetCoordinate.cpp : implementation file
//

#include "stdafx.h"
#include "Rangekit.h"
#include "DlgSetCoordinate.h"
#include "afxdialogex.h"


// CDlgSetCoordinate dialog

IMPLEMENT_DYNAMIC(CDlgSetCoordinate, CDialogEx)

CDlgSetCoordinate::CDlgSetCoordinate(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_SET_COORDINATE, pParent)
	, m_strCoorX(_T(""))
	, m_strCoorY(_T(""))
{
	m_dCoorX = 0.0;
	m_dCoorY = 0.0;
}

CDlgSetCoordinate::~CDlgSetCoordinate()
{
}

void CDlgSetCoordinate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, m_strCoorX);
	DDV_MaxChars(pDX, m_strCoorX, 8);
	DDX_Text(pDX, IDC_EDIT_Y, m_strCoorY);
	DDV_MaxChars(pDX, m_strCoorY, 8);
}


BEGIN_MESSAGE_MAP(CDlgSetCoordinate, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSetCoordinate::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSetCoordinate message handlers


void CDlgSetCoordinate::OnBnClickedOk()
{
	char szCoor[10] =  {0};
	UpdateData();
	theApp.CStringToChar(m_strCoorX, szCoor, 10);
	m_dCoorX = atof(szCoor);
	theApp.CStringToChar(m_strCoorY, szCoor, 10);
	m_dCoorY = atof(szCoor);
	CDialogEx::OnOK();
}
