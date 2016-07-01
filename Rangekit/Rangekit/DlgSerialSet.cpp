// DlgSerialSet.cpp : implementation file
//

#include "stdafx.h"
#include "rangekit.h"
#include "DlgSerialSet.h"
#include "afxdialogex.h"
#include <Dbt.h>
// CDlgSerialSet dialog

IMPLEMENT_DYNAMIC(CDlgSerialSet, CDialogEx)

CDlgSerialSet::CDlgSerialSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSerialSet::IDD, pParent)
	, m_strPortName(_T(""))
	, m_nBoudRate(115200)
	, m_nBoudRateIndex(0)
	, m_nStopBitsIdx(0)
{
	m_nCountDown = 5;
	m_caPorts.RemoveAll();
}

CDlgSerialSet::~CDlgSerialSet()
{
	m_caPorts.RemoveAll();
}

void CDlgSerialSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_PORT, m_cmbPort);
	DDX_Control(pDX, IDC_CMB_BOUDRATE, m_cmbBoudRate);
	DDX_CBString(pDX, IDC_CMB_PORT, m_strPortName);
	DDX_CBIndex(pDX, IDC_CMB_BOUDRATE, m_nBoudRateIndex);
	DDX_Control(pDX, IDC_CMB_STOPBITS, m_cmbStopBits);
	DDX_CBIndex(pDX, IDC_CMB_STOPBITS, m_nStopBitsIdx);
}


BEGIN_MESSAGE_MAP(CDlgSerialSet, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_BTN_OPEN_SERIAL, &CDlgSerialSet::OnBnClickedOpenSerial)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_CMB_PORT, &CDlgSerialSet::OnCbnSelchangeCmbPort)
	ON_CBN_SELCHANGE(IDC_CMB_BOUDRATE, &CDlgSerialSet::OnCbnSelchangeCmbBoudrate)
	ON_BN_CLICKED(ID_BTN_REFRESH, &CDlgSerialSet::OnBnClickedBtnRefresh)
END_MESSAGE_MAP()


// CDlgSerialSet message handlers


BOOL CDlgSerialSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CStringArray caPorts;
	ReadPort(caPorts);
	SavePorts(caPorts);
	caPorts.RemoveAll();

	InitPortCmb();

	m_cmbBoudRate.SetCurSel(m_nBoudRateIndex);
	m_cmbStopBits.SetCurSel(m_nStopBitsIdx);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgSerialSet::ReadPort(CStringArray& caPorts)
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

	LPCTSTR data_Set=_T("HARDWARE\\DEVICEMAP\\SERIALCOMM\\"); 

	caPorts.RemoveAll();
	bHavePort = FALSE;

	long ret0 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hKey); 

	if(ret0 == ERROR_SUCCESS) 
	{
		do 
		{
			dwName = sizeof(Name); 
			dwSizeofPortName = sizeof(szPortName); 

			Status = RegEnumValue(hKey, dwIndex++, Name, &dwName, NULL, &Type, szPortName, &dwSizeofPortName); 
			if((Status == ERROR_SUCCESS)||(Status == ERROR_MORE_DATA)) 
			{
				strPortName.Format(_T("%s"), szPortName);
				caPorts.Add(strPortName);
				bHavePort = TRUE;
			} 
		} while((Status == ERROR_SUCCESS)||(Status == ERROR_MORE_DATA)); 

		RegCloseKey(hKey); 
	}

	return bHavePort;
}

BOOL CDlgSerialSet::CheckPorts(CStringArray& caPorts)
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

void CDlgSerialSet::SavePorts(CStringArray& caPorts)
{
	m_caPorts.RemoveAll();
	for (int i = 0; i < caPorts.GetCount(); i++)
	{
		m_caPorts.Add(caPorts[i]);
	}

	return;
}

void CDlgSerialSet::InitPortCmb(BOOL bAutoOpen)
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
		
		GetDlgItem(ID_BTN_OPEN_SERIAL)->EnableWindow(TRUE);
	}
	else
	{
		KillTimer(1);
		SetDlgItemText(IDC_STATIC_OPEN_SERIAL_TIP, _T(""));
	}

	return;
}

void CDlgSerialSet::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (1 == nIDEvent)
	{
		CString strTips;
		strTips.Format(_T("We will open this COM after %d seconds."), --m_nCountDown);
		SetDlgItemText(IDC_STATIC_OPEN_SERIAL_TIP, strTips);
		if (m_nCountDown <= 0)
		{
			OnBnClickedOpenSerial();
			m_nCountDown = 5;
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CDlgSerialSet::OnBnClickedOpenSerial()
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
	UpdateData();
	m_nBoudRate = GetDlgItemInt(IDC_CMB_BOUDRATE);
	m_nStopBits = GetDlgItemInt(IDC_CMB_STOPBITS);
	OnOK();
}


HBRUSH CDlgSerialSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_OPEN_SERIAL_TIP)
	{
		SetTextColor(pDC->GetSafeHdc(),RGB(255,0,0));
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CDlgSerialSet::OnCbnSelchangeCmbPort()
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
	SetDlgItemText(IDC_STATIC_OPEN_SERIAL_TIP, _T(""));
}


void CDlgSerialSet::OnCbnSelchangeCmbBoudrate()
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
	SetDlgItemText(IDC_STATIC_OPEN_SERIAL_TIP, _T(""));
}


LRESULT CDlgSerialSet::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
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


BOOL CDlgSerialSet::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		
		if(VK_F5 == pMsg->wParam)
		{
			OnBnClickedBtnRefresh();
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgSerialSet::OnBnClickedBtnRefresh()
{
	// TODO: Add your control notification handler code here
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
