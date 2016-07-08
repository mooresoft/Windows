
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
DWORD __stdcall HandleRxThrd(LPVOID pParam)
{
	CVisualcustomDlg* pDlg = NULL;

	if (NULL != pParam)
	{
		pDlg = (CVisualcustomDlg*)pParam;
		while (TRUE == pDlg->m_bOpenPort)
		{			
			pDlg->HandleRx();
			Sleep(1);
		}
	}

	return 0;
}


DWORD __stdcall UpgradeThrd(LPVOID pParam)
{
	CVisualcustomDlg* pDlg = NULL;

	if (NULL != pParam)
	{
		pDlg = (CVisualcustomDlg*)pParam;
		pDlg->UpgradeDevice();
	}

	return 0;
}

CVisualcustomDlg::CVisualcustomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVisualcustomDlg::IDD, pParent)
	, m_strdevid(_T("1000"))
	, m_usleeptime(200)
	, m_uAntennaDelay(16495)
	, m_bSysType(FALSE)
	, m_strDevId(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bOpenPort = FALSE;
    m_hHandleRxThrd = NULL;
	m_hUpgradeThrd  = NULL;
	InitializeCriticalSectionAndSpinCount(&m_csRecvSerail, 400);
	m_brushEditbk = CreateSolidBrush(RGB(0,0,0)); 
	m_uRfModeIdx = 1;
	m_bStartRecv = FALSE;
	m_pbin       = NULL;
	m_binsize    = 0;
	m_bUpgrade   = FALSE;
}

CVisualcustomDlg::~CVisualcustomDlg()
{
	DeleteCriticalSection(&m_csRecvSerail);
	if (NULL != m_pbin)
	{
		delete [] m_pbin;
		m_pbin = NULL;
	}
}

void CVisualcustomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DEVICE_ID, m_strdevid);
	DDV_MaxChars(pDX, m_strdevid, 4);
	DDX_Text(pDX, IDC_EDIT_SLEEP_TIME, m_usleeptime);
	DDV_MinMaxUInt(pDX, m_usleeptime, 1, 100000);
	DDX_Control(pDX, IDC_COMBO_RF_MODE, m_cmbRfMode);
	DDX_Text(pDX, IDC_EDIT_ANTENNA_DELAY, m_uAntennaDelay);
	DDV_MinMaxUInt(pDX, m_uAntennaDelay, 16400, 16600);
	DDX_Control(pDX, IDC_LIST_ANCHORS, m_listanchors);
	DDX_Radio(pDX, IDC_RADIO_TDOA, m_bSysType);
	DDX_Control(pDX, IDC_LIST_DEVICE, m_lstDevice);
	DDX_Text(pDX, IDC_EDIT_DEV_ID, m_strDevId);
	DDV_MaxChars(pDX, m_strDevId, 4);
}

