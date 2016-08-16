
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "TopologyX.h"
#include "TopologyXDoc.h"
#include "MainFrm.h"

#include "TopologyXView.h"
#include "DevList.h"
#include "TopoSouth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define USART_BUF_SIZE  255
#define IDT_EMULATE	1001
#define IDT_UPDATE	1002
#define WM_MY_UPDATE (WM_USER+100)

// r为输入的距离矩阵，按（1）的原则存储
// 0行（列）为原点基站行（列），1行（列）为指定了北向方位角的基站
// dim为基站个数（包括原点基站）
// a1为指定的北向方位角
// pout为输出坐标数组
int CalculatPosition(double *r, unsigned int dim, double a1, HPVEC2D *pout)
{
	unsigned int i,j,k;

	//根据距离和北向角直接得到第一个基站的坐标	
	double x1,y1;
	x1 = MAT_ITEM(r,0,1) * sin(a1);
	y1 = MAT_ITEM(r,0,1) * cos(a1);

	//根据余弦定理得到第二个基站的北向角
	//这里取反余弦时，取正号的角度；
	//反余弦的负号角度，同样是解——需要指定，由两个角算出来的拓扑，将是对于直线A0A1左右对称的
	double a2 = a1 + acos((MAT_ITEM(r,0,1) * MAT_ITEM(r,0,1) + MAT_ITEM(r,0,2)*MAT_ITEM(r,0,2) - MAT_ITEM(r,1,2)*MAT_ITEM(r,1,2))/(2*MAT_ITEM(r,0,1)*MAT_ITEM(r,0,2)));

	//由距离和北向角得到第二个基站的坐标
	double x2,y2;
	x2 = MAT_ITEM(r,0,2) * sin(a2);
	y2 = MAT_ITEM(r,0,2) * cos(a2);

	pout[0].x=0;pout[0].y=0;
	pout[1].x=x1;pout[1].y=y1;
	pout[2].x=x2;pout[2].y=y2;

	//由已知的3个基站的坐标，以及其他基站与他们的距离，计算得到其他基站的坐标

	//系数矩阵A
	double AM[3][2];
	AM[0][0] = 2*(x1);
	AM[0][1] = 2*(y1);
	AM[1][0] = 2*(x2);
	AM[1][1] = 2*(y2);

	// A ^ T(A)
	double MatrixA[2][2];
	MatrixA[0][0]=AM[0][0]*AM[0][0]+AM[1][0]*AM[1][0];
	MatrixA[0][1]=AM[0][0]*AM[0][1]+AM[1][0]*AM[1][1];
	MatrixA[1][0]=AM[0][1]*AM[0][0]+AM[1][1]*AM[1][0];
	MatrixA[1][1]=AM[0][1]*AM[0][1]+AM[1][1]*AM[1][1];

	double Inv[2][2];
	double Div=MatrixA[0][0]*MatrixA[1][1]-MatrixA[0][1]*MatrixA[1][0];

	if (fabs(Div) < 1e-9)
	{
		// 2号基站选择不合理，在0、1连线上？
		return -3;
	}

	Inv[0][0]=MatrixA[1][1]/Div;
	Inv[0][1]=-MatrixA[0][1]/Div;
	Inv[1][0]=-MatrixA[1][0]/Div;
	Inv[1][1]=MatrixA[0][0]/Div;

	double E[2][2];
	for(i=0;i<2;i++)
	{
		for(j=0;j<2;j++)
		{
			E[i][j]=Inv[i][0]*AM[j][0]+Inv[i][1]*AM[j][1];
		}
	}

	double b[2];
	for(k=3;k<dim;k++)
	{
		b[0] = MAT_ITEM(r,0,k)*MAT_ITEM(r,0,k) - MAT_ITEM(r,1,k)*MAT_ITEM(r,1,k) + MAT_ITEM(r,0,1)*MAT_ITEM(r,0,1);
		b[1] = MAT_ITEM(r,0,k)*MAT_ITEM(r,0,k) - MAT_ITEM(r,2,k)*MAT_ITEM(r,2,k) + MAT_ITEM(r,0,2)*MAT_ITEM(r,0,2);

		pout[k].x = E[0][0]*b[0] + E[0][1]*b[1];
		pout[k].y = E[1][0]*b[0] + E[1][1]*b[1];

	}

	return dim;

}

// CMainFrame

DWORD __stdcall ReadThrd(LPVOID pParam)
{
	CMainFrame* pMainFrame = NULL;

	if (NULL != pParam)
	{
		pMainFrame = (CMainFrame*)pParam;
		while (TRUE == pMainFrame->m_bRead)
		{			
			pMainFrame->PostMessage(WM_MY_UPDATE, 0, 0);
//			pMainFrame->south_parse_msg();
			Sleep(1);
		}
	}

	return 0;
}

