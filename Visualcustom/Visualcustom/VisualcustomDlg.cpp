
// VisualcustomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Visualcustom.h"
#include "VisualcustomDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVisualcustomDlg dialog
CVisualcustomDlg::CVisualcustomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVisualcustomDlg::IDD, pParent)
	, m_bSysType(TRUE)
	, m_strdevid(_T("1000"))
	, m_usleeptime(200)
	, m_uAntennaDelay(16495)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bOpenPort = FALSE;
	InitializeCriticalSectionAndSpinCount(&m_csRecvSerail, 400);
	m_brushEditbk = CreateSolidBrush(RGB(0,0,0)); 
	m_uRfModeIdx = 1;
}

CVisualcustomDlg::~CVisualcustomDlg()
{
	DeleteCriticalSection(&m_csRecvSerail);
}

void CVisualcustomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_TDOA, m_bSysType);
	DDX_Text(pDX, IDC_EDIT_DEVICE_ID, m_strdevid);
	DDV_MaxChars(pDX, m_strdevid, 4);
	DDX_Text(pDX, IDC_EDIT_SLEEP_TIME, m_usleeptime);
	DDV_MinMaxUInt(pDX, m_usleeptime, 1, 100000);
	DDX_Control(pDX, IDC_COMBO_RF_MODE, m_cmbRfMode);
	DDX_Text(pDX, IDC_EDIT_ANTENNA_DELAY, m_uAntennaDelay);
	DDV_MinMaxUInt(pDX, m_uAntennaDelay, 16400, 16600);
	DDX_Control(pDX, IDC_LIST_ANCHORS, m_listanchors);
}

BEGIN_MESSAGE_MAP(CVisualcustomDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_COMM_RXCHAR, OnRecvSerial)
	ON_BN_CLICKED(IDC_BTN_OPEN_PORT, &CVisualcustomDlg::OnBnClickedBtnOpenPort)
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_BTN_READ, &CVisualcustomDlg::OnBnClickedBtnRead)
	ON_BN_CLICKED(IDC_BTN_REBOOT, &CVisualcustomDlg::OnBnClickedBtnReboot)
	ON_BN_CLICKED(IDC_BTN_RESET, &CVisualcustomDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_UPGRADE, &CVisualcustomDlg::OnBnClickedBtnUpgrade)
	ON_BN_CLICKED(IDC_BTN_POWER, &CVisualcustomDlg::OnBnClickedBtnPower)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CVisualcustomDlg::OnBnClickedBtnApply)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPECTRUM, &CVisualcustomDlg::OnDeltaposSpinSpectrum)
END_MESSAGE_MAP()


// CVisualcustomDlg message handlers