BEGIN_MESSAGE_MAP(CVisualcustomDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_COMM_RXCHAR, OnRecvSerial)
	ON_BN_CLICKED(IDC_BTN_OPEN_PORT, &CVisualcustomDlg::OnBnClickedBtnOpenPort)
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_BTN_SEARCH, &CVisualcustomDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(ID_BTN_READ, &CVisualcustomDlg::OnBnClickedBtnRead)
	ON_BN_CLICKED(IDC_BTN_REBOOT, &CVisualcustomDlg::OnBnClickedBtnReboot)
	ON_BN_CLICKED(IDC_BTN_RESET, &CVisualcustomDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_UPGRADE, &CVisualcustomDlg::OnBnClickedBtnUpgrade)
	ON_BN_CLICKED(IDC_BTN_POWER, &CVisualcustomDlg::OnBnClickedBtnPower)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CVisualcustomDlg::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_RADIO_TDOA, &CVisualcustomDlg::OnBnClickedRadioTdoa)
	ON_BN_CLICKED(IDC_RADIO_TWR, &CVisualcustomDlg::OnBnClickedRadioTwr)
	ON_BN_CLICKED(IDC_BTN_ENTERCLI, &CVisualcustomDlg::OnBnClickedBtnEntercli)
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
	InitDevList();
	InitAnchorList();
	GetDlgItem(IDC_LIST_ANCHORS)->EnableWindow(m_bSysType);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVisualcustomDlg::InitDevList(void)
{
	LONG lStyle;
	lStyle = GetWindowLong(m_lstDevice.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_LIST; //设置style
	SetWindowLong(m_lstDevice.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_lstDevice.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	m_lstDevice.SetExtendedStyle(dwStyle); //设置扩展风格

	m_lstDevice.InsertColumn( 0, _T("Index"), LVCFMT_LEFT, 40 );//插入列
	m_lstDevice.InsertColumn( 1, _T("TAG"), LVCFMT_LEFT, 50 );
	
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
		if (TRUE == m_serial.InitPort(this, m_dlgOpenSerial.m_nPort, m_dlgOpenSerial.m_nBoudRate))
		{
			m_bOpenPort		= TRUE;
			m_hHandleRxThrd	= CreateThread(NULL, 0, HandleRxThrd, this, 0, NULL);
			m_serial.StartMonitor();

			strWindowText.Format(_T("Air Custom - COM%d"), m_dlgOpenSerial.m_nPort);
			SetWindowText(strWindowText);

			strWindowText.Format(_T("COM%d is connect"), m_dlgOpenSerial.m_nPort);
			SetDlgItemText(IDC_OPEN_PORT_TIPS, strWindowText);

			GetDlgItem(ID_BTN_SEARCH)->EnableWindow(TRUE);
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
	m_bOpenPort = FALSE;
	
	GetDlgItem(ID_BTN_SEARCH)->EnableWindow(FALSE);
	CString strWindowText;
	strWindowText.Format(_T("COM%d is not connect"), m_dlgOpenSerial.m_nPort);
	SetDlgItemText(IDC_OPEN_PORT_TIPS, strWindowText);
	if (NULL != m_hHandleRxThrd)
	{
		WaitForSingleObject(m_hHandleRxThrd, INFINITE);
		CloseHandle(m_hHandleRxThrd);
		m_hHandleRxThrd = NULL;
	}

	if (NULL != m_hUpgradeThrd)
	{
		WaitForSingleObject(m_hUpgradeThrd, INFINITE);
		CloseHandle(m_hUpgradeThrd);
		m_hUpgradeThrd = NULL;
	}
	m_serial.UninitPort();
	return;
}

void CVisualcustomDlg::OnClose()
{
	CloseSerial();
	CDialogEx::OnClose();
}

void CVisualcustomDlg::HandleRx(void)
{
	CString strCmd;
	string str;
	unsigned char cmd_len = 0;
	unsigned char cmd_type;
	if (false == m_qsCmd.empty())
	{
		EnterCriticalSection(&m_csRecvSerail);
		str = m_qsCmd.front();
		m_qsCmd.pop();
		LeaveCriticalSection(&m_csRecvSerail);
		
		EnableButtons();

		strCmd = str.c_str();
		if (TRUE == m_bUpgrade && strCmd.GetLength() > 1)
		{
			if (-1 != strCmd.Find(_T("OK")))
			{
				m_nUpgradeAck = OTA_SUC;
				strCmd = ".";
			}
			else if (-1 != strCmd.Find(_T("seq err")))
			{
				m_nUpgradeAck = OTA_SEQ_ERR;
			}
			else if (-1 != strCmd.Find(_T("write flash err")))
			{
				m_nUpgradeAck = WRITE_FLASH_ERR;
			}
			else if (-1 != strCmd.Find(_T("data err")))
			{
				m_nUpgradeAck = DATA_ERR;
			}
		}
		
		((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_INFO))->SetSel(-1);
		((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_INFO))->ReplaceSel(strCmd);
	}
}


void CVisualcustomDlg::AddMgmtInfo(CString strInfo)
{
	if (TRUE == m_bOpenPort)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_INFO))->SetSel(-1);
		((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_INFO))->ReplaceSel(strInfo + _T("\n"));
	}
}

