
// windcView.cpp : CwindcView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "windc.h"
#endif

#include "windcDoc.h"
#include "windcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CwindcView

IMPLEMENT_DYNCREATE(CwindcView, CView)

BEGIN_MESSAGE_MAP(CwindcView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CwindcView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CwindcView 构造/析构

CwindcView::CwindcView()
{
	m_iPixelFormat = 0;
	m_hGLContext   = NULL;

	m_scale = 250;
}

CwindcView::~CwindcView()
{
}

BOOL CwindcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	return CView::PreCreateWindow(cs);
}

// CwindcView 绘制

void CwindcView::OnDraw(CDC* /*pDC*/)
{
	CwindcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CwindcView 打印


void CwindcView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CwindcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CwindcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CwindcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CwindcView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CwindcView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CwindcView 诊断

#ifdef _DEBUG
void CwindcView::AssertValid() const
{
	CView::AssertValid();
}

void CwindcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CwindcDoc* CwindcView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CwindcDoc)));
	return (CwindcDoc*)m_pDocument;
}
#endif //_DEBUG


// CwindcView 消息处理程序
BOOL CwindcView::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW/*|PFD_SUPPORT_GDI*/;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	m_iPixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (0 == m_iPixelFormat)
	{
		if (0 == DescribePixelFormat(hDC, 1, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
		{
			return FALSE;
		}
	}

	if(FALSE == SetPixelFormat(hDC, m_iPixelFormat, &pfd))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CwindcView::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);

	if (NULL == m_hGLContext)
	{
		return FALSE;
	}

	if (FALSE == wglMakeCurrent(hDC, m_hGLContext))
	{
		return FALSE;
	}

	theApp.m_opengl.Init(1000, 500);
	SetTimer(1, 1, NULL);
	SetTimer(2, 1000, NULL);
	return TRUE;
}

int CwindcView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	HWND hwnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hwnd);
	if (FALSE == SetWindowPixelFormat(hDC))
	{
		return 0;
	}

	if (FALSE == CreateViewGLContext(hDC))
	{
		return 0;
	}

	return 0;
}


void CwindcView::OnDestroy()
{
	CView::OnDestroy();

	if (NULL != wglGetCurrentContext())
	{
		wglMakeCurrent(NULL, NULL);
	}

	if (NULL != m_hGLContext)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}
}


void CwindcView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	GLsizei  width, height;
	GLdouble aspect;
	width = cx;
	height = cy;
	if (0 == cy)
	{
		aspect = (GLdouble)width;
	}
	else
	{
		aspect = (GLdouble)width / (GLdouble)height;
	}
	glViewport(0, 0, width, height);
	
}


void CwindcView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CView::OnPaint()
	theApp.m_opengl.Draw(250);
	SwapBuffers(dc.m_hDC);
}


BOOL CwindcView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CView::OnEraseBkgnd(pDC);
}


BOOL CwindcView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	unsigned int scales = 20;
	if (zDelta > 0)
	{
		m_scale += scales;
	}
	else if (zDelta < 0 && scales < m_scale)
	{
		m_scale -= scales;
	}
	Invalidate();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CwindcView::OnTimer(UINT_PTR nIDEvent)
{
	if (1 == nIDEvent)
	{
		theApp.m_opengl.UpdateTexture();
	}
	else if (2 == nIDEvent)
	{
		Invalidate();
	}

	CView::OnTimer(nIDEvent);
}
