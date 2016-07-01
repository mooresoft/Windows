// DlgSetAnchorList.cpp : implementation file
//

#include "stdafx.h"
#include "Rangekit.h"
#include "DlgSetAnchorList.h"
#include "afxdialogex.h"


// CDlgSetAnchorList dialog

IMPLEMENT_DYNAMIC(CDlgSetAnchorList, CDialogEx)

CDlgSetAnchorList::CDlgSetAnchorList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetAnchorList::IDD, pParent)
	, m_strAnchorMac(_T(""))
	, m_nAnchorIndex(0)
{

}

CDlgSetAnchorList::~CDlgSetAnchorList()
{
}

void CDlgSetAnchorList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_ANCHOR_ID, m_cmbAnchorId);
	DDX_Text(pDX, IDC_EDIT_ANCHOR_MAC, m_strAnchorMac);
	DDV_MaxChars(pDX, m_strAnchorMac, 4);
	DDX_CBIndex(pDX, IDC_CMB_ANCHOR_ID, m_nAnchorIndex);
}


BEGIN_MESSAGE_MAP(CDlgSetAnchorList, CDialogEx)
	ON_BN_CLICKED(ID_BTN_SET_ANCHOR_LIST, &CDlgSetAnchorList::OnBnClickedBtnSetAnchorList)
END_MESSAGE_MAP()


// CDlgSetAnchorList message handlers


BOOL CDlgSetAnchorList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (0 == m_cmbAnchorId.GetCount())
	{
		CString strId;
		for (int i = 0; i < 80; i++)
		{
			strId.Format(_T("%d"), i);
			m_cmbAnchorId.AddString(strId);
		}
	}

	if (m_nAnchorIndex >= m_cmbAnchorId.GetCount())
	{
		m_nAnchorIndex = 0;
	}
	
	m_cmbAnchorId.SetCurSel(m_nAnchorIndex);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSetAnchorList::OnBnClickedBtnSetAnchorList()
{
	UpdateData();
	m_uAnchorId = GetDlgItemInt(IDC_CMB_ANCHOR_ID);
	m_nAnchorIndex++;
	UpdateData(FALSE);
	OnOK();
}
