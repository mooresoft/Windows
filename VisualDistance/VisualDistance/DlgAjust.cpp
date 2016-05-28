// DlgAjust.cpp : implementation file
//

#include "stdafx.h"
#include "VisualDistance.h"
#include "DlgAjust.h"
#include "afxdialogex.h"


// CDlgAjust dialog

IMPLEMENT_DYNAMIC(CDlgAjust, CDialogEx)

CDlgAjust::CDlgAjust(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_AJUST, pParent)
	, m_nAjust(0)
{
	m_uStepId = 0;
}

CDlgAjust::~CDlgAjust()
{
}

void CDlgAjust::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STEP_ID, m_cmbStepId);
	DDX_Text(pDX, IDC_EDIT_AJUST, m_nAjust);
	DDV_MinMaxInt(pDX, m_nAjust, -500, 500);
}


BEGIN_MESSAGE_MAP(CDlgAjust, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgAjust::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAjust message handlers


BOOL CDlgAjust::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString strStepId;
	for (int i = 0; i < 50; i++)
	{
		strStepId.Format(_T("%d"), i);
		m_cmbStepId.AddString(strStepId);
	}
	m_cmbStepId.SetCurSel(m_uStepId);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgAjust::OnBnClickedOk()
{
	m_uStepId = m_cmbStepId.GetCurSel();
	UpdateData();
	CDialogEx::OnOK();
}
