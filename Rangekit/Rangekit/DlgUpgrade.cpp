// DlgUpgrade.cpp : implementation file
//

#include "stdafx.h"
#include "Rangekit.h"
#include "DlgUpgrade.h"
#include "afxdialogex.h"


// CDlgUpgrade dialog

IMPLEMENT_DYNAMIC(CDlgUpgrade, CDialogEx)

CDlgUpgrade::CDlgUpgrade(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_UPGRADE, pParent)
{

}

CDlgUpgrade::~CDlgUpgrade()
{
}

void CDlgUpgrade::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgUpgrade, CDialogEx)
END_MESSAGE_MAP()


// CDlgUpgrade message handlers


BOOL CDlgUpgrade::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		pSysMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CDlgUpgrade::PreTranslateMessage(MSG* pMsg)
{
	int key;
	if ((WM_KEYUP == pMsg->message) || (WM_KEYDOWN == pMsg->message))
	{
		key = pMsg->wParam;
		if (VK_ESCAPE == key 
		 || VK_SPACE == key
		 || VK_RETURN == key)
		{
			return TRUE;
		}
	}

	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