BOOL CVisualcustomDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	OnBnClickedBtnOpenPort();
	m_cmbRfMode.SetCurSel(m_uRfModeIdx);
	InitAnchorList();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVisualcustomDlg::InitAnchorList(void)
{
	LONG lStyle;
	lStyle = GetWindowLong(m_listanchors.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_listanchors.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_listanchors.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_listanchors.SetExtendedStyle(dwStyle); //设置扩展风格

	m_listanchors.InsertColumn( 0, _T("Index"), LVCFMT_LEFT, 40 );//插入列
	m_listanchors.InsertColumn( 1, _T("Anchor"), LVCFMT_LEFT, 50 );

	int nRow;
	CString strIdx;
	for (int i = 0; i < 10; i++)
	{
		strIdx.Format(_T("%d"), i);
		nRow = m_listanchors.InsertItem(i, strIdx);//插入行
		m_listanchors.SetItemText(nRow, 1, _T("0000"));//设置数据
	}
	

}

void CVisualcustomDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVisualcustomDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVisualcustomDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



HBRUSH CVisualcustomDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO:  Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_OPEN_PORT_TIPS)
	{
		if (FALSE == m_bOpenPort)
		{
			SetTextColor(pDC->GetSafeHdc(), RGB(255, 0, 0));
		}
		else
		{
			SetTextColor(pDC->GetSafeHdc(), RGB(0, 255, 0));
		}
	}
	else if (CTLCOLOR_EDIT==nCtlColor && IDC_EDIT_DEVICE_INFO == pWnd->GetDlgCtrlID())
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkColor(RGB(0, 0, 0));
		return m_brushEditbk;
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CVisualcustomDlg::OnBnClickedBtnOpenPort()
{
	// TODO: Add your control notification handler code here
	CloseSerial();
	if (IDOK == m_dlgOpenSerial.DoModal())
	{
		CString strWindowText;
		Sleep(1000);
		if (TRUE == m_serial.InitPort(this, m_dlgOpenSerial.m_nPort, m_dlgOpenSerial.m_nBoudRate))
		{
			m_bOpenPort		= TRUE;
			m_serial.StartMonitor();

			strWindowText.Format(_T("Visual Custom - COM%d"), m_dlgOpenSerial.m_nPort);
			SetWindowText(strWindowText);

			strWindowText.Format(_T("COM%d is connect"), m_dlgOpenSerial.m_nPort);
			SetDlgItemText(IDC_OPEN_PORT_TIPS, strWindowText);
			EnableButtons(m_bOpenPort);
		}
		else
		{
			strWindowText.Format(_T("COM%d is not connect"), m_dlgOpenSerial.m_nPort);
			SetDlgItemText(IDC_OPEN_PORT_TIPS, strWindowText);
			MessageBox(_T("Open Serial Port Fail!"));
		}
	}
}

void CVisualcustomDlg::CloseSerial(void)
{
	m_serial.UninitPort();
	m_bOpenPort = FALSE;
	EnableButtons(m_bOpenPort);
	CString strWindowText;
	strWindowText.Format(_T("COM%d is not connect"), m_dlgOpenSerial.m_nPort);
	SetDlgItemText(IDC_OPEN_PORT_TIPS, strWindowText);
	return;
}

void CVisualcustomDlg::OnClose()
{
	CloseSerial();
	CDialogEx::OnClose();
}

LRESULT CVisualcustomDlg::OnRecvSerial(WPARAM wParam, LPARAM lParam)
{
	char RecvChar = (char)wParam;

	if('\r' == RecvChar || '\n' == RecvChar)
	{
		if (m_strcommand.GetLength() > 2)
		{
			m_strcommand += '\n';
		}
		EnterCriticalSection(&m_csRecvSerail);
		((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_INFO))->SetSel(-1);
		((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_INFO))->ReplaceSel(m_strcommand);
		LeaveCriticalSection(&m_csRecvSerail);		
		m_strcommand.Empty();
	}
	else
	{
		m_strcommand += RecvChar;
	}

	return 0;
}

void CVisualcustomDlg::OnBnClickedBtnRead()
{
	unsigned char buf[USART_BUF_SIZE] = {0};
	unsigned char len = 0;
	CString str = _T("Get system_type\r\n");

	len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);	
	m_serial.WriteSerial(buf, len);

	Sleep(20);
	str = _T("Get operator_mode\r\n");
	len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);	
	m_serial.WriteSerial(buf, len);

	Sleep(20);
	str = _T("Get mac\r\n");
	len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);	
	m_serial.WriteSerial(buf, len);

	Sleep(20);
	str = _T("Get sleep_time\r\n");
	len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);	
	m_serial.WriteSerial(buf, len);

	Sleep(20);
	str = _T("Get rf_mode\r\n");
	len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);	
	m_serial.WriteSerial(buf, len);

	Sleep(20);
	str = _T("Get antenna_delay\r\n");
	len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);	
	m_serial.WriteSerial(buf, len);

	Sleep(20);
	str = _T("AnchorListShow\r\n");
	len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);	
	m_serial.WriteSerial(buf, len);
}


void CVisualcustomDlg::OnBnClickedBtnReboot()
{
	unsigned char buf[USART_BUF_SIZE] = {0};
	unsigned char len = 0;

	CString str = _T("Reboot\r\n");

	len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);	

	m_serial.WriteToPort(buf, len);
}