void CVisualcustomDlg::AddDevList(void)
{
	CString strdev;
	strdev.Format(_T("%02x%02x"), m_usartconf.m_devParam.host_mac[1], m_usartconf.m_devParam.host_mac[0]);
	m_lstDevice.InsertItem(m_lstDevice.GetItemCount(), strdev);
	memcpy(m_dstmac, m_usartconf.m_devParam.host_mac, 2);
}

LRESULT CVisualcustomDlg::OnRecvSerial(WPARAM wParam, LPARAM lParam)
{
	char RecvChar = (char)wParam;

	if('\r' == RecvChar || '\n' == RecvChar)
	{
		if (m_sCmd.size() > 2)
		{
			m_sCmd += '\n';
		}
		EnterCriticalSection(&m_csRecvSerail);
		m_qsCmd.push(m_sCmd);
		LeaveCriticalSection(&m_csRecvSerail);		
		m_sCmd.clear();
	}
	else
	{
		m_sCmd += RecvChar;
	}
	return 0;

}

void CVisualcustomDlg::OnBnClickedBtnSearch()
{
	unsigned char buf[USART_BUF_SIZE] = {0};
	unsigned char len = 0;
	m_lstDevice.DeleteAllItems();

	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_INFO))->SetSel(0, -1);
	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_INFO))->Clear();	

	len = m_usartconf.QueryDevice(buf);
	m_serial.WriteSerial(buf, len);
	AddMgmtInfo(_T("Search Device"));
}

void CVisualcustomDlg::OnBnClickedBtnRead()
{
	unsigned char buf[USART_BUF_SIZE] = {0};
	unsigned char len = 0;
	len = m_usartconf.ReadDevice(buf);

	m_serial.WriteSerial(buf, len);
	AddMgmtInfo(_T("Read Device"));
}


void CVisualcustomDlg::OnBnClickedBtnReboot()
{
	unsigned char buf[USART_BUF_SIZE] = {0};
	unsigned char len = 0;
	len = m_usartconf.RebootDevice(buf);

	m_serial.WriteSerial(buf, len);
	AddMgmtInfo(_T("Reboot Device"));
}


void CVisualcustomDlg::OnBnClickedBtnReset()
{
	unsigned char buf[USART_BUF_SIZE] = {0};
	unsigned char len = 0;
	len = m_usartconf.ResetDevice(buf);

	m_serial.WriteSerial(buf, len);
	AddMgmtInfo(_T("Reset Device"));
}