/////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
	ON_COMMAND(IDR_EMULATE, &CMainFrame::OnEmulate)
	ON_MESSAGE(WM_COMM_RXCHAR, OnRecvSerial)
	ON_MESSAGE(WM_MY_UPDATE, OnMyUpdate)
	ON_WM_TIMER()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	m_bRead = FALSE;
	m_hReadThrd = NULL;
	InitializeCriticalSection(&m_csRecvSerail);
	srand((int)time(0));
}

CMainFrame::~CMainFrame()
{
	DeleteCriticalSection(&m_csRecvSerail);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndSplitter.CreateStatic(this, 2, 1))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CTopologyXView), CSize(200, 700), pContext) ||
		!m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CDevList), CSize(100, 200), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	//m_wndSplitter.EnableWindow(FALSE);
	return TRUE;
//	return CFrameWndEx::OnCreateClient(lpcs, pContext);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnFileOpen()
{
	if (IDOK == m_dlgOpenSerial.DoModal())
	{
		CString strWindowText;
		OnSerialClose();
		Sleep(10);
		if (TRUE == m_serial.InitPort(this, m_dlgOpenSerial.m_nPort, m_dlgOpenSerial.m_nBoudRate))
		{
			KillTimer(IDT_EMULATE);

			m_bRead		= TRUE;
			m_hReadThrd	= CreateThread(NULL, 0, ReadThrd, this, 0, NULL);
	//		theApp.m_pMainFrame = this;
			m_serial.StartMonitor();
			//m_StartTime = CTime::GetCurrentTime();

			strWindowText.Format(_T("Visual Distance - COM%d"), m_dlgOpenSerial.m_nPort);
			SetWindowText(strWindowText);
		}
		else
		{
			MessageBox(_T("Open Serial Port Fail!"));
		}
	}
}


void CMainFrame::OnSerialClose()
{
	m_serial.UninitPort();
	m_bRead = FALSE;
	Sleep(20);
	if (NULL != m_hReadThrd)
	{
		WaitForSingleObject(m_hReadThrd, INFINITE);
		CloseHandle(m_hReadThrd);
		m_hReadThrd = NULL;
	}
	return;
}


unsigned char recv_start2 = 0;
unsigned char recv_size2   = 0;
LRESULT CMainFrame::OnRecvSerial(WPARAM wParam, LPARAM lParam)
{
	unsigned char RecvChar = (unsigned char)wParam;

/*
	if (0xAA == RecvChar)
	{
		recv_size2 = 0;
		recv_start2 = 1;
	}
	else if(RecvChar == 0x55)
	{
		if (recv_size2 < USART_BUF_SIZE)
		{
			EnterCriticalSection(&m_csRecvSerail);
			m_qsCmd.push_back(m_sCmd);
			LeaveCriticalSection(&m_csRecvSerail);		
			m_sCmd.clear();
		}

		recv_start2 = 0;
		recv_size2 = 0;
	}
	else if (1 == recv_start2)
	{
		if (recv_size2 >= USART_BUF_SIZE)
		{
			recv_size2 = 0;
			recv_start2 = 0;
		}
		else
		{
			m_sCmd += RecvChar;
		}
	}
*/

	if('\n' == RecvChar)
	{
		if (m_sCmd.size() > 2)
		{
			m_sCmd += '\n';
		}
		EnterCriticalSection(&m_csRecvSerail);
		m_qsCmd.push_back(m_sCmd);
		LeaveCriticalSection(&m_csRecvSerail);		
		m_sCmd.clear();
	}
	else
	{
		m_sCmd += RecvChar;
	}


	return 0;
}



void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	OnSerialClose();
	CFrameWnd::OnClose();
}

