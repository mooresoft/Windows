
// TopologyXView.cpp : CTopologyXView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "TopologyX.h"
#endif

#include "TopologyXDoc.h"
#include "TopologyXView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTopologyXView

IMPLEMENT_DYNCREATE(CTopologyXView, CView)

BEGIN_MESSAGE_MAP(CTopologyXView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_MAP_CTRL, OnMapCtrlMsg)
END_MESSAGE_MAP()

// CTopologyXView 构造/析构

CTopologyXView::CTopologyXView()
{
	// TODO: 在此处添加构造代码

	m_pMapCtrl = NULL;
	//	m_nRgnID = -1;
	m_nShowAnchor = 3;
}

CTopologyXView::~CTopologyXView()
{
}

BOOL CTopologyXView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTopologyXView 绘制

void CTopologyXView::OnDraw(CDC* /*pDC*/)
{
	CTopologyXDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if(NULL == m_pMapCtrl || FALSE == ::IsWindow(m_pMapCtrl->m_hWnd))
		return;

	m_pMapCtrl->Invalidate();
}


// CTopologyXView 诊断

#ifdef _DEBUG
void CTopologyXView::AssertValid() const
{
	CView::AssertValid();
}

void CTopologyXView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTopologyXDoc* CTopologyXView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTopologyXDoc)));
	return (CTopologyXDoc*)m_pDocument;
}
#endif //_DEBUG


// CTopologyXView 消息处理程序


int CTopologyXView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_bmAnchorOn.LoadBitmap(IDB_ANCHOR_ONLINE);
	m_bmAnchorOff.LoadBitmap(IDB_ANCHOR_OFFLINE);

	LOGFONT lf = {0};
	lf.lfEscapement = 0;
	lf.lfItalic = FALSE;
	lf.lfStrikeOut = FALSE;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfWidth = 0;
	_tcscpy_s(lf.lfFaceName, _T("宋体"));

	lf.lfUnderline = FALSE;
	lf.lfHeight = 12;
	lf.lfWeight = 400;

	m_font.CreateFontIndirect(&lf);	

	CRect rect(0,0,0,0);

	m_pMapCtrl = new CWxMapCtrl();
	if(!m_pMapCtrl->Create(NULL, _T("LSA1000"),  WS_CHILD|WS_VISIBLE/*|WS_BORDER*/, rect, this, 1, NULL))
	{
		TRACE0("未能创建地图控件\n");
		return -1;      // 未能创建
	};
	m_nShowAnchor = 3;

	return 0;
}


void CTopologyXView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_pMapCtrl && ::IsWindow(m_pMapCtrl->m_hWnd))
	{
		//		CRect r1,r2,rect;
		//		m_pMapCtrl->GetClientRect(&rect);
		m_pMapCtrl->MoveWindow(0, 0, cx, cy, TRUE);
		//		m_pMapCtrl->OffsetView((rect.Width()-cx)/2, (rect.Height()-cy)/2);
		m_pMapCtrl->SetCenterPos(0,0);
	}
}


void CTopologyXView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CRect rect;
	GetClientRect(&rect);
	m_pMapCtrl->MoveWindow(&rect);
	m_pMapCtrl->SetCenterPos(0,0);
}


void CTopologyXView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pMapCtrl->DrawCtrl(NULL);
}



LRESULT CTopologyXView::OnMapCtrlMsg(WPARAM wp, LPARAM lp)
{
	if(lp == NULL)
		return -1;

	LRESULT rlt = 1;
	switch(wp)
	{
	case MDRAW_DEFAULT:
		DrawAnchors((CDC *)lp);
		break;
//	case MOBJECT_RMENU: rlt = OnMapCtrlRMenuMsg((POINT *)lp);break;
	default: rlt = -1;break;
	}
	return rlt;
}



BOOL CTopologyXView::DrawAnchors(CDC *pDC)
{
	if(m_nShowAnchor == 0)
		return FALSE;

	CTopologyXDoc *pDoc = GetDocument();
	if(pDoc == NULL)
		return FALSE;

	CDC dcA,dcB;
	dcA.CreateCompatibleDC(pDC);
	dcB.CreateCompatibleDC(pDC);
	CBitmap *pOldBmpA=dcA.SelectObject(&m_bmAnchorOn);
	CBitmap *pOldBmpB=dcB.SelectObject(&m_bmAnchorOff);
	CHPVec2D pt;

	CFont *pOldFont = pDC->SelectObject(&m_font);
	std::map<UINT64, CTopoDev> *pDevs = pDoc->GetDevMap();
	for(std::map<UINT64, CTopoDev>::iterator itr=pDevs->begin(); itr!=pDevs->end(); itr++)
	{
		CTopoDev &dev = itr->second;
		m_pMapCtrl->Pos2View(dev.m_pos.x*100,dev.m_pos.y*100, pt);
		if(dev.IsOnline() && ((m_nShowAnchor&0x1)==1))
		{
			m_pMapCtrl->DrawIcon(pDC, &dcA, pt.Point(), 14, 14, dev.m_name.c_str());
		}
		else if(!dev.IsOnline() && ((m_nShowAnchor&0x2)==2))
		{
			m_pMapCtrl->DrawIcon(pDC, &dcB, pt.Point(), 14, 14, dev.m_name.c_str());
		}
	}

	pDC->SelectObject(pOldFont);

	dcA.SelectObject(pOldBmpA);
	dcB.SelectObject(pOldBmpB);
	dcA.DeleteDC();
	dcB.DeleteDC();
	
	return TRUE;
}