void CVisualcustomDlg::readupfile(void)
{
	if (NULL != m_pbin)
	{
		delete [] m_pbin;
	}

	try 
	{
		CFile file(m_strUpgradeFile, CFile::modeRead);
		m_binsize = file.GetLength();
		m_pbin = new BYTE[m_binsize];
		file.Read(m_pbin, m_binsize);
		file.Close();
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
}

void CVisualcustomDlg::openupfile(void)
{
	CString defaultDir     = _T("E:\\");           
	CString filter         = _T("File (*.bin)|*.bin||");
	TCHAR   path[MAX_PATH] = { 0 };
	CString fileName; 

	CFileDialog openFileDlg(TRUE, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);

	if (m_strUpgradeFile.IsEmpty())
	{
		GetCurrentDirectory(MAX_PATH, path);
		openFileDlg.GetOFN().lpstrInitialDir = path;
	}
	else
	{
		openFileDlg.GetOFN().lpstrInitialDir = m_strUpgradeFile;
	}

	if (TRUE == m_strUpgradeFile.IsEmpty())
	{
		if (IDOK == openFileDlg.DoModal())
		{
			m_strUpgradeFile = openFileDlg.GetPathName();
			m_strUpFileName = openFileDlg.GetFileName();

			readupfile();			
		}
	}	
}

void CVisualcustomDlg::OnBnClickedBtnUpgrade()
{
	openupfile();
	if (NULL == m_pbin)
	{
		return;
	}
	
	unsigned char buf[USART_BUF_SIZE] = {0};
	unsigned char len = 0;

	
	AddMgmtInfo(_T("Upgrade Device"));
	m_sentbinsize = 0;
	len = m_usartconf.PreUpDevice(buf);
	m_serial.WriteSerial(buf, len);
	Sleep(2000);
	m_bUpgrade = TRUE;
	
	m_hUpgradeThrd	= CreateThread(NULL, 0, UpgradeThrd, this, 0, NULL);
	m_sentseq  = 0;
}

void CVisualcustomDlg::UpgradeDevice(void)
{
	unsigned char buf[USART_BUF_SIZE] = {0};
	unsigned char buflen = 0;
	unsigned int  sendlen = 0;
	unsigned char msgtype  = 0;
	unsigned char up_count = 0;
	unsigned char wait_cout = 0;
	unsigned char erase_flash = 1;

	m_sentseq = 0;
	
	while (TRUE == m_bOpenPort)
	{
		if (m_binsize - m_sentbinsize > WRITE_SIZE)
		{
			sendlen = WRITE_SIZE;
		}
		else
		{
			sendlen = m_binsize - m_sentbinsize;
		}

		if (0 == m_sentbinsize)
		{
			msgtype = 0;
		}
		else if (m_binsize - m_sentbinsize > WRITE_SIZE)
		{
			msgtype = 1;
		}
		else if (m_binsize > m_sentbinsize)
		{
			msgtype = 2;
		}
		else
		{
			AddMgmtInfo(_T("Upgrade Device Success"));
            break;
		}

		up_count = 0;
		while (3 > up_count++)
		{
			m_nUpgradeAck = OTA_UNKNOWN;
			buflen = m_usartconf.UpgradeDevice(buf, m_sentseq, msgtype, m_pbin + m_sentbinsize, sendlen);
			m_serial.WriteSerial(buf, buflen);
			if (1 == erase_flash)
			{
				erase_flash = 0;
				Sleep(1000);
			}
			if (3 > m_sentseq)
		    {
				wait_cout = 400;				
				while (0 < wait_cout-- && OTA_UNKNOWN == m_nUpgradeAck)
				{
					Sleep(5);
				}				
			}
			else
			{
				wait_cout = 50;
				while (0 < wait_cout-- && OTA_UNKNOWN == m_nUpgradeAck)
				{
					Sleep(1);
				}
			}

			if (OTA_SUC == m_nUpgradeAck)
			{
				m_sentbinsize += sendlen;
				m_sentseq++;
				break;
			}
			else if (OTA_SEQ_ERR == m_nUpgradeAck)
			{
				m_sentbinsize = 0;
				m_sentseq = 0;
				break;
			}
		}

		if (OTA_SUC != m_nUpgradeAck)
		{
			if (OTA_UNKNOWN == m_nUpgradeAck)
			{
				AddMgmtInfo(_T("Upgrade Device no ack"));
			}
			break;
		}

	}
	m_bUpgrade = FALSE;
}

void CVisualcustomDlg::OnBnClickedBtnPower()
{
	unsigned char buf[USART_BUF_SIZE] = {0};
	unsigned char len = 0;
	len = m_usartconf.PowerTest(buf);

	m_serial.WriteSerial(buf, len);
	AddMgmtInfo(_T("Power Test Device"));
}

int str_to_mac(char *macaddr_str,unsigned char* macaddr,unsigned int macaddr_len)
{
	int i = 0;
	int spn = 0;
	int dn = 0;
	unsigned char byte = 0;
	unsigned char tmp = 0;

	if(NULL==macaddr_str||0==strlen(macaddr_str)||NULL==macaddr||0==macaddr_len)
	{
		return -1;
	}

	for (i=strlen(macaddr_str); i!=0; i-- )
	{
		macaddr_str[i-1]=toupper(macaddr_str[i-1]);
		if(isxdigit(macaddr_str[i-1]))
		{
			dn++;
			tmp = macaddr_str[i-1]>='A' ? macaddr_str[i-1]-'A'+10 : macaddr_str[i-1]-'0';
			//byte = (byte) + (tmp<<4);
			if (dn == 1)
			{
				byte = tmp;
			}
			else if (dn == 2)
			{
				byte = byte + (tmp<<4);
			}     
		}
		else 
		{
			break;
		}
		if (((i-1) % 2) == 0)
		{
			if ( dn==1 || dn==2 )
			{
				if(spn<macaddr_len)
				{					
					macaddr[spn++] = byte;
					byte = 0;
					dn=0;
				}
				else
				{
					break;
				}
			}
		}
	}
	if ( spn != 8 && spn != 2)
	{
		return -1;
	}

	return 0;
}

void CString2Mac(CString strMac, unsigned char* macaddr, unsigned int mac_len)
{
	char szMac[5] = {0};
	theApp.CStringToChar(strMac, szMac, 5);
	str_to_mac(szMac, macaddr, mac_len);
}

void CVisualcustomDlg::OnBnClickedBtnApply()
{
	unsigned char buf[USART_BUF_SIZE] = {0};
	unsigned char len = 0;

	memset(&m_applyparam, 0, sizeof(m_applyparam));
	
	UpdateData();
	m_applyparam.sys_type = m_bSysType + 1;

	CString2Mac(m_strdevid, m_applyparam.host_mac, 2);

	CString strRfMode;
	char szRfMode;
	GetDlgItemText(IDC_COMBO_RF_MODE, strRfMode);
	theApp.CStringToChar(strRfMode, &szRfMode, 1);
	m_applyparam.rf_mode = atoi(&szRfMode);

	m_applyparam.sleep_time = m_usleeptime;
	m_applyparam.antenna_delay = m_uAntennaDelay;

	if (TRUE == m_bSysType)
	{
		TCHAR szAnchor[5] = {0};
		CString strAnchor;

		for (int i = 0; i < 10; i++)
		{
			m_listanchors.GetItemText(i, 1, szAnchor, sizeof(szAnchor));
			strAnchor = szAnchor;
			CString2Mac(strAnchor, &m_applyparam.anchors_mac[i * 2], 2);
		}
	}

	len = m_usartconf.ConfDevice(buf, m_dstmac, m_applyparam);

	m_serial.WriteSerial(buf, len);
}

void CVisualcustomDlg::EnableButtons(BOOL enabled)
{
	GetDlgItem(ID_BTN_READ)->EnableWindow(enabled);
	GetDlgItem(IDC_BTN_REBOOT)->EnableWindow(enabled);
	GetDlgItem(IDC_BTN_RESET)->EnableWindow(enabled);
	GetDlgItem(IDC_BTN_POWER)->EnableWindow(enabled);
	GetDlgItem(IDC_BTN_UPGRADE)->EnableWindow(enabled);
}

void CVisualcustomDlg::OnBnClickedRadioTdoa()
{
	m_bSysType = FALSE;
	GetDlgItem(IDC_LIST_ANCHORS)->EnableWindow(m_bSysType);
}


void CVisualcustomDlg::OnBnClickedRadioTwr()
{
	m_bSysType = TRUE;
	GetDlgItem(IDC_LIST_ANCHORS)->EnableWindow(m_bSysType);
}


void CVisualcustomDlg::OnBnClickedBtnEntercli()
{
	//EnterCli
}