void CMainFrame::south_parse_msg()
{
	CString strCmd;
	string str;
	unsigned char cmd_len = 0;
	unsigned char cmd_buf[USART_BUF_SIZE]  = {0};
	unsigned char* pCmd = cmd_buf;
	unsigned char bcc;
	unsigned char chk_bcc;

	if (false == m_qsCmd.empty())
	{
		EnterCriticalSection(&m_csRecvSerail);
		str = m_qsCmd.front();
		m_qsCmd.pop_front();
		LeaveCriticalSection(&m_csRecvSerail);	

		CTopologyXDoc *pDoc = (CTopologyXDoc *)(GetActiveView()->GetDocument());
		//更新设备信息
		std::map<UINT64, CTopoDev> *pdevs = pDoc->GetDevMap();
		int id, x, y;
		UINT64 devid;
		if(3==sscanf_s(str.c_str(), "%x,%d,%d",&id,&x,&y))
		{
			wchar_t wbuf[32];
			time_t tmNow = time(NULL);
			devid = id;
			CHPVec2D pos;
			pos.x = (double)x/10.00f;
			pos.y = (double)y/10.00f;
			std::map<UINT64, CTopoDev>::iterator itrd;

			itrd = pdevs->find((UINT64)id);
			if(itrd != pdevs->end())
			{
				itrd->second.m_pos = pos;
				itrd->second.m_tmUpdate = tmNow;
			}
			else
			{
				CTopoDev dev;
				dev.m_id = devid;
				wsprintf(wbuf, L"%I64X", devid);
				dev.m_name = wbuf;
				dev.m_tmFound = tmNow;
				dev.m_tmUpdate = tmNow;
				dev.m_pos = pos;
				pdevs->insert(std::pair<UINT64,CTopoDev>(devid, dev));
			}

		}
		pDoc->UpdateAllViews(NULL);
/*

//		strCmd = str.c_str();
//		cmd_len = theApp.CStringToChar(strCmd, (CHAR*)cmd_buf, USART_BUF_SIZE);
		cmd_len = str.size();
		memcpy(pCmd, str.c_str(), cmd_len);
		cmd_len = south_decode_data(pCmd, cmd_len);
		bcc = pCmd[cmd_len - 1];
		chk_bcc = south_BCC_CheckSum(pCmd, cmd_len - 1);

		if (bcc == chk_bcc)
		{
			switch(pCmd[0])
			{
			case 0x09:
				calc_topology(&pCmd[1]);
				break;
			}
		}
*/

	}
}

void CMainFrame::calc_topology(unsigned char* pmsg)
{
	unsigned char len = pmsg[0];

	unsigned short id1;
	unsigned short id2;
	unsigned short range;
	unsigned short *psh = (unsigned short *)(pmsg+1);

	std::map<UINT, UINT> mapr;
	std::map<UINT, UINT> mapid;
	std::map<UINT, UINT>::iterator itr;
	UINT i,key;

	//获得 id1-id2：range映射
	for(i=0; i<len/6; i++)
	{
		// TODO: CONFIRM IF NEED ntohs
		id1 = psh[i*3];
		id2 = psh[i*3+1];
		range = psh[i*3+2];

		// 0距离为无效数据，跳过
		if(range == 0)
			continue;

		//获得 mapid
		itr=mapid.find((UINT)id1);
		if(itr==mapid.end())
		{
			mapid.insert(std::pair<UINT,UINT>((UINT)id1,(UINT)id1));
		}
		itr=mapid.find((UINT)id2);
		if(itr==mapid.end())
		{
			mapid.insert(std::pair<UINT,UINT>((UINT)id2,(UINT)id2));
		}
		//小ID在前
		key = (id1>id2?((((UINT)id1)&0xFFFF)|((((UINT)id2)<<16)&0xFFFF0000)):((((UINT)id2)&0xFFFF)|(((UINT)id1)<<16)&0xFFFF0000));
		mapr.insert(std::pair<UINT,UINT>(key, (UINT)range));
	}

	CTopologyXDoc *pDoc = (CTopologyXDoc *)(GetActiveView()->GetDocument());
	//得到id数组和range矩阵
	std::vector<UINT> *pvecids = pDoc->GetDevIDs();
	pvecids->clear();

	for(itr=mapid.begin(); itr!=mapid.end(); itr++)
	{
		pvecids->push_back(itr->first);
	}
	mapid.clear();


	//(&vecid[0]);
	UINT j,dev_num = pvecids->size();
	double *ranges = pDoc->GetRMatrix();
	HPVEC2D *posout = pDoc->GetPosArr();

	for(i=0; i<dev_num; i++)
	{
		id1 = pvecids->at(i);
		for(j=0; j<dev_num; j++)
		{
			id2 = pvecids->at(j);
			key = (id1>id2?((((UINT)id1)&0xFFFF)|((((UINT)id2)<<16)&0xFFFF0000)):((((UINT)id2)&0xFFFF)|(((UINT)id1)<<16)&0xFFFF0000));
			itr = mapr.find(key);
			if(itr!=mapr.end())
			{
				MAT_ITEM(ranges, i, j) = ((double)(itr->second)) / 10.00f;
			}
			else
			{
				MAT_ITEM(ranges, i, j) = -1.00f;
			}
		}
	}
	mapr.clear();

	//计算位置
	if(CalculatPosition(ranges, dev_num, 0.00f, posout) == dev_num)
	{
		//更新设备信息
		std::map<UINT64, CTopoDev> *pdevs = pDoc->GetDevMap();
		std::map<UINT64, CTopoDev>::iterator itrd;
		UINT64 devid;
		wchar_t wbuf[32];
		time_t tmNow = time(NULL);
		for(i=0; i<dev_num; i++)
		{
			devid = (UINT64)pvecids->at(i);
			itrd = pdevs->find(devid);
			if(itrd != pdevs->end())
			{
				itrd->second.m_pos = posout[i];
				itrd->second.m_tmUpdate = tmNow;
			}
			else
			{
				CTopoDev dev;
				dev.m_id = devid;
				wsprintf(wbuf, L"%I64X", devid);
				dev.m_name = wbuf;
				dev.m_tmFound = tmNow;
				dev.m_tmUpdate = tmNow;
				pdevs->insert(std::pair<UINT64,CTopoDev>(devid, dev));
			}
		}
	//	AfxGetMainWnd()->SendMessage(WM_MY_UPDATE, 0, 0);
		pDoc->UpdateAllViews(NULL);
	}
}

