// DlgOpenSerial.cpp : implementation file
//

#include "stdafx.h"
#include "TopologyX.h"
#include "DlgOpenSerial.h"
#include "afxdialogex.h"
#include <Dbt.h>

// CDlgOpenSerial dialog

IMPLEMENT_DYNAMIC(CDlgOpenSerial, CDialogEx)

CDlgOpenSerial::CDlgOpenSerial(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgOpenSerial::IDD, pParent)
{

}

CDlgOpenSerial::~CDlgOpenSerial()
{
}

void CDlgOpenSerial::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_PORTS, m_cmbPorts);
}


BEGIN_MESSAGE_MAP(CDlgOpenSerial, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_REFRESH_PORT, &CDlgOpenSerial::OnBnClickedBtnRefreshPort)
	ON_BN_CLICKED(IDOK, &CDlgOpenSerial::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_CMB_PORTS, &CDlgOpenSerial::OnCbnSelchangeCmbPorts)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgOpenSerial message handlers
BOOL CDlgOpenSerial::OnInitDialog()
{
	CDialogEx::OnInitDialog();

    CStringArray caPorts;
	ReadPort(caPorts);
	SavePorts(caPorts);
	caPorts.RemoveAll();

	InitPortCmb();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgOpenSerial::ReadPort(CStringArray& caPorts)
{
	TCHAR Name[25];
	UCHAR szPortName[25];
	LONG Status;
	DWORD dwIndex = 0;
	DWORD dwName;
	DWORD dwSizeofPortName;
	DWORD Type;
	CString strPortName;
	HKEY hKey;
	BOOL bHavePort;

	LPCTSTR data_Set = _T("HARDWARE\\DEVICEMAP\\SERIALCOMM\\");

	caPorts.RemoveAll();
	bHavePort = FALSE;

	long ret0 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hKey);

	if (ret0 == ERROR_SUCCESS)
	{
		do
		{
			dwName = sizeof(Name);
			dwSizeofPortName = sizeof(szPortName);

			Status = RegEnumValue(hKey, dwIndex++, Name, &dwName, NULL, &Type, szPortName, &dwSizeofPortName);
			if ((Status == ERROR_SUCCESS) || (Status == ERROR_MORE_DATA))
			{
				strPortName.Format(_T("%s"), szPortName);
				caPorts.Add(strPortName);
				bHavePort = TRUE;
			}
		} while ((Status == ERROR_SUCCESS) || (Status == ERROR_MORE_DATA));

		RegCloseKey(hKey);
	}

	return bHavePort;
}

BOOL CDlgOpenSerial::CheckPorts(CStringArray& caPorts)
{
	BOOL bDevChange = FALSE;

	if (caPorts.GetCount() != m_caPorts.GetCount())
	{
		bDevChange = TRUE;
	}
	else
	{
		for (int i = 0; i < caPorts.GetCount(); i++)
		{
			if (0 != caPorts[i].CompareNoCase(m_caPorts[i]))
			{
				bDevChange = TRUE;
				break;
			}
		}
	}

	return bDevChange;
}

void CDlgOpenSerial::SavePorts(CStringArray& caPorts)
{
	m_caPorts.RemoveAll();
	for (int i = 0; i < caPorts.GetCount(); i++)
	{
		m_caPorts.Add(caPorts[i]);
	}

	return;
}

void CDlgOpenSerial::InitPortCmb(BOOL bAutoOpen)
{
	m_cmbPorts.ResetContent();

	for (int i = 0; i < m_caPorts.GetCount(); i++)
	{
		m_cmbPorts.AddString(m_caPorts[i]);
	}

	if (m_cmbPorts.GetCount() > 0)
	{
		if (m_cmbPorts.GetCurSel() < 0)
		{
			m_cmbPorts.SetCurSel(0);
		}

	}
	else
	{
		KillTimer(1);
	}

	return;
}


void CDlgOpenSerial::OnBnClickedBtnRefreshPort()
{
	CStringArray caPorts;

	ReadPort(caPorts);
	if (TRUE == CheckPorts(caPorts))
	{
		SavePorts(caPorts);
		InitPortCmb();

	}

	caPorts.RemoveAll();
}


void CDlgOpenSerial::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CString strPort;
	char    szPort[MAX_PATH] = { 0 };
	GetDlgItemText(IDC_CMB_PORTS, strPort);
	if (-1 != strPort.Find(_T("COM")))
	{
		strPort.Replace(_T("COM"), _T(""));
	}

	theApp.CStringToChar(strPort, szPort, MAX_PATH);
	m_nPort = atoi(szPort);
	m_nBoudRate = 115200;

	CDialogEx::OnOK();
}


void CDlgOpenSerial::OnCbnSelchangeCmbPorts()
{
	// TODO: Add your control notification handler code here
}





void CDlgOpenSerial::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnTimer(nIDEvent);
}
