
// TopologyView.cpp : implementation of the CTopologyView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Topology.h"
#endif

#include "TopologyDoc.h"
#include "TopologyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTopologyView

IMPLEMENT_DYNCREATE(CTopologyView, CView)

BEGIN_MESSAGE_MAP(CTopologyView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTopologyView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CTopologyView construction/destruction

CTopologyView::CTopologyView()
{
	// TODO: add construction code here

}

CTopologyView::~CTopologyView()
{
}

BOOL CTopologyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTopologyView drawing

void CTopologyView::OnDraw(CDC* pDC)
{
	CTopologyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	CRect rect;
	int m_Low = -500;
	int m_High = 500;
	CString str;
	int i;
	int m_left,m_top,m_right,m_bottom;

	int m_Interval = (m_High - m_Low)/10;
	if (m_Interval < 1)  m_Interval = 1;

	// 获取绘制坐标的文本框

	GetClientRect(&rect);
	pDC->Rectangle(&rect);

	m_left = rect.left + 50;
	m_top = rect.top+10;
	m_right = rect.right-50;
	m_bottom = rect.bottom-50;

	int m_IntervalPan = (m_right - m_left)/10;
	if (m_IntervalPan < 1 ) m_IntervalPan =1;

	int m_IntervalPanY = (m_bottom - m_top) / 10;
	if (m_IntervalPanY < 1 ) m_IntervalPanY =1;

	// 创建画笔对象
	CPen* pPenRed = new CPen;

	// 红色画笔
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));

	// 创建画笔对象
	CPen* pPenBlue = new CPen;

	// 蓝色画笔
	pPenBlue->CreatePen(PS_SOLID,1,RGB(0,0, 255));

	// 创建画笔对象
	CPen* pPenGreen = new CPen;

	// 绿色画笔
	pPenGreen->CreatePen(PS_DOT,1,RGB(0,255,0));

	// 选中当前红色画笔，并保存以前的画笔
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);

	// 绘制坐标轴
	pDC->MoveTo(rect.left + 50, (m_bottom - m_top) / 2);

	// X轴
	pDC->LineTo(rect.right - 50, (m_bottom - m_top) / 2);

	pDC->MoveTo(m_left+5*m_IntervalPan, 10);
	// Y轴
	pDC->LineTo(m_left+5*m_IntervalPan, rect.Height() - 10);


	// 写X轴刻度值
	for(i=0;i<10;i++)
	{
		//str.Format(_T("%d"),m_Min+i*m_Interval);
		str.Format(_T("%d"),m_Low+i*m_Interval);
		pDC->TextOut(m_left+i*m_IntervalPan,(m_bottom - m_top) / 2 + 3,str); 
	}
	//str.Format(_T("%d"),m_Max);
	str.Format(_T("%d"),m_High);
	pDC->TextOut(m_left+10*m_IntervalPan,(m_bottom - m_top) / 2+3,str); 

	// 绘制X轴刻度
	for (i = m_left; i < m_right-20; i += 5)
	{
		if ((i & 1) == 0)
		{
			// 10的倍数
			pDC->MoveTo(i + 10, (m_bottom - m_top) / 2);
			pDC->LineTo(i + 10, (m_bottom - m_top) / 2+4);
		}
		else
		{
			// 10的倍数
			pDC->MoveTo(i + 10, (m_bottom - m_top) / 2);
			pDC->LineTo(i + 10, (m_bottom - m_top) / 2+2);
		}
	}

	// 绘制X轴箭头
	pDC->MoveTo(m_right-5,(m_bottom - m_top) / 2-5);
	pDC->LineTo(m_right,(m_bottom - m_top) / 2);
	pDC->LineTo(m_right-5,(m_bottom - m_top) / 2+5);

	// 绘制Y轴箭头 
	pDC->MoveTo(m_left+5*m_IntervalPan-5,m_top+5);
	pDC->LineTo(m_left+5*m_IntervalPan,m_top);
	pDC->LineTo(m_left+5*m_IntervalPan+5,m_top+5);

	// 绘制Y轴网格 选择绿色画笔
	pDC->SelectObject(pPenGreen); 
	//
	int iTemp = (m_bottom  - m_top)/10;
	for (i = 10 ;i >= 0 ;i--)
	{
		if (5 == i)
		{
			continue;
		}
		pDC->MoveTo(m_left,m_bottom - i*iTemp );
		pDC->LineTo(m_right,m_bottom - i*iTemp);
		str.Format(_T("%d"),m_High - i*m_Interval);
		pDC->TextOut(m_left+5*m_IntervalPan - 40,m_top+i*iTemp,str); 
	}
	/************************
	// 更改成蓝色画笔
	pDC->SelectObject(pPenBlue);
	在这里添加绘制曲线的函数
	*************************/
	// 恢复以前的画笔
	pDC->SelectObject(pOldPen); 

	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;

	return;
}


// CTopologyView printing


void CTopologyView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTopologyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTopologyView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTopologyView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTopologyView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTopologyView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTopologyView diagnostics

#ifdef _DEBUG
void CTopologyView::AssertValid() const
{
	CView::AssertValid();
}

void CTopologyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTopologyDoc* CTopologyView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTopologyDoc)));
	return (CTopologyDoc*)m_pDocument;
}
#endif //_DEBUG


// CTopologyView message handlers