LRESULT CMainFrame::OnMyUpdate(WPARAM wParam,LPARAM lParam)
{
//	CTopologyXDoc *pDoc = (CTopologyXDoc *)(GetActiveView()->GetDocument());
//	pDoc->UpdateAllViews(NULL);
	south_parse_msg();

	return S_OK;
}

void CMainFrame::OnEmulate()
{
	if(m_bRead)
	{
		AfxMessageBox(L"请先关闭串口监听！");
		return;
	}

	int i,j;
	UINT16 aidstart=0x9000;
	for(i=0; i<MAX_DIM; i++)
	{
		m_emu_aids[i] = aidstart+i;
	}

	m_emu_pos[0]=CHPVec2D(0.00f,0.00f);
	m_emu_pos[1]=CHPVec2D(0.00f,10.00f);
	m_emu_pos[2]=CHPVec2D(10.00f,0.00f);
	m_emu_pos[3]=CHPVec2D(10.00f,10.00f);
	m_emu_pos[4]=CHPVec2D(5.00f,5.00f);

	SetTimer(IDT_EMULATE, 1000, NULL);
/*
	for(i=0;i<MAX_DIM; i++)
	{
		for(j=0;j<MAX_DIM;j++)
		{
			m_emu_r[i][j] = 0;
		}
	}

	int ran;	
	for(i=0; i<5; i++)
	{
		m_emu_r[i][i] = 0;
		for(j=i+1; j<5; j++)
		{
			ran=(int)(2.0*rand()/(RAND_MAX) - 1.0) + sqrt(m_emu_pos[i].DistanceSquare(m_emu_pos[j])*10);
			m_emu_r[i][j] = (UINT16)ran;
			m_emu_r[j][i] = (UINT16)ran;
		}
	}

	//构造距离数组
	uint8 *buf;
	m_sCmd.clear();

	int ret = Send_Ranges(5, m_emu_aids, m_emu_r, &buf);
	for(i=1; i<ret-1; i++)
	{
		m_sCmd.push_back((char)buf[i]);
	};
*/
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(IDT_EMULATE == nIDEvent)
	{
		int i,j;
		for(i=0;i<MAX_DIM; i++)
		{
			for(j=0;j<MAX_DIM;j++)
			{
				m_emu_r[i][j] = 0;
			}
		}

		int ran;	
		for(i=0; i<5; i++)
		{
			m_emu_r[i][i] = 0;
			for(j=i+1; j<5; j++)
			{
				ran=(int)(2.0*rand()/(RAND_MAX) - 1.0) + sqrt(m_emu_pos[i].DistanceSquare(m_emu_pos[j]))*10;
				m_emu_r[i][j] = (UINT16)ran;
				m_emu_r[j][i] = (UINT16)ran;
			}
		}

		//构造距离数组
		uint8 *buf;
		m_sCmd.clear();

		int ret = Send_Ranges(5, m_emu_aids, m_emu_r, &buf);
		for(i=1; i<ret-1; i++)
		{
			m_sCmd.push_back((char)buf[i]);
		}

		EnterCriticalSection(&m_csRecvSerail);
		m_qsCmd.push_back(m_sCmd);
		LeaveCriticalSection(&m_csRecvSerail);
		south_parse_msg();
	}
	

	CFrameWnd::OnTimer(nIDEvent);
}