void CVisualcustomDlg::OnBnClickedBtnReset()
{
	unsigned char buf[USART_BUF_SIZE] = {0};
	unsigned char len = 0;
	CString str = _T("Reset\r\n");
	len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);	
	m_serial.WriteSerial(buf, len);
}


void CVisualcustomDlg::OnBnClickedBtnUpgrade()
{
	unsigned char buf[USART_BUF_SIZE] = {0};
	unsigned char len = 0;
	len = m_usartconf.UpgradeDevice(buf);

	m_serial.WriteToPort(buf, len);
}


void CVisualcustomDlg::OnBnClickedBtnPower()
{
	unsigned char buf[USART_BUF_SIZE] = {0};
	unsigned char len = 0;
	CString str = _T("PowerTest\r\n");
	len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);	
	m_serial.WriteSerial(buf, len);
}


void CVisualcustomDlg::OnBnClickedBtnApply()
{
	unsigned char buf[USART_BUF_SIZE] = {0};
	unsigned char len = 0;
	CString str;
	UpdateData();
	str.Format(_T("Set system_type %d\r\n"), m_bSysType + 1);
	len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);	
	m_serial.WriteSerial(buf, len);
	Sleep(200);

	str.Format(_T("Set operator_mode 1\r\n"));
	len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);	
	m_serial.WriteSerial(buf, len);
	Sleep(200);

	str.Format(_T("Set mac %s\r\n"), m_strdevid);
	len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);	
	m_serial.WriteSerial(buf, len);
	Sleep(200);

	str.Format(_T("Set sleep_time %u\r\n"), m_usleeptime);
	len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);	
	m_serial.WriteSerial(buf, len);
	Sleep(200);

	CString strRfMode;
	GetDlgItemText(IDC_COMBO_RF_MODE, strRfMode);
	str.Format(_T("Set rf_mode %s\r\n"), strRfMode);
	len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);	
	m_serial.WriteSerial(buf, len);
	Sleep(200);

	str.Format(_T("Set antenna_delay %u\r\n"), m_uAntennaDelay);
	len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);	
	m_serial.WriteSerial(buf, len);
	Sleep(200);
	TCHAR szAnchor[5];
	int itemtxtlen = sizeof(szAnchor);
	for (int i = 0; i < 10; i++)
	{
		memset(szAnchor, 0, itemtxtlen);
		m_listanchors.GetItemText(i, 1, szAnchor, itemtxtlen);
		str.Format(_T("AnchorListSet %d %s\r\n"), i, szAnchor);
		len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);	
		m_serial.WriteSerial(buf, len);
		Sleep(200);
	}

	m_serial.WriteToPort(buf, len);
}

void CVisualcustomDlg::EnableButtons(BOOL enabled)
{
	GetDlgItem(ID_BTN_READ)->EnableWindow(enabled);
	GetDlgItem(IDC_BTN_REBOOT)->EnableWindow(enabled);
	GetDlgItem(IDC_BTN_RESET)->EnableWindow(enabled);
	GetDlgItem(IDC_BTN_POWER)->EnableWindow(enabled);
	//GetDlgItem(IDC_BTN_UPGRADE)->EnableWindow(enabled);
}

void CVisualcustomDlg::OnDeltaposSpinSpectrum(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	unsigned char buf[USART_BUF_SIZE] = { 0 };
	unsigned char len = 0;

	if (1 == pNMUpDown->iDelta) // 如果此值为1 , 说明点击了Spin的往下箭头 
	{		
		CString str = _T("TxPowerDown\r\n");
		len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);
		m_serial.WriteSerial(buf, len);
	}
	else if (-1 == pNMUpDown->iDelta) // 如果此值为-1 , 说明点击了Spin的往上箭头 
	{
		CString str = _T("TxPowerUp\r\n");
		len = theApp.CStringToChar(str, (char*)buf, USART_BUF_SIZE);
		m_serial.WriteSerial(buf, len);
	}

	*pResult = 0;
}
