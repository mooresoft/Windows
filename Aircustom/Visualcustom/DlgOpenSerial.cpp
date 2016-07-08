// DlgOpenSerial.cpp : implementation file
//

#include "stdafx.h"
#include "Visualcustom.h"
#include "DlgOpenSerial.h"
#include "afxdialogex.h"
#include <Dbt.h>

// CDlgOpenSerial dialog

IMPLEMENT_DYNAMIC(CDlgOpenSerial, CDialogEx)

CDlgOpenSerial::CDlgOpenSerial(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgOpenSerial::IDD, pParent)
{
	m_nBoudRateIdx = 0;
	m_nPort        = 0;
}

CDlgOpenSerial::~CDlgOpenSerial()
{
}

void CDlgOpenSerial::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_PORT, m_cmbPort);
	DDX_CBIndex(pDX, IDC_CMB_BOUDRATE, m_nBoudRateIdx);
	DDX_Control(pDX, IDC_CMB_BOUDRATE, m_cmbBoudRate);
}


BEGIN_MESSAGE_MAP(CDlgOpenSerial, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CDlgOpenSerial::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CDlgOpenSerial::OnBnClickedBtnRefresh)
	ON_CBN_SELCHANGE(IDC_CMB_PORT, &CDlgOpenSerial::OnCbnSelchangeCmbPort)
	ON_CBN_SELCHANGE(IDC_CMB_BOUDRATE, &CDlgOpenSerial::OnCbnSelchangeCmbBoudrate)
	ON_WM_CTLCOLOR()
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

	m_cmbBoudRate.SetCurSel(m_nBoudRateIdx);

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
	m_cmbPort.ResetContent();

	for (int i = 0; i < m_caPorts.GetCount(); i++)
	{
		m_cmbPort.AddString(m_caPorts[i]);
	}

	if (m_cmbPort.GetCount() > 0)
	{
		if (m_cmbPort.GetCurSel() < 0)
		{
			m_cmbPort.SetCurSel(0);
		}

		if (TRUE == bAutoOpen)
		{
			m_nCountDown = 5;
			SetTimer(1, 1000, NULL);
			CString strTips;
			strTips.Format(_T("We will open this COM after %d seconds."), m_nCountDown);
			SetDlgItemText(IDC_STATIC_OPEN_SERIAL_TIP, strTips);
		}

		GetDlgItem(IDC_BTN_OPEN)->EnableWindow(TRUE);
	}
	else
	{
		KillTimer(1);
		SetDlgItemText(IDC_STATIC_OPEN_SERIAL_TIP, _T(""));
	}

	return;
}

void CDlgOpenSerial::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (1 == nIDEvent)
	{
		CString strTips;
		strTips.Format(_T("We will open this COM after %d seconds."), --m_nCountDown);
		SetDlgItemText(IDC_STATIC_OPEN_SERIAL_TIP, strTips);
		if (m_nCountDown <= 0)
		{
			OnBnClickedBtnOpen();
			m_nCountDown = 5;
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CDlgOpenSerial::OnBnClickedBtnOpen()
{
	KillTimer(1);
	UpdateData();

	CString strPort;
	char    szPort[MAX_PATH] = { 0 };
	GetDlgItemText(IDC_CMB_PORT, strPort);
	if (-1 != strPort.Find(_T("COM")))
	{
		strPort.Replace(_T("COM"), _T(""));
	}

	theApp.CStringToChar(strPort, szPort, MAX_PATH);
	m_nPort = atoi(szPort);
	m_nBoudRate = GetDlgItemInt(IDC_CMB_BOUDRATE);
	OnOK();
}


void CDlgOpenSerial::OnBnClickedBtnRefresh()
{
	CStringArray caPorts;

	ReadPort(caPorts);
	if (TRUE == CheckPorts(caPorts))
	{
		SavePorts(caPorts);
		InitPortCmb();

	}

	caPorts.RemoveAll();

	return;
}


void CDlgOpenSerial::OnCbnSelchangeCmbPort()
{
	KillTimer(1);
	SetDlgItemText(IDC_STATIC_OPEN_SERIAL_TIP, _T(""));
}


void CDlgOpenSerial::OnCbnSelchangeCmbBoudrate()
{
	KillTimer(1);
	SetDlgItemText(IDC_STATIC_OPEN_SERIAL_TIP, _T(""));
}


LRESULT CDlgOpenSerial::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WM_DEVICECHANGE == message)
	{
		if (DBT_DEVICEARRIVAL == wParam)
		{
			TRACE("--------------------Plugin USB!!!!!!------------wParam=%d------lParam=%d--\n", wParam, lParam);
			InitPortCmb();
		}
		else if (DBT_DEVICEREMOVECOMPLETE == wParam)
		{
			TRACE("--------------------Remove USB!!!!!!------------wParam=%d------lParam=%d--\n", wParam, lParam);
			InitPortCmb();
		}
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}


HBRUSH CDlgOpenSerial::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_OPEN_SERIAL_TIP)
	{
		SetTextColor(pDC->GetSafeHdc(), RGB(255, 0, 0));
	}

	return hbr;
}
