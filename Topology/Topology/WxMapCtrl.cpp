// WxMapCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "WxMapCtrl.h"
#include <math.h>
#include "WxDC.h"


// CWxMapCtrl

//IMPLEMENT_DYNAMIC(CWxMapCtrl, CWnd)
IMPLEMENT_DYNCREATE(CWxMapCtrl, CWnd)

CWxMapCtrl::CWxMapCtrl()
{
//	m_crBack = RGB(64,64,80);
//	m_crBack = RGB(0,0,0);
	m_crBack = RGB(255,255,255);
	m_nPix = 50;
//	m_nRulerWidth = 20;
	m_nSelect = pvisNone;
	m_nMovePos = pvisPic;
	m_bCtrlAlwaysFocus = FALSE;
	m_crTrack = RGB(128,128,128);
	m_crPoint = RGB(192,0,192);
	m_bMouseTracking = FALSE;
	m_strBmpName = _T("");
//	m_bMapLoad = FALSE;
	SetColor(m_crBack); // m_crGrid, m_crLine

	m_bHasBorder = FALSE;
	m_bRectsNeedRecalculate = TRUE;

	m_rectThumb = CRect(0,0,0,0);			//小图区域
	m_rectPic = CRect(0,0,0,0);				//图形显示区域
	m_rectXRuler = CRect(0,0,0,0);
	m_rectYRuler = CRect(0,0,0,0);
	m_rectRedraw = CRect(0,0,0,0);

	m_rectUser = CRect(0,0,0,0);

//	m_hBitmap = NULL;

	m_PntOffset = CPoint(0,0);
	m_PntOffsetAdd = CPoint(0,0);
	m_CursorPos = CPoint(0,0);
	m_fZoom = 1.0f;
	m_fPicZoomX = 1.0f;
	m_fPicZoomY = 1.0f;
	m_pntPicOff.x = 0;
	m_pntPicOff.y = 0;

//	m_nCmPerPixel=10; //每像素的厘米数 ——最大比例为1像素1厘米

	HDC hdc = ::GetDC(NULL);
	int xDpi = ::GetDeviceCaps (hdc,  LOGPIXELSX);
	int yDpi = ::GetDeviceCaps (hdc,  LOGPIXELSY);
	::DeleteDC(hdc);

	m_coefx = xDpi / 2.54f; 
	m_coefy = yDpi / 2.54f;

	m_nGridRadiu = 0;
	m_nShowRuler = 0;
//	m_nScale = 100;

	m_hCursorArrow = ::LoadCursor(NULL, IDC_ARROW);
	m_hCursorSizeall = ::LoadCursor(NULL, IDC_SIZEALL);
	m_hCursorHand = ::LoadCursor(NULL, IDC_HAND);
	m_hMyCursor = m_hCursorHand;

	m_bSetTimer = FALSE;
	m_bShowThumb = FALSE;

	m_dLogThresholdX = 0.00f;
	m_dLogThresholdY = 0.00f;


	int nBody = MAPCTRL_RULER_WIDTH - 2*(MAPCTRL_RULER_WIDTH/4);

	LOGFONT lf = {0};
	lf.lfEscapement = 0;
	lf.lfItalic = NULL;
	lf.lfUnderline = NULL;
	lf.lfStrikeOut = NULL;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfHeight = nBody - 2;
	_tcscpy(lf.lfFaceName, _T("Arial"));
	m_font.CreateFontIndirect(&lf);

	m_pMessageHandler = NULL;

	m_dwPrs = prsMM;

	m_nGridRadiu = 100;
	m_nShowRuler = 3;
}

CWxMapCtrl::~CWxMapCtrl()
{
	m_pMessageHandler = NULL;

	m_font.DeleteObject();

	Clear();
}

void CWxMapCtrl::Clear()
{
	m_imgLoader1.Destroy();

	for(std::vector<ImageObject*>::iterator itr=m_vecImg.begin(); itr!=m_vecImg.end(); itr++)
	{
		delete (*itr);
		*itr=NULL;
	}
	m_vecImg.clear();
}

BEGIN_MESSAGE_MAP(CWxMapCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
//	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()



// CWxMapCtrl message handlers


int CWxMapCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CWxMapCtrl::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(TIMER_PICVIEW_MOVE)
	{
		if(m_bSetTimer)
			KillTimer(TIMER_PICVIEW_MOVE);
		m_bSetTimer = FALSE;
	}
	CWnd::OnTimer(nIDEvent);
}


BOOL CWxMapCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE; 	
}


void CWxMapCtrl::RecalculateRects(int cx, int cy) 
{
	if(m_bHasBorder)
	{
		int xBdr = ::GetSystemMetrics(SM_CXBORDER);
		int yBdr = ::GetSystemMetrics(SM_CYBORDER);

		m_rectPic.left = xBdr+1;
		m_rectPic.right = cx;
		m_rectPic.top = yBdr+1;
		m_rectPic.bottom = cy;
	}
	else
	{
		m_rectPic = CRect(0,0,cx,cy);
	}
	m_rectRedraw = m_rectPic;
	
	if(m_nShowRuler)
	{
		m_rectXRuler.left = m_rectPic.left + ((m_nShowRuler & MAPCTRL_SHOW_VRULER)?MAPCTRL_RULER_WIDTH:0);
		m_rectXRuler.right = m_rectPic.right;
		m_rectXRuler.top = m_rectPic.top;
		m_rectXRuler.bottom = m_rectPic.top + MAPCTRL_RULER_WIDTH*2/3;

		m_rectYRuler.top = m_rectPic.top + ((m_nShowRuler & MAPCTRL_SHOW_HRULER)?MAPCTRL_RULER_WIDTH*2/3:0);
		m_rectYRuler.bottom = m_rectPic.bottom;
		m_rectYRuler.left = m_rectPic.left;
		m_rectYRuler.right = m_rectPic.left + MAPCTRL_RULER_WIDTH;


		m_rectPic.top = m_rectYRuler.top;
		m_rectPic.left = m_rectXRuler.left;
	}
/*
// TODO: 缩略图不受底图限制，应为所有轨迹范围 m_rectUser
	if(m_bShowThumb && (!m_imgLoader1.IsNull()))
	{
		m_rectThumb.top = m_rectPic.top;
		m_rectThumb.right = m_rectPic.right;
		m_rectThumb.left = m_rectThumb.right - m_rectPic.Width()/3;
		m_rectThumb.bottom = m_rectThumb.top + m_rectPic.Height()/2;
		double dtemp;
		dtemp = (m_bmHeight*m_bmWidth)?1.00:((double)m_bmHeight/(double)m_bmWidth);

		int nThumbLen = m_rectThumb.Width()*dtemp;
		if(nThumbLen > m_rectThumb.Height())
		{
			nThumbLen = m_rectThumb.Height()/dtemp;
			m_rectThumb.left = m_rectThumb.right - nThumbLen;
		}
		else
		{
			m_rectThumb.bottom = m_rectThumb.top + nThumbLen;
		}
	}
*/
	m_bRectsNeedRecalculate = FALSE;
}

void CWxMapCtrl::OnPaint() 
{
	CPaintDC dc(this);
	CRect rct;
	GetClientRect(rct);

	if(m_bRectsNeedRecalculate)
	{
		if(rct.Width() >= 100 && rct.Height() >= 250)
			RecalculateRects(rct.Width(),rct.Height());
	}
	
	if(m_bHasBorder)
	{
		dc.DrawEdge(&rct, BDR_SUNKENINNER|BDR_SUNKENOUTER, BF_RECT);
	}

	DrawCtrl(&dc);

	
}
void CWxMapCtrl::OnSize(UINT nType, int cx, int cy)
{
//	int cx0 = m_rectPic.Width();
//	int cy0 = m_rectPic.Height();

	CWnd::OnSize(nType, cx, cy);
/*	switch(m_dwPrs)
	{
	case prsTL:break;
	case prsTM:m_PntOffset.x += ((cx0-cx)/2);break;
	case prsTR:m_PntOffset.x += (cx0-cx);break;
	case prsML:m_PntOffset.y += (cy0-cy);break;
	case prsMM:m_PntOffset.x += ((cx0-cx)/2); m_PntOffset.y += ((cy0-cy)/2); break;
	case prsMR:m_PntOffset.x += ((cx0-cx)); m_PntOffset.y += ((cy0-cy)/2); break;
	case prsBL:m_PntOffset.y += (cy0-cy); break;
	case prsBM:m_PntOffset.x += ((cx0-cx)/2); m_PntOffset.y += (cy0-cy);break;
	case prsBR:m_PntOffset.x += (cx0-cx); m_PntOffset.y += (cy0-cy); break;
	}*/

	m_bRectsNeedRecalculate = TRUE;
//	OnPaint();
}


int CWxMapCtrl::CheckPointPos(CPoint point)
{
	int nRet = pvisNone;
	

	if(m_bShowThumb && m_rectThumb.PtInRect(point))
	{
		nRet = pvisThmb;
	}
	else if(((m_nShowRuler&MAPCTRL_SHOW_HRULER) && m_rectXRuler.PtInRect(point))
		||((m_nShowRuler&MAPCTRL_SHOW_VRULER) && m_rectYRuler.PtInRect(point)))
	{
		nRet = pvisRuler;
	}
	else 
		nRet = pvisPic;

	return nRet;
}

LRESULT CWxMapCtrl::SendUserMessage(UINT nMsg, LPARAM lp)
{
	if(m_pMessageHandler)
		return m_pMessageHandler->SendMessage(WM_MAP_CTRL, (WPARAM)nMsg, lp);
	else
	{
		CWnd *pParent = GetParent();
		if(pParent)
		{
			return pParent->SendMessage(WM_MAP_CTRL,(WPARAM)nMsg, lp);
		}
	}
	return -1;

}

void CWxMapCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_CursorPos = point;
	m_nSelect = CheckPointPos(point);
	//如果落点在图像上，检查是否在用户对象上
	if(m_nSelect == pvisPic)
	{
		if(0 < SendUserMessage(MOBJECT_POINTIN,(LPARAM)(&point)))
		{
			m_nSelect = pvisUser;
		}
	}

	m_nMovePos = m_nSelect;

	if(m_nSelect & (pvisPic|pvisUser))
		m_hMyCursor = m_hCursorSizeall;

};
void CWxMapCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_nSelect & pvisUser)
	{
		m_nSelect = pvisNone;
		SendUserMessage(MOBJECT_CHGPOS,(LPARAM)(&point));
		Invalidate();
	}

	if(m_nSelect &(pvisRuler))
	{
		::SetCursor(m_hCursorHand);
		m_nSelect = pvisNone;
		m_bRectsNeedRecalculate = TRUE;
		OnPaint();
	}
	else
		m_nSelect = pvisNone;

	m_hMyCursor = m_hCursorHand;
};


void CWxMapCtrl::MoveFloatLayer(CRect &rectLayer, CPoint point) 
{
	int w = rectLayer.Width();
	int h = rectLayer.Height();
	rectLayer.left += (point.x - m_CursorPos.x);
	if(rectLayer.left < m_rectPic.left)
		rectLayer.left = m_rectPic.left;
	rectLayer.right = rectLayer.left + w;
	if(rectLayer.right > m_rectPic.right)
	{
		rectLayer.right = m_rectPic.right;
		rectLayer.left = rectLayer.right - w;
	}
	rectLayer.top += (point.y - m_CursorPos.y);
	if(rectLayer.top < m_rectPic.top)
		rectLayer.top = m_rectPic.top;
	rectLayer.bottom = rectLayer.top + h;
	if(rectLayer.bottom > m_rectPic.bottom)
	{
		rectLayer.bottom = m_rectPic.bottom;
		rectLayer.top = rectLayer.bottom - h;
	}

}

void CWxMapCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bMouseTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);

		tme.dwFlags  = TME_LEAVE;// | TME_HOVER;
		tme.hwndTrack = this->m_hWnd;

		if(::_TrackMouseEvent(&tme))
		{
			m_bMouseTracking = TRUE;
		}
	}

	int nLastPos = m_nMovePos;
		

	if(m_nSelect == pvisPic || m_nSelect == pvisUser)
		m_nMovePos = m_nSelect;
	else // m_nSelect != pvisPic && m_nSelect != pvisUser
	{
		m_nMovePos = CheckPointPos(point);
		//没有选中对象，检查鼠标点的位置
		if(m_nMovePos == pvisPic)
		{
			SendUserMessage(MPOINT_POS,(LPARAM)(&point));
		}
	}
	if(m_nSelect & (pvisPic|pvisThmb|pvisUser))
		m_hMyCursor = m_hCursorSizeall;
	else if(m_nMovePos == pvisRuler)
		m_hMyCursor = m_hCursorArrow;
	else
		m_hMyCursor = m_hCursorHand;
	

	if(m_nSelect == pvisPic)
	{
		m_PntOffset.x += (LONG)((m_CursorPos.x-point.x)/m_fZoom);
		m_PntOffset.y += (LONG)((m_CursorPos.y-point.y)/m_fZoom);
	
		Invalidate();
	}
	else if(m_nSelect == pvisThmb)
	{
		MoveFloatLayer(m_rectThumb, point);
		Invalidate();
	}
	else if(m_nSelect == pvisUser)
	{
		if(0 < SendUserMessage(MOBJECT_MOVE,(LPARAM)(&point)))
			Invalidate();
	}
	else if(m_nMovePos != nLastPos)
	{
		Invalidate();
	}
	m_CursorPos = point;

};

LRESULT CWxMapCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if(m_bCtrlAlwaysFocus == FALSE)
	{
		m_nSelect = pvisNone;
		m_nMovePos = pvisNone;
		
		Invalidate();
	}
	KillTimer(TIMER_PICVIEW_MOVE);
	m_bMouseTracking = FALSE;
	return TRUE;
}
void CWxMapCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

//		::SetCursor(m_hCursor[3]);

	m_CursorPos = point;

	SendUserMessage(MOBJECT_ADD,(LPARAM)(&point));

//	CWnd::OnRButtonUp(nFlags, point);
}

void CWxMapCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//获得当前状态
	SendUserMessage(MOBJECT_RMENU,(LPARAM)(&point));
	//	CWnd::OnRButtonUp(nFlags, point);
}

BOOL CWxMapCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if((HTCLIENT == nHitTest) && (m_hMyCursor != NULL))
	{
		::SetCursor(m_hMyCursor);
		return TRUE;
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CWxMapCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
//	dwStyle |= ;
//	rect.right = rect.left + 50;
//	rect.bottom = rect.top + 230;
	dwStyle |= WS_OVERLAPPED;
//	dwStyle |= WS_CHILD | WS_BORDER;
	if(dwStyle & WS_BORDER)
	{
		m_bHasBorder = TRUE;
		dwStyle &= ~WS_BORDER;
	}
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CWxMapCtrl::PreSubclassWindow() 
{
	CWnd::PreSubclassWindow();	
	// TODO: Add your specialized code here and/or call the base class
	HMODULE hModule = GetModuleHandle(_T("User32.DLL"));
	if(hModule == NULL)
	{
		return;
	}
	typedef BOOL (WINAPI* SETLAYEREDWND)(HWND, COLORREF, BYTE, DWORD);
	SETLAYEREDWND SetLayeredWindowPtr = NULL;
	SetLayeredWindowPtr = (SETLAYEREDWND)GetProcAddress(hModule, "SetLayeredWindowAttributes");
	if(SetLayeredWindowPtr)
	{
	#ifndef WS_EX_LAYERED
		#define LWA_COLORKEY 	0x00000001
		#define LWA_ALPHA		0x00000002
		#define WS_EX_LAYERED 	0x00080000
	#endif

		SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE)|WS_EX_LAYERED);
	
		SetLayeredWindowPtr(this->GetSafeHwnd(), m_crBack, 255/2, LWA_COLORKEY|LWA_ALPHA);
//	SetLayeredWindowPtr(this->GetSafeHwnd(), 0, 255/2, LWA_ALPHA);

	}
	FreeLibrary(hModule);

//	SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, m_hCursor[13]);
}



HCURSOR CWxMapCtrl::SetMyCursor(HCURSOR hCursor)
{
	HCURSOR hRet = m_hMyCursor;
	m_hMyCursor = hCursor;
	return hRet;
}

void CWxMapCtrl::SetColor(COLORREF crBack, BOOL bRedraw)
{
	//背景颜色
	m_crBack = crBack;

	int r,g,b, r1,g1,b1;
	
	r = GetRValue(crBack);
	g = GetGValue(crBack);
	b = GetBValue(crBack);

	//格线颜色
	r1 = (r>127)?(r-31):(r+31);
	g1 = (g>127)?(g-31):(g+31);
	b1 = (b>127)?(b-31):(b+31);
	m_crGrid = RGB(r1,g1,b1);

	//文字颜色,标示线颜色
	r1 = (r>127)?(64):(192);
	g1 = (g>127)?(64):(192);
	b1 = (b>127)?(64):(192);
	m_crLine = RGB(r1,g1,b1);
	



	if(bRedraw)
	{
		Invalidate();
	}
}

void CWxMapCtrl::Zoom(BOOL bIncrease)
{
	m_fZoom += ((bIncrease)? (0.1f):(-0.1f));
	m_fZoom = (m_fZoom > 10.0f) ? 10.0f : m_fZoom;
	m_fZoom = (m_fZoom < 0.1f) ? 0.1f : m_fZoom;
	Invalidate();
}

void CWxMapCtrl::ShowBorder(BOOL bShow) 
{
	if(m_bHasBorder != bShow)
	{
		m_bRectsNeedRecalculate = TRUE;
		m_bHasBorder = bShow;
		Invalidate();
	}
}


void CWxMapCtrl::ShowGrid(INT nGrid)
{
	if(nGrid != m_nGridRadiu)
	{
		m_nGridRadiu = nGrid;
		Invalidate();
	}
}

void CWxMapCtrl::ShowRuler(INT nShow)
{
	if(nShow != m_nShowRuler)
	{
		m_nShowRuler = nShow;
		m_bRectsNeedRecalculate = TRUE;
		Invalidate();
	}
}

void CWxMapCtrl::SetCenterPixel(POINT &pnt)
{
	POINT pnt0;
	pnt0.x = m_rectPic.Width()/2 / m_fZoom;
	pnt0.y = m_rectPic.Height()/2 / m_fZoom;
	m_PntOffset.x = pnt.x - pnt0.x;
	m_PntOffset.y = pnt.y - pnt0.y;
}

void CWxMapCtrl::OffsetView(int deltaX, int deltaY)
{
	m_PntOffset.x += deltaX;
	m_PntOffset.y += deltaY;
}

void CWxMapCtrl::SetCenterPos(double xPos, double yPos)
{
	CPoint pnt,pnt0;
	pnt.x = (int)(xPos * m_coefx / 100.00f);
	pnt.y = (int)(yPos * m_coefy / 100.00f);
	SetCenterPixel(pnt);
}


void CWxMapCtrl::ShowScale(INT nScale)
{
	float fz = (float)(100.00f / (double)nScale);
	if(fz != m_fZoom)
	{
		m_fZoom = fz;
		Invalidate();
	}

}

void CWxMapCtrl::_Pix2Pos(double xPix, double yPix, double &xPos, double &yPos)
{
	double dx,dy;
	dx = xPix * 100 / m_coefx;
	dy = yPix * 100 / m_coefy;

	xPos = CalcLogarithms(dx, m_dLogThresholdX);
	yPos = CalcLogarithms(dy, m_dLogThresholdY);

}

void CWxMapCtrl::_Pos2Pix(double xPos, double yPos, double &dx, double &dy)
{
	//dx = xPos * m_coefx / 100;
	//dy = yPos * m_coefy / 100;
	if(m_dLogThresholdX >= 0.001f && xPos > m_dLogThresholdX)
	{
		dx = (int) (m_dLogThresholdX*m_coefx/100.00f * (1+log10(xPos/m_dLogThresholdX)));
	}
	else
	{
		dx = (int)(xPos * m_coefx / 100.00f);
	}

	if(m_dLogThresholdY >= 0.001f && yPos > m_dLogThresholdY)
	{
		dy = (int) (m_dLogThresholdY*m_coefy/100.00f * (1+log10(yPos/m_dLogThresholdY)));
	}
	else
	{
		dy = (int)(yPos * m_coefy / 100.00f);
	}


}

void CWxMapCtrl::DrawPosHyperbola(CDC *pDC, CHPVec3D& pos0, CHPVec3D& pos1, double a, BOOL isVp, CHPVec2D ptt)
{
	CHPVec2D pt0,pt1;
	Pos2View(pos0.x*100.00f, pos0.y*100.00f, pt0);
	Pos2View(pos1.x*100.00f, pos1.y*100.00f, pt1);

	double dcc = pt0.DistanceSquare(pt1)/4;
	double dc = sqrt(dcc);
	double da = a * m_coefy * m_fZoom;
	double dbb = dcc - da*da;
	double db = sqrt(dbb);
	double ph = atan2(pt1.y-pt0.y, pt1.x-pt0.x);
	double cosph = cos(-ph);
	double sinph = sin(-ph);
	double deltax = (pt0.x+pt1.x)/2.00f;
	double deltay = (pt0.y+pt1.y)/2.00f;

	std::vector<CHPVec2D> vec1;
	std::vector<CHPVec2D> vec2;
	vec1.reserve(101);
	vec2.reserve(101);
	CHPVec2D hv2,hv1;
	double x,y,x1,y1;
	for(y=-1000.00f; y<=1000.00f; y += 20.00f)
	{
		x = da*sqrt(1+(y*y)/dbb);

		x1 = x*cosph + y*sinph + deltax;
		y1 = y*cosph - x*sinph + deltay;
		vec1.push_back(CHPVec2D(x1,y1));
		
/*		// x = -x;
		x1 = -x*cosph + y*sinph + deltax;
		y1 = y*cosph + x*sinph + deltay;
		vec2.push_back(CHPVec2D(x1,y1));*/
	}

	BOOL bFirst = TRUE;
	double w = m_rectPic.Width();
	double h = m_rectPic.Height();
	for(std::vector<CHPVec2D>::iterator itr=vec1.begin(); itr!=vec1.end(); itr++)
	{
		if(itr->x > 0 && itr->x < w && itr->y > 0 && itr->y < h)
		{
			(bFirst)?pDC->MoveTo(itr->x, itr->y): pDC->LineTo(itr->x, itr->y);
			bFirst = FALSE;
		}
		else
		{
			bFirst = TRUE;
		};
	}
	bFirst = TRUE;
	for(std::vector<CHPVec2D>::iterator itr=vec2.begin(); itr!=vec2.end(); itr++)
	{
		if(itr->x > 0 && itr->x < w && itr->y > 0 && itr->y < h)
		{
			(bFirst)?pDC->MoveTo(itr->x, itr->y): pDC->LineTo(itr->x, itr->y);
			bFirst = FALSE;
		}
		else
		{
			bFirst = TRUE;
		};
	}

/*
	double th, r;
	for(th=0; th<2*CONST_PI; th = th + CONST_PI/25.00f)
	{
		if(a>0)
		{
			r = da*da/(da-dc*cos(th-ph));
		}
		else
		{
			r = da*da/(-da+dc*cos(th-ph));
		}
		hv2.x = r*cos(th)+pt0.x+dc*cos(ph);
		hv2.y = r*sin(th)+pt0.y+dc*sin(ph);

		vec.push_back(hv2);
	}


	BOOL bFirst = TRUE;
	double w = m_rectPic.Width();
	double h = m_rectPic.Height();
	for(std::vector<CHPVec2D>::iterator itr=vec.begin(); itr!=vec.end(); itr++)
	{
		if(itr->x > 0 && itr->x < w && itr->y > 0 && itr->y < h)
		{
			(bFirst)?pDC->MoveTo(itr->x, itr->y): pDC->LineTo(itr->x, itr->y);
			bFirst = FALSE;
		}
		else
		{
			bFirst = TRUE;
		};
	}
*/
	if(isVp)
	{
		pDC->MoveTo(pt0.x,pt0.y);
		pDC->LineTo(ptt.x,ptt.y);

		pDC->MoveTo(pt1.x,pt1.y);
		pDC->LineTo(ptt.x,ptt.y);
	}

}

void CWxMapCtrl::DrawPosRound(CDC *pDC, CHPVec3D pos, double r, BOOL isVp, CHPVec2D ptt)
{
	CBrush *pOldBrush = (CBrush *)pDC->SelectStockObject(NULL_BRUSH);

	CHPVec2D pt;
	Pos2View(pos.x*100.00f, pos.y*100.00f, pt);

	int dy = (int)(r * m_coefy * m_fZoom);
	int dx = (int)(r * m_coefx * m_fZoom);
	pDC->Ellipse(pt.x-dx, pt.y-dy, pt.x+dx,  pt.y+dy); 

	if(isVp)
	{
		pDC->MoveTo(pt.x,pt.y);
		pDC->LineTo(ptt.x,ptt.y);
	}
	pDC->SelectObject(pOldBrush);
};

void CWxMapCtrl::Pos2Pix(double xPos, double yPos, double &x, double &y)
{
	_Pos2Pix(xPos,yPos,x,y);


	if(x<m_rectUser.left) m_rectUser.left = x;
	if(x>m_rectUser.right) m_rectUser.right = x;
	if(y<m_rectUser.top) m_rectUser.top = y;
	if(y>m_rectUser.bottom) m_rectUser.bottom = y;
};


void CWxMapCtrl::Pix2View(double x, double y, HPVEC2D &pt)
{
	pt.x = ((x - m_PntOffset.x)*m_fZoom) + m_rectPic.left;
	pt.y = ((y - m_PntOffset.y)*m_fZoom) + m_rectPic.top;

}

void CWxMapCtrl::Pos2View(double xPos, double yPos, HPVEC2D &pt)
{
	double dx,dy;
	_Pos2Pix(xPos,yPos,dx,dy);

	int x = (int)dx;
	int y = (int)dy;

	if(x<m_rectUser.left) m_rectUser.left = x;
	if(x>m_rectUser.right) m_rectUser.right = x;
	if(y<m_rectUser.top) m_rectUser.top = y;
	if(y>m_rectUser.bottom) m_rectUser.bottom = y;

	pt.x = (LONG)((dx - m_PntOffset.x)*m_fZoom) + m_rectPic.left;
	pt.y = (LONG)((dy - m_PntOffset.y)*m_fZoom) + m_rectPic.top;

}

void CWxMapCtrl::_View2Pix(HPVEC2D &ptv, double &xPix, double &yPix)
{
	xPix = (ptv.x - m_rectPic.left)/m_fZoom + m_PntOffset.x;
	yPix = (ptv.y - m_rectPic.top)/m_fZoom + m_PntOffset.y;
};

void CWxMapCtrl::View2Pos(HPVEC2D &pt, double &xPos, double &yPos)
{
	double xPix,yPix;
	_View2Pix(pt, xPix, yPix);
	_Pix2Pos(xPix, yPix, xPos,yPos);
//	xPos = (m_PntOffset.x + (pt.x-m_rectPic.left)/m_fZoom) * 100 / m_coefx;
//	yPos = (m_PntOffset.y + (pt.y-m_rectPic.top)/m_fZoom) * 100 / m_coefy;
};

void CWxMapCtrl::ViewLen2PosLen( double &coefx, double &coefy)
{
	coefx = 100.00f / (m_fZoom * m_coefx);
	coefy = 100.00f / (m_fZoom * m_coefy);
}

void CWxMapCtrl::GetCoef( double &coefx, double &coefy)
{
	coefx = m_coefx;
	coefy = m_coefy;
}

void CWxMapCtrl::DrawPosRgn(CDC *pDC, CRgn *pRgn, BOOL bShowBorder, BOOL bFillColor, COLORREF cr)
{
	float fScaleX = (m_fZoom * m_coefx)/100.00f, fScaleY = (m_fZoom * m_coefy)/100.00f;// 缩放倍数
	float fDx = m_rectPic.left - m_PntOffset.x*m_fZoom, fDy=m_rectPic.top - m_PntOffset.y*m_fZoom; //平移坐标
	XFORM xAffine = { fScaleX,0,0,fScaleY,fDx,fDy };
//	xAffine.eM11 *= fScaleX;
//	xAffine.eM12 *= fScaleX;
//	xAffine.eM21 *= fScaleY;
//	xAffine.eM22 *= fScaleY;
//	xAffine.eDx  *= fScaleX;
//	xAffine.eDy  *= fScaleY;

	int nCount = pRgn->GetRegionData(NULL, 0);
	if(nCount == 0)
		return;
	LPRGNDATA pRgnData = new RGNDATA[nCount];
	pRgn->GetRegionData(pRgnData, nCount);

	CRgn rgnScale;
	rgnScale.CreateFromData(&xAffine, nCount, pRgnData );
	CBrush brhScale(cr);
	if(bFillColor)
	{
//		pDC->FillRgn(&rgnScale, &brhScale);
		nCount = rgnScale.GetRegionData(NULL, 0);
		LPRGNDATA pRgnData1 = new RGNDATA[nCount];
		rgnScale.GetRegionData(pRgnData1, nCount);
		CRect rct = pRgnData1->rdh.rcBound;

		pDC->SelectClipRgn(&rgnScale,RGN_COPY);  
		rgnScale.DeleteObject();  
		// TODO: add draw code for native data here  
		BLENDFUNCTION bl;   
		bl.BlendOp=AC_SRC_OVER;   
		bl.BlendFlags=0;   
		bl.AlphaFormat=0;   
		bl.SourceConstantAlpha=128;   

		CDC memDC;   
		memDC.CreateCompatibleDC(pDC);   
		CBitmap bmp;   
		bmp.CreateCompatibleBitmap(pDC,rct.Width(),rct.Height());  
		CBitmap *pOldBmp = memDC.SelectObject(&bmp);  
		memDC.FillSolidRect(0,0,rct.Width(),rct.Height(),cr);  
		AlphaBlend(pDC->m_hDC,rct.left,rct.top,rct.Width(),rct.Height(),memDC.m_hDC,0,0,rct.Width(),rct.Height(),bl);   
		memDC.SelectObject(pOldBmp);   
		ReleaseDC(&memDC);  
		bmp.DeleteObject();  
		delete []pRgnData1;

		pDC->SelectClipRgn(NULL);  
	}
	else if(bShowBorder)
	{
		pDC->FrameRgn(&rgnScale, &brhScale, 1,1);
	}
	rgnScale.Detach();
	delete []pRgnData;
	DeleteObject(rgnScale.m_hObject);
}


void CWxMapCtrl::CreateRgnFromWxRgn(CRgn &rgn, CWxRgn *wxrgn, XFORM *xaf)
{
	LONG x1,y1,x2,y2;
	switch(wxrgn->GetRgnType())
	{
	case 1:
		{
			CWxEllipticRgn *pergn=(CWxEllipticRgn *)wxrgn;
			x1 = xaf->eM11 * (pergn->m_x0 - pergn->m_rx) + xaf->eM21 * (pergn->m_y0 - pergn->m_ry) + xaf->eDx;
			y1 = xaf->eM12 * (pergn->m_x0 - pergn->m_rx) + xaf->eM22 * (pergn->m_y0 - pergn->m_ry) + xaf->eDy;
			x2 = xaf->eM11 * (pergn->m_x0 + pergn->m_rx) + xaf->eM21 * (pergn->m_y0 + pergn->m_ry) + xaf->eDx;
			y2 = xaf->eM12 * (pergn->m_x0 + pergn->m_rx) + xaf->eM22 * (pergn->m_y0 + pergn->m_ry) + xaf->eDy;
			rgn.CreateEllipticRgn(x1,y1,x2,y2);
		}
		break;
	case 2:
		{
			CWxRectRgn *prrgn = (CWxRectRgn *)wxrgn;
			x1 = xaf->eM11 * (prrgn->m_left) + xaf->eM21 * (prrgn->m_top) + xaf->eDx;
			y1 = xaf->eM12 * (prrgn->m_left) + xaf->eM22 * (prrgn->m_top) + xaf->eDy;
			x2 = xaf->eM11 * (prrgn->m_left + prrgn->m_width) + xaf->eM21 * (prrgn->m_top + prrgn->m_height) + xaf->eDx;
			y2 = xaf->eM12 * (prrgn->m_left + prrgn->m_width) + xaf->eM22 * (prrgn->m_top + prrgn->m_height) + xaf->eDy;
			rgn.CreateRectRgn(x1,y1,x2,y2);
		}
		break;
	case 3:
		{
			CWxPolygonRgn *pprgn = (CWxPolygonRgn *)wxrgn;
			UINT nCount = (pprgn->m_polys?pprgn->m_polys->size():0);
			if(nCount>0)
			{
				POINT *lpPnt = new POINT[nCount];
				for(UINT i=0; i<nCount; i++)
				{
					CHPVec2D hp = pprgn->m_polys->at(i);
					lpPnt[i].x = xaf->eM11 * hp.x + xaf->eM21 * hp.y + xaf->eDx;
					lpPnt[i].y = xaf->eM12 * hp.x + xaf->eM22 * hp.y + xaf->eDy;
				}
				rgn.CreatePolygonRgn(lpPnt, nCount, ALTERNATE);

				ReleaseArraySafe(lpPnt);
			}
		}
	default:break;
	}

}
void CWxMapCtrl::DrawPosRgn(CDC *pDC, CWxRgn *pRgn, BOOL bShowBorder, BOOL bFillColor, COLORREF cr)
{
	float fScaleX = (m_fZoom * m_coefx)/100.00f, fScaleY = (m_fZoom * m_coefy)/100.00f;// 缩放倍数
	float fDx = m_rectPic.left - m_PntOffset.x*m_fZoom, fDy=m_rectPic.top - m_PntOffset.y*m_fZoom; //平移坐标
	XFORM xAffine = { fScaleX,0,0,fScaleY,fDx,fDy };

	CRgn rgnScale;
	CreateRgnFromWxRgn(rgnScale, pRgn, &xAffine);
	if(rgnScale.m_hObject == NULL)
		return;

	CBrush brhScale(cr);
	if(bFillColor)
	{
//		pDC->FillRgn(&rgnScale, &brhScale);
		UINT nCount = rgnScale.GetRegionData(NULL, 0);
		LPRGNDATA pRgnData1 = new RGNDATA[nCount];
		rgnScale.GetRegionData(pRgnData1, nCount);
		CRect rct = pRgnData1->rdh.rcBound;

		pDC->SelectClipRgn(&rgnScale,RGN_COPY);  
		rgnScale.DeleteObject();  
		// TODO: add draw code for native data here  
		BLENDFUNCTION bl;   
		bl.BlendOp=AC_SRC_OVER;   
		bl.BlendFlags=0;   
		bl.AlphaFormat=0;   
		bl.SourceConstantAlpha=128;   

		CDC memDC;   
		memDC.CreateCompatibleDC(pDC);   
		CBitmap bmp;   
		bmp.CreateCompatibleBitmap(pDC,rct.Width(),rct.Height());  
		CBitmap *pOldBmp = memDC.SelectObject(&bmp);  
		memDC.FillSolidRect(0,0,rct.Width(),rct.Height(),cr);  
		AlphaBlend(pDC->m_hDC,rct.left,rct.top,rct.Width(),rct.Height(),memDC.m_hDC,0,0,rct.Width(),rct.Height(),bl);   
		memDC.SelectObject(pOldBmp);   
		ReleaseDC(&memDC);  
		bmp.DeleteObject();  
		delete []pRgnData1;

		pDC->SelectClipRgn(NULL);  
	}
	else if(bShowBorder)
	{
		pDC->FrameRgn(&rgnScale, &brhScale, 1,1);
	}
	rgnScale.Detach();
	DeleteObject(rgnScale.m_hObject);
}


void CWxMapCtrl::DrawIcon(CDC *pDC, CDC *pSrc, POINT &pt, int w, int h, CString strName, CString strVal)
{
	if(m_rectPic.PtInRect(pt))
	{
//		pDC->BitBlt( pt.x-w, pt.y-h, pt.x+w, pt.y+h, pSrc, 0, 0, SRCCOPY/*SRCINVERT*//* SRCPAINT*//*SRCAND*/);
		::TransparentBlt2(pDC->m_hDC, pt.x-w, pt.y-h,2*w,2*h, pSrc->m_hDC, 0,0,2*w,2*h, RGB(255,255,255));
		if(strName.GetLength() > 0)
		{
			//DrawText(pDC,strName,pt.x+2+r, pt.y+2-r, DT_LEFT | DT_TOP);
			DrawText(pDC,strName, pt.x, pt.y-h, DT_CENTER | DT_BOTTOM);
		}
		if(strVal.GetLength() > 0)
		{
			//DrawText(pDC,strVal,pt.x+2+r, pt.y+2-r, DT_LEFT | DT_TOP);
			DrawText(pDC,strVal,pt.x, pt.y+h, DT_CENTER | DT_TOP);
		}
	};
}

void CWxMapCtrl::DrawIconRotate(CDC *pDC, CDC *pSrc, POINT &pt, int w, int h, double fangle, CString strName, CString strVal)
{
	if(m_rectPic.PtInRect(pt))
	{
		if(fangle > 0.01f || fangle < -0.01f)
		{
			// 获取一个旋转过的Graphics对象  
		//	int nMapMode = ::SetMapMode(pDC->m_hDC, MM_LOENGLISH);
			int nGraphicsMode = RotateDC(pDC->m_hDC, fangle, pt);  
			::TransparentBlt2(pDC->m_hDC, pt.x-w, pt.y-h,2*w,2*h, pSrc->m_hDC, 0,0,2*w,2*h, RGB(255,255,255));
			// 恢复
			RestoreRotatedDC(pDC->m_hDC, nGraphicsMode); 
		//	::SetMapMode(pDC->m_hDC, nMapMode);
		}
		else
		{
			::TransparentBlt2(pDC->m_hDC, pt.x-w, pt.y-h,2*w,2*h, pSrc->m_hDC, 0,0,2*w,2*h, RGB(255,255,255));
		}

		if(strName.GetLength() > 0)
		{
			//DrawText(pDC,strName,pt.x+2+r, pt.y+2-r, DT_LEFT | DT_TOP);
			DrawText(pDC,strName, pt.x, pt.y-h, DT_CENTER | DT_BOTTOM);
		}
		if(strVal.GetLength() > 0)
		{
			//DrawText(pDC,strVal,pt.x+2+r, pt.y+2-r, DT_LEFT | DT_TOP);
			DrawText(pDC,strVal,pt.x, pt.y+h, DT_CENTER | DT_TOP);
		}
	};
}

void CWxMapCtrl::DrawPoint(CDC *pDC, POINT &pt, int r)
{
	if(m_rectPic.PtInRect(pt))
	{
		pDC->Ellipse(pt.x-r, pt.y-r, pt.x+r,  pt.y+r); 

	};
};

void CWxMapCtrl::DrawArrow(CDC *pDC, POINT &pt, double azimuth, int r)
{
	if(m_rectPic.PtInRect(pt))
	{
		double x1, y1, x2, y2, x3, y3, r1, b;
		b = CONST_PI / 6.00f;
		r1 = 20;
		
		x1 = pt.x + r * cos(azimuth);
		y1 = pt.y + r * sin(azimuth);

		x2 = x1 - r1 * cos(azimuth - b);
		y2 = y1 - r1 * sin(azimuth - b);

		x3 = x1 - r1 * cos(azimuth + b);
		y3 = y1 - r1 * sin(azimuth + b);

		pDC->MoveTo(pt);
		pDC->LineTo((int)x1,(int)y1);
		pDC->LineTo((int)x2,(int)y2);
		pDC->MoveTo((int)x1,(int)y1);
		pDC->LineTo((int)x3,(int)y3);
	};
};


BOOL CWxMapCtrl::PtInView(POINT &pt)
{
	return m_rectPic.PtInRect(pt);
}

/*
void CWxMapCtrl::GetShowRects(CRect &rctSrc, CRect &rctDst)
{
	rctSrc.left = m_PntOffset.x;
	rctSrc.top = m_PntOffset.y;
	rctSrc.right = rctSrc.left + (LONG)(m_rectPic.Width()/m_fZoom);
	rctSrc.bottom = rctSrc.top + (LONG)(m_rectPic.Height()/m_fZoom);

	rctDst = m_rectPic;
}
*/

void CWxMapCtrl::DrawCtrl(CDC *pDC, UINT drawMode)
{
	CClientDC dc(this);
	if(pDC == NULL)
	{
		pDC = &dc;
	}

	CBitmap     memBitmap;
	CWxDC      memDC(pDC, &m_rectRedraw, &memBitmap);
	memDC.FillSolidRect(&m_rectRedraw, m_crBack);

	CRect rectShow;
	rectShow.left = m_PntOffset.x;
	rectShow.top = m_PntOffset.y;
	rectShow.right = (LONG)(m_rectPic.Width()/m_fZoom) + rectShow.left;
	rectShow.bottom = (LONG)(m_rectPic.Height()/m_fZoom) + rectShow.top;

	memDC.SetBkMode(TRANSPARENT);

	if(!m_imgLoader1.IsNull())
	{
		POINT ptLT, ptBR;
		ptLT.x = (LONG)(m_rectPic.left - (m_PntOffset.x+m_pntPicOff.x)*m_fZoom);
		ptLT.y = (LONG)(m_rectPic.top - (m_PntOffset.y+m_pntPicOff.y)*m_fZoom);
		ptBR.x = (LONG)(m_imgLoader1.GetWidth()*m_fPicZoomX*m_fZoom) + ptLT.x;
		ptBR.y = (LONG)(m_imgLoader1.GetHeight()*m_fPicZoomY*m_fZoom) + ptLT.y;


		m_imgLoader1.Draw(memDC.m_hDC,CRect(ptLT,ptBR), CRect(0,0,m_imgLoader1.GetWidth(), m_imgLoader1.GetHeight()));
		//DrawBmp();
	}
	for(UINT i=0; i<m_vecImg.size(); i++)
	{
		POINT ptLT, ptBR;
		ptLT.x = (LONG)(m_rectPic.left - (m_PntOffset.x+m_vecImg[i]->offX)*m_fZoom);
		ptLT.y = (LONG)(m_rectPic.top - (m_PntOffset.y+m_vecImg[i]->offY)*m_fZoom);
		ptBR.x = (LONG)(m_vecImg[i]->GetWidth()*m_vecImg[i]->zoomX*m_fZoom) + ptLT.x;
		ptBR.y = (LONG)(m_vecImg[i]->GetHeight()*m_vecImg[i]->zoomY*m_fZoom) + ptLT.y;


		m_vecImg[i]->DrawTo(memDC.m_hDC,CRect(ptLT,ptBR));

	}

	// UserDraw
	// Draw MASK COLOR RECT
	SendUserMessage(MDRAW_MASKCOLOR, (LPARAM)(&memDC));

	if(m_nShowRuler)
	{
		DrawRuler(&memDC, rectShow, m_rectPic, m_fZoom);
	}


	if(m_nGridRadiu > 0)
	{
		DrawGridLine(&memDC, rectShow, m_rectPic, m_fZoom);
//		DrawLogCoords(&memDC, 1000, 10000);
	}

	// UserDraw
	// Draw Anchors And Tracks
	SendUserMessage(drawMode, (LPARAM)(&memDC));

/*	if(m_bShowThumb)
	{
		CRect rect0(0,0,m_bmWidth, m_bmHeight);
		DrawThumbnail(&memDC, rect0, m_rectThumb);
	}*/


	pDC->BitBlt(m_rectRedraw.left, m_rectRedraw.top, m_rectRedraw.Width(), m_rectRedraw.Height(), &memDC, m_rectRedraw.left, m_rectRedraw.top, SRCCOPY);

}


void CWxMapCtrl::DrawText(CDC *pdc, CString strText, int x, int y, DWORD dwType)
{
//	pdc->SetBkMode(TRANSPARENT);

	CRect rectTxt(0,0,0,0);
	pdc->DrawText(strText, &rectTxt, DT_CALCRECT);

	int h = rectTxt.Height();
	int w = rectTxt.Width();

	if(dwType & DT_RIGHT)		{	rectTxt.right = x;		rectTxt.left = x - w;		}
	else if(dwType & DT_CENTER)	{	rectTxt.left = x-w/2;	rectTxt.right = x+w/2;	}
	else							{	rectTxt.left = x;		rectTxt.right = x+w;		dwType |= DT_LEFT;	}

	if(dwType & DT_BOTTOM)		{	rectTxt.bottom = y;		rectTxt.top = y-h;		}
	else if(dwType & DT_VCENTER)	{	rectTxt.top = y - h/2;	rectTxt.bottom = y-h/2+h;}
	else							{	rectTxt.top = y;		rectTxt.bottom = y+h;	dwType |= DT_TOP;	}
	pdc->DrawText(strText, rectTxt, dwType | DT_SINGLELINE);
}

//远距对数处理
double CWxMapCtrl::CalcLogarithms(double inVal, double throshVal)
{
	double outVal = inVal;
	if(throshVal > 0.00f)
	{
		if(inVal > throshVal)
		{
			outVal = throshVal*(pow(10,(inVal-throshVal)/throshVal));
		}
		else if(inVal < -throshVal)
		{
			outVal = -throshVal*(pow(10,(-inVal-throshVal)/throshVal));
		}
	}
	return outVal;
}

// 只显示数字，不显示标尺形状
void CWxMapCtrl::DrawRuler(CDC *pDC,  CRect &rectShow, CRect &rectView, double dZoom)
{
	int nBorder = MAPCTRL_RULER_WIDTH/4;
	CRect rect2;

	int xr,yb,top, bottom, left, right;		
	int x, x1, y, y1,i, ii;
	


	CPen pen;
	pen.CreatePen(PS_SOLID, 1, m_crLine); 
	CPen *pOldPen = pDC->SelectObject(&pen); 

	CFont *pOldFont;
	pOldFont = (CFont *)pDC->SelectObject(&m_font);	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_crLine);
	rect2 = CRect(0,0,0,0);

	CString str =  _T("8888");
	pDC->DrawText(str, &rect2, DT_CALCRECT);
	int w = rect2.Width();

	//水平标尺
	if(m_nShowRuler & MAPCTRL_SHOW_HRULER)
	{
		xr = (int)((m_rectUser.Width() - rectShow.left+m_rectUser.left)*dZoom) + rectView.left;

		top = rectView.top - MAPCTRL_RULER_WIDTH;//+nBorder;
		left = rectView.left;
		right = rectView.right;

		if(rectShow.left < 0)
		{
			left +=  (int)(- rectShow.left*dZoom);
		}

		if(xr < right)
		{
			right = xr;
		}

		y = rectView.top - nBorder+1;
		y1 = rectView.top-1;


		//标尺的格密度由尺标的大小决定——1厘米的整数倍
		ii = (int)(w /(m_coefx*dZoom)) + 1;

		i = (int)(rectShow.left /m_coefx);
		//	i = (int)(rectShow.left / m_nCmPerPixel);
		double xv;

		while(1)
		{
			x = (INT)(rectView.left - rectShow.left*dZoom + i*m_coefx*dZoom);

			xv = CalcLogarithms(i, m_dLogThresholdX);

			i += ii;

			if(x > rectView.right)
				break;

			pDC->MoveTo(x, y);
			pDC->LineTo(x, y1);

//			str.Format(_T("%d"), (i<ii) ? (ii-i):(i-ii));
			str.Format(_T("%d"), (xv>=0) ? (int)xv :(int)(-xv));
			DrawText(pDC, str, x, y-1, DT_CENTER|DT_BOTTOM);
		}
	}

	//垂直标尺
	if(m_nShowRuler & MAPCTRL_SHOW_VRULER)
	{
		yb = (int)((m_rectUser.Height() - rectShow.top+m_rectUser.top)*dZoom) + rectView.top;

		left = rectView.left - MAPCTRL_RULER_WIDTH;//+nBorder;
		top = rectView.top;
		bottom = rectView.bottom;

		if(rectShow.top < 0)
		{
			top += (int)(-rectShow.top*dZoom);
		}

		if(yb < bottom)
		{
			bottom = yb;
		}
		x = rectView.left - nBorder+1;
		x1 = rectView.left-1;

		//标尺的格密度由尺标的大小决定——1厘米的整数倍
		ii = (int)(w /(m_coefy*dZoom)) + 1;
		//	i = (int)(rectShow.top / m_nCmPerPixel);
		i = (int)(rectShow.top /m_coefy);

		double yv;
		while(1)
		{
			y = (INT)(rectView.top - rectShow.top*dZoom + i*m_coefy*dZoom);
			yv = CalcLogarithms(i, m_dLogThresholdY);

			i += ii;

			if(y > rectView.bottom)
				break;

			pDC->MoveTo(x, y);
			pDC->LineTo(x1, y);

//			str.Format(_T("%d"), (i<ii) ? (ii-i):(i-ii));
			str.Format(_T("%d"), (yv>=0) ? (int)yv:(int)(-yv));
			DrawText(pDC, str, rectView.left - MAPCTRL_RULER_WIDTH/2, y, DT_CENTER|DT_VCENTER);
		}
	}


	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);

}

/*

void CWxMapCtrl::DrawRuler(CDC *pDC,  CRect &rectShow, CRect &rectView, double dZoom)
{
	int nBorder = MAPCTRL_RULER_WIDTH/4;
	int nBody = MAPCTRL_RULER_WIDTH - 2*(MAPCTRL_RULER_WIDTH/4);
	CRect rect2;
	COLORREF cr = RGB(192,192,192);
	COLORREF cr1 = RGB(240,240,240);
	COLORREF cr2 = RGB(128,128,128);

	int xr,yb,top, bottom, left, right;		
	int x, x1, y, y1,i, ii;

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0,0,0)); 
	CPen *pOldPen = pDC->SelectObject(&pen); 

	CFont pUnitFont, *pOldFont;
	LOGFONT lf = {0};
	lf.lfEscapement = 0;
	lf.lfItalic = NULL;
	lf.lfUnderline = NULL;
	lf.lfStrikeOut = NULL;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfHeight = nBody - 2;
	_tcscpy(lf.lfFaceName, _T("Arial"));
	pUnitFont.CreateFontIndirect(&lf);
	pOldFont = (CFont *)pDC->SelectObject(&pUnitFont);	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0, 0, 0));
	rect2 = CRect(0,0,0,0);

	CString str =  _T("8888");
	pDC->DrawText(str, &rect2, DT_CALCRECT);
	int w = rect2.Width();



	//水平标尺
	if(m_nShowRuler & MAPCTRL_SHOW_HRULER)
	{
		pDC->FillSolidRect(rectView.left, rectView.top-nBorder, rectView.Width(), nBorder, cr);
		pDC->FillSolidRect(rectView.left, rectView.top-MAPCTRL_RULER_WIDTH, rectView.Width(), nBorder, cr);
		//		xr = (int)((m_bmWidth - rectShow.left+m_PntImgTL.x)*dZoom) + rectView.left;
		xr = (int)((m_rectUser.Width() - rectShow.left+m_rectUser.left)*dZoom) + rectView.left;

		top = rectView.top - MAPCTRL_RULER_WIDTH+nBorder;
		left = rectView.left;
		right = rectView.right;

		if(rectShow.left < 0)
		{
			//FillRectColor(pDC, left, top,(int)((m_PntImgTL.x-rectShow.left)*dZoom), nBody, cr2);
			pDC->FillSolidRect(left, top,(int)((m_rectUser.left-rectShow.left)*dZoom), nBody, cr2);
			left +=  (int)(- rectShow.left*dZoom);
		}

		if(xr < right)
		{
			pDC->FillSolidRect(xr, top, right - xr, nBody, cr2);
			right = xr;
		}

		pDC->FillSolidRect(left, top, right-left, nBody, cr1);

		y = rectView.top - nBorder+1;
		y1 = rectView.top-1;

		//标尺的格密度由尺标的大小决定——1厘米的整数倍
		ii = (int)(w /(m_coefx*dZoom)) + 1;

		i = (int)(rectShow.left /m_coefx);
		//	i = (int)(rectShow.left / m_nCmPerPixel);

		while(1)
		{
			x = (INT)(rectView.left - rectShow.left*dZoom + i*m_coefx*dZoom);
			//		x = rectView.left - rectShow.left*dZoom + i*dZoom*m_nCmPerPixel;
			i += ii;

			if(x > rectView.right)
				break;

			pDC->MoveTo(x, y);
			pDC->LineTo(x, y1);

			str.Format(_T("%d"), (i<ii) ? (ii-i):(i-ii));
			DrawText(pDC, str, x, y-1, DT_CENTER|DT_BOTTOM);
		}
	}

	//垂直标尺
	if(m_nShowRuler & MAPCTRL_SHOW_VRULER)
	{
		pDC->FillSolidRect(rectView.left-nBorder, rectView.top, nBorder, rectView.Height(), cr);
		pDC->FillSolidRect(rectView.left-MAPCTRL_RULER_WIDTH, rectView.top, nBorder, rectView.Height(), cr);
		//		yb = (int)((m_bmHeight - rectShow.top+m_PntImgTL.y)*dZoom) + rectView.top;
		yb = (int)((m_rectUser.Height() - rectShow.top+m_rectUser.top)*dZoom) + rectView.top;

		left = rectView.left - MAPCTRL_RULER_WIDTH+nBorder;
		top = rectView.top;
		bottom = rectView.bottom;

		if(rectShow.top < 0)
		{
			//			FillRectColor(pDC, left, top,nBody, (int)((m_PntImgTL.y-rectShow.top)*dZoom), cr2);
			pDC->FillSolidRect(left, top,nBody, (int)((m_rectUser.top-rectShow.top)*dZoom), cr2);
			top += (int)(-rectShow.top*dZoom);
		}

		if(yb < bottom)
		{
			pDC->FillSolidRect(left, yb, nBody, bottom - yb, cr2);
			bottom = yb;
		}

		pDC->FillSolidRect(left, top, nBody, bottom-top, cr1);

		x = rectView.left - nBorder+1;
		x1 = rectView.left-1;

		//标尺的格密度由尺标的大小决定——1厘米的整数倍
		ii = (int)(w /(m_coefy*dZoom)) + 1;
		//	i = (int)(rectShow.top / m_nCmPerPixel);
		i = (int)(rectShow.top /m_coefy);
		while(1)
		{
			y = (INT)(rectView.top - rectShow.top*dZoom + i*m_coefy*dZoom);
			//		y = rectView.top - rectShow.top*dZoom + i*dZoom*m_nCmPerPixel;
			i += ii;

			if(y > rectView.bottom)
				break;

			pDC->MoveTo(x, y);
			pDC->LineTo(x1, y);

			str.Format(_T("%d"), (i<ii) ? (ii-i):(i-ii));
			DrawText(pDC, str, rectView.left - MAPCTRL_RULER_WIDTH/2, y, DT_CENTER|DT_VCENTER);
		}
	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
	pUnitFont.DeleteObject();

	if((m_nShowRuler & MAPCTRL_SHOW_BRULER) == MAPCTRL_SHOW_BRULER)
	{
		pDC->FillSolidRect(rectView.left-MAPCTRL_RULER_WIDTH, rectView.top-MAPCTRL_RULER_WIDTH, MAPCTRL_RULER_WIDTH, MAPCTRL_RULER_WIDTH, cr);
	}
}
*/

void CWxMapCtrl::DrawThumbnail(CDC *pDC,  CRect &rectShow, CRect &rect)
{
	if(!m_imgLoader1.IsNull())
	{
//		m_imgLoader1.Draw(pDC->m_hDC, rect, CRect(0,0,m_bmWidth, m_bmHeight));
	}

	//TO BE FIXED!!
	DrawRectRgn(pDC, rect.CenterPoint().x, rect.CenterPoint().y, rect.Width()/2, rect.Height()/2, RGB(128,128,128));

}

void CWxMapCtrl::DrawPathLine(CDC *pDC, int x, int y, int x0, int y0, int r, COLORREF color, unsigned char path)
{
	CPen *pOldPen;
	CPen PenLine;
	if(path == 1)
	{
		PenLine.CreatePen(PS_SOLID, 2, color); 
		pOldPen = pDC->SelectObject(&PenLine); 
		pDC->MoveTo(x-2*r, y-2*r);
		pDC->LineTo(x+2*r, y-2*r);
		pDC->LineTo(x+r, y-3*r);
	}
	else if(path&0x2)
	{
		PenLine.CreatePen(PS_DASHDOT, 1, color); 
		pOldPen = pDC->SelectObject(&PenLine); 
		pDC->MoveTo(x0, y0);
		pDC->LineTo(x, y);
	}
	else
		return;
	pDC->SelectObject(pOldPen);
}


void CWxMapCtrl::DrawRectRgn(CDC *pDC, int x, int y, int w, int h, COLORREF crPen)
{
	CPen *pOldPen;
	CPen PenLine;
	PenLine.CreatePen(PS_SOLID, 1, crPen); 
	pOldPen = pDC->SelectObject(&PenLine); 
	pDC->MoveTo(x-w, y-h);
	pDC->LineTo(x-w, y+h);
	pDC->LineTo(x+w, y+h);
	pDC->LineTo(x+w, y-h);
	pDC->LineTo(x-w, y-h);
	pDC->SelectObject(pOldPen);
}

void CWxMapCtrl::DrawRect2View(CDC *pDC, CHPRect &rectPix)
{
	CHPVec2D ptLT,ptRB;
	Pix2View(rectPix.left,rectPix.top,ptLT);
	Pix2View(rectPix.right,rectPix.bottom,ptRB);

//	CPen *pOldPen;
//	CPen PenLine;
//	PenLine.CreatePen(PS_SOLID, 4, RGB(128,255,128)); 
//	pOldPen = pDC->SelectObject(&PenLine); 

	pDC->DrawFocusRect(CRect(ptLT.Floor(),ptRB.Ceil()));
//	pDC->Draw3dRect(CRect(ptLT,ptRB),RGB(128,255,128),RGB(128,255,128));
//	pDC->MoveTo(ptLT.);
//	pDC->LineTo(ptRB.x,ptLT.y);
//	pDC->LineTo(ptRB);
//	pDC->LineTo(ptLT.x,ptRB.y);
//	pDC->LineTo(ptLT);
//	pDC->SelectObject(pOldPen);
}

void CWxMapCtrl::FillRect2View(CDC *pDC, CHPRect &rectPix, COLORREF cr, CHPRect &rectExclude)
{
	CBrush br;
	br.CreateSolidBrush(cr);

	CRgn rg1,rg2;
	CHPVec2D ptLT1,ptRB1,ptLT2,ptRB2;
	Pix2View(rectPix.left,rectPix.top,ptLT1);
	Pix2View(rectPix.right,rectPix.bottom,ptRB1);
	rg1.CreateRectRgn(floor(ptLT1.x),floor(ptLT1.y),floor(ptRB1.x),floor(ptRB1.y));

	if(!rectExclude.IsRectEmpty())
	{
		Pix2View(rectExclude.left,rectExclude.top,ptLT2);
		Pix2View(rectExclude.right,rectExclude.bottom,ptRB2);
		rg2.CreateRectRgn(floor(ptLT2.x),floor(ptLT2.y),floor(ptRB2.x),floor(ptRB2.y));
		rg1.CombineRgn(&rg1,&rg2,RGN_XOR);
		rg2.DeleteObject();
	}

	pDC->FillRgn(&rg1,&br);
	rg1.DeleteObject();
}



void CWxMapCtrl::DrawGridLine(CDC *pDC,  CRect &rectShow, CRect &rectView, double dZoom)
{
	//网格线从图像中心向四周发散
//	int x0 = (m_bmWidth - m_nGridRadiu*m_coefx*xGridCnt)/2;
//	int y0 = (m_bmHeight - m_nGridRadiu*m_coefy*yGridCnt)/2;

	//网格线从图像的TopLeft开始
	int x0=0;
	int y0=0;

	int x, y, x1,y1, i;
//	int yb = (int)((m_bmHeight - rectShow.top)*dZoom) + rectView.top;
//	int xr = (int)((m_bmWidth - rectShow.left)*dZoom) + rectView.left;

	CPen pen;
	pen.CreatePen(PS_DOT, 1, m_crGrid); 
	CPen *pOldPen = pDC->SelectObject(&pen); 


	//只在图像可显范围内绘制网格线
	//i = 0;
	//y = (rectShow.top > 0) ? rectView.top : (rectView.top - rectShow.top*dZoom);
	//y1 = (yb > rectView.bottom) ? rectView.bottom : yb; 

	//在显示区域内绘制网格线
	i = (int)((rectShow.left - x0 - rectView.left/dZoom)/(m_nGridRadiu*m_coefx/100));
	y = rectView.top+1;
	y1 = rectView.bottom-1;


	while(1)
	{
		x = (LONG)((x0+m_nGridRadiu*m_coefx*i/100 - rectShow.left)*dZoom) + rectView.left;

	//	x = CalcLogarithms(x, m_dLogThresholdX);

		i++;

		//只在图像可显范围内绘制网格线
		//if(x<rectView.left)					continue;
		//else if(x > rectView.right || x > xr)	break;

		//在显示区域内绘制网格线
		if(x<rectView.left) continue;
		else if(x>rectView.right)
			break;
		pDC->MoveTo(x, y);
		pDC->LineTo(x, y1);

	};

	//只在图像可显范围内绘制网格线
	//i = 0;
	//x = (rectShow.left > 0)?rectView.left : (rectView.left - rectShow.left*dZoom);
	//x1 = (xr > rectView.right) ? rectView.right : xr;

	//在显示区域内绘制网格线
	i = (int)((rectShow.top - y0 - rectView.top/dZoom)/(m_nGridRadiu*m_coefy/100));
	x = rectView.left+1;
	x1 = rectView.right-1;

	while(1)
	{
		y = (LONG)((y0+m_nGridRadiu*m_coefy*i/100 - rectShow.top)*dZoom) + rectView.top;

	//	y = CalcLogarithms(y, m_dLogThresholdY);

		i++;

		//只在图像可显范围内绘制网格线
		//if(y<rectView.top)						continue;
		//else if(y > rectView.bottom || y > yb)	break;

		//在显示区域内绘制网格线
		if(y<rectView.top) continue;
		else if(y > rectView.bottom)
			break;

		pDC->MoveTo(x, y);
		pDC->LineTo(x1, y);
	};

	CPen pen1;
	pen1.CreatePen(PS_SOLID, 1, RGB(128,0,0)); 
	pDC->SelectObject(&pen1);
	
	x = (LONG)((x0 - rectShow.left)*dZoom) + rectView.left;
	if(x>rectView.left && x<rectView.right)
	{
		pDC->MoveTo(x, rectView.top+1);
		pDC->LineTo(x, rectView.bottom-1);
	}

	y = (LONG)((y0 - rectShow.top)*dZoom) + rectView.top;
	if(y>rectView.top && y<rectView.bottom)
	{
		pDC->MoveTo(rectView.left+1, y);
		pDC->LineTo(rectView.right-1, y);
	}

	pDC->SelectObject(pOldPen);

}

/*
void CWxMapCtrl::DrawLogCoords(CDC* pDC, double StartFreq, double StopFreq)
{
	CString note;
	CRect rect;
	this->GetClientRect(&rect);
	INT ORIGIN_X=rect.left+rect.Width()/10;    // 坐标原点X坐标
	INT ORIGIN_Y=rect.bottom-rect.Height()/10;    // 坐标原点Y坐标
	INT COORD_TOP=rect.top+rect.Height()/10;    // 坐标系最高点Y坐标
	INT COORD_RIGHT=rect.right-rect.Width()/10;    // 坐标系最右点X坐标

	pDC->MoveTo(ORIGIN_X,ORIGIN_Y);
	pDC->LineTo(ORIGIN_X,COORD_TOP);    // 画纵坐标轴
	pDC->MoveTo(ORIGIN_X,ORIGIN_Y);
	pDC->LineTo(COORD_RIGHT,ORIGIN_Y);    // 画横坐标轴
	note.Format(L"%4.2fM",StartFreq);
	pDC->TextOutW(ORIGIN_X-15,ORIGIN_Y+5,note);    // 原点标注

	// 画横格线
	double yInc=(ORIGIN_Y-COORD_TOP)/9.0;
	double tmp=(double)ORIGIN_Y-yInc;
	for(int i=0;i<8;++i)
	{
		pDC->MoveTo(ORIGIN_X,(int)tmp);
		pDC->LineTo(COORD_RIGHT,(int)tmp);
		note.Format(L"%d",10*(i+1));
		pDC->TextOutW(ORIGIN_X-25,(int)tmp-8,note);    // z纵坐标标注
		tmp-=yInc;
	}
	pDC->MoveTo(ORIGIN_X,COORD_TOP);
	pDC->LineTo(COORD_RIGHT,COORD_TOP);
	note.Format(L"%d",90);
	pDC->TextOutW(ORIGIN_X-25,COORD_TOP-8,note);// z纵坐标标注
	note.Format(L"[dBuV]");
	pDC->TextOutW(ORIGIN_X-25,COORD_TOP-28,note);// z纵坐标标注

	// 画竖格线
	//    |    .  .|    .    
	//    |    .  .|    .
	//    |    .  .|    .
	//    |    .  .|    .
	//    |    .  .|    .
	//    |    .  .|    . 

	//    定义：标准频率，是指0.01MHz、0.1MHz、1MHz
	//        10MHZ、100MHz、1000MHz等频率

	double stdStartFreq,stdStopFreq;//  标准频率起始点、终止点
	// 格式化标准起始频率
	for(int i=0;i<7;++i)
	{
		if(StartFreq==pow((double)10,i-2))
		{
			stdStartFreq=StartFreq;
			break;
		}
		else if(StartFreq>pow((double)10,i-2) && StartFreq<pow((double)10,i-1))
		{
			stdStartFreq=pow((double)10,i-1);
			break;
		}
	}
	// 格式化标准终止频率
	for(int i=0;i<7;++i)
	{
		if(StopFreq==pow((double)10,i-2))
		{
			stdStopFreq=StopFreq;
			break;
		}
		else if(StopFreq>pow((double)10,i-2) && StopFreq<pow((double)10,i-1))
		{
			stdStopFreq=pow((double)10,i-2);
			break;
		}
	}

	double totalRatio=log10(StopFreq/StartFreq);    // 整个频率段所占的比例
	int totalLen=COORD_RIGHT-ORIGIN_X;    // 总长度

	if(stdStartFreq<=stdStopFreq)    // 起始频率和终止频率相差大于等于一个数量级，如StartFreq=2.5，StopFreq=200
	{        
		double startRatio=log10(stdStartFreq/StartFreq)/totalRatio;    // 起始频率与标准起始频率之间的部分所占的比例
		double stdRatio=log10(stdStopFreq/stdStartFreq)/totalRatio;    // 标准起始频率和标准终止频率之间部分所占的比例
		double stopRatio=log10(StopFreq/stdStopFreq)/totalRatio;    // 标准终止频率和终止频率之间部分所占的比例

		double xStdStart=ORIGIN_X+startRatio*totalLen;    // 标准起始频率所在的X坐标
		pDC->MoveTo((int)xStdStart,ORIGIN_Y);
		pDC->LineTo((int)xStdStart,COORD_TOP);    // 绘制标准起始频率所在的垂直实线
		note.Format(L"%4.2fM",stdStartFreq);
		pDC->TextOutW((int)xStdStart-15,ORIGIN_Y+5,note);    // 坐标标注

		double xStdStop=xStdStart+stdRatio*totalLen;    // 标准终止频率所在的X坐标
		pDC->MoveTo((int)xStdStop,ORIGIN_Y);
		pDC->LineTo((int)xStdStop,COORD_TOP);    // 绘制标准终止频率所在的垂直实线
		note.Format(L"%4.2fM",stdStopFreq);
		pDC->TextOutW((int)xStdStop-15,ORIGIN_Y+5,note);    // 坐标标注

		double xtmp=xStdStart;
		for(int i=0;i<(int)(stdRatio*totalRatio);++i)    // 绘制标准起始频率和标准终止频率之间的垂直实线
		{
			xtmp+=totalLen/totalRatio;
			pDC->MoveTo((int)xtmp,ORIGIN_Y);
			pDC->LineTo((int)xtmp,COORD_TOP);    // 绘制个关键频率所在的垂直实线
			note.Format(L"%4.2fM",stdStartFreq*pow((double)10,i+1));
			pDC->TextOutW((int)xtmp-15,ORIGIN_Y+5,note);    // 坐标标注
		}

		pDC->MoveTo(COORD_RIGHT,ORIGIN_Y);    // 终止频率X坐标为COORD_RIGHT
		pDC->LineTo(COORD_RIGHT,COORD_TOP);
		note.Format(L"%4.2fM",StopFreq);
		pDC->TextOutW(COORD_RIGHT-15,ORIGIN_Y+5,note);    // 坐标标注

		CPen dotPen(PS_DOT,1,RGB(0,0,0));
		CPen* pOldPen=pDC->SelectObject(&dotPen);

		// 绘制起始频率与标准起始频率之间的虚线
		double tmpFreqL=stdStartFreq-stdStartFreq/10;    // 低频率
		double tmpFreqH=stdStartFreq;    // 高频率
		double tmpRatio;
		xtmp=xStdStart;
		while((tmpFreqL-StartFreq)>0.000001)    ///// 由于double型浮点数精度的问题，不得不采用这种方法进行判断 /////
		{
			tmpRatio=log10(tmpFreqH/tmpFreqL)/totalRatio;
			xtmp-=tmpRatio*totalLen;
			pDC->MoveTo((int)xtmp,ORIGIN_Y);
			pDC->LineTo((int)xtmp,COORD_TOP);    // 绘制虚线
			if(tmpRatio>=0.06)
			{
				note.Format(L"%4.2fM",tmpFreqL);
				pDC->TextOutW(xtmp-15,ORIGIN_Y+5,note);    // 坐标标注
			}

			// 如果两个频率之间的间隔所占长度大于等于横坐标全长的10%,
			// 用灰色虚线画出中间频率线
			if(tmpRatio>=0.10)
			{
				CPen grayPen(PS_DOT,0.5,RGB(192,192,192));
				CPen* pDotPen=pDC->SelectObject(&grayPen);    // 灰色画笔

				double grayInc;
				if(tmpRatio<0.15)
					grayInc=(tmpFreqH-tmpFreqL)/2;    // 画1条灰色虚线
				else if(tmpRatio>=0.15 && tmpRatio<0.20)
					grayInc=(tmpFreqH-tmpFreqL)/4;    // 画3条灰色虚线
				else if(tmpRatio>=0.20)
					grayInc=(tmpFreqH-tmpFreqL)/6;    // 画5条灰色虚线

				double grayFreqH=tmpFreqL+grayInc;
				double grayFreqL=tmpFreqL;
				double grayLen=tmpRatio*totalLen;
				double xgray=xtmp;
				double grayRatio;
				while(tmpFreqH-grayFreqH>0.000001)
				{
					grayRatio=log10(grayFreqH/grayFreqL)/log10(tmpFreqH/tmpFreqL);
					xgray+=grayRatio*grayLen;
					pDC->MoveTo(xgray,ORIGIN_Y);
					pDC->LineTo(xgray,COORD_TOP);    // 绘制灰色虚线
					grayFreqH+=grayInc;
					grayFreqL+=grayInc;
				}

				pDC->SelectObject(pDotPen);
			}

			tmpFreqH-=stdStartFreq/10;
			tmpFreqL-=stdStartFreq/10;
		}
		// 起始频率和第一条黑色虚线之间的灰色虚线
		// 如果两个频率之间的间隔所占长度大于等于横坐标全长的10%
		// 用灰色虚线画出全部中间频率线
		tmpRatio=log10(tmpFreqH/StartFreq)/totalRatio;
		if(tmpRatio>=0.10)
		{
			CPen grayPen(PS_DOT,0.5,RGB(192,192,192));
			CPen* pDotPen=pDC->SelectObject(&grayPen);    // 灰色画笔

			double grayInc;
			if(tmpRatio<0.15)
				grayInc=(tmpFreqH-tmpFreqL)/2;    // 画1条灰色虚线
			else if(tmpRatio>=0.15 && tmpRatio<0.20)
				grayInc=(tmpFreqH-tmpFreqL)/4;    // 画2条灰色虚线
			else if(tmpRatio>=0.20  && tmpRatio<0.25)
				grayInc=(tmpFreqH-tmpFreqL)/6;    // 画5条灰色虚线
			else if(tmpRatio>=0.25  && tmpRatio<0.30)
				grayInc=(tmpFreqH-tmpFreqL)/8;    // 画7条灰色虚线
			else if(tmpRatio>=0.30 && tmpRatio<0.4)
				grayInc=(tmpFreqH-tmpFreqL)/9;    // 画8条灰色虚线
			else if(tmpRatio>=0.40)
				grayInc=(tmpFreqH-tmpFreqL)/10;    // 画8条灰色虚线

			double grayFreqH=tmpFreqH;
			double grayFreqL=tmpFreqH-grayInc;
			double xgray=xtmp;
			double grayRatio;
			while(grayFreqL-StartFreq>0.000001)
			{
				grayRatio=log10(grayFreqH/grayFreqL)/log10(tmpFreqH/StartFreq);
				xgray-=(xtmp-ORIGIN_X)*grayRatio;
				pDC->MoveTo(xgray,ORIGIN_Y);
				pDC->LineTo(xgray,COORD_TOP);    // 绘制灰色虚线
				grayFreqH-=grayInc;
				grayFreqL-=grayInc;
			}

			pDC->SelectObject(pDotPen);
		}

		// 绘制标准起始频率和标准终止频率之间的垂直虚线
		tmp=xStdStart;
		double tmpFreq=stdStartFreq;
		for(int i=0;i<(int)(stdRatio*totalRatio);++i)
		{
			xtmp=tmp;    // tmp代表关键频率所在的X坐标，xtmp代表将要绘制的虚线的X坐标

			tmpFreqL=tmpFreq;
			tmpFreqH=2*tmpFreq;
			// 由低频向高频绘制虚线
			for(int j=0;j<8;++j)
			{
				tmpRatio=log10(tmpFreqH/tmpFreqL)/totalRatio;
				xtmp+=tmpRatio*totalLen;
				pDC->MoveTo(xtmp,ORIGIN_Y);
				pDC->LineTo(xtmp,COORD_TOP);    // 绘制虚线
				if(tmpRatio>=0.06)
				{
					note.Format(L"%4.2fM",tmpFreqH);
					pDC->TextOutW(xtmp-15,ORIGIN_Y+5,note);    // 坐标标注
				}

				// 如果两个频率之间的间隔所占长度大于等于横坐标全长的10%,
				// 用灰色虚线画出中间频率线
				if(tmpRatio>=0.10)
				{
					CPen grayPen(PS_DOT,0.5,RGB(192,192,192));
					CPen* pDotPen=pDC->SelectObject(&grayPen);    // 灰色画笔

					double grayInc;
					if(tmpRatio<0.15)
						grayInc=(tmpFreqH-tmpFreqL)/2;    // 画1条灰色虚线
					else if(tmpRatio>=0.15 && tmpRatio<0.20)
						grayInc=(tmpFreqH-tmpFreqL)/4;    // 画3条灰色虚线
					else if(tmpRatio>=0.20)
						grayInc=(tmpFreqH-tmpFreqL)/6;    // 画3条灰色虚线

					double grayFreqH=tmpFreqL+grayInc;
					double grayFreqL=tmpFreqL;
					double grayLen=tmpRatio*totalLen;
					double xgray=xtmp-grayLen;
					double grayRatio;
					while(tmpFreqH-grayFreqH>0.00001)
					{
						grayRatio=log10(grayFreqH/grayFreqL)/log10(tmpFreqH/tmpFreqL);
						xgray+=grayRatio*grayLen;
						pDC->MoveTo(xgray,ORIGIN_Y);
						pDC->LineTo(xgray,COORD_TOP);    // 绘制灰色虚线
						grayFreqH+=grayInc;
						grayFreqL+=grayInc;
					}

					pDC->SelectObject(pDotPen);
				}

				tmpFreqL+=tmpFreq;
				tmpFreqH+=tmpFreq;
			}
			tmpFreq*=10;    // tmpFreq代表关键频率
			tmp+=totalLen/totalRatio;
		}

		// 绘制标准终止频率和终止频率之间的垂直虚线
		xtmp=xStdStop;
		tmpFreqL=stdStopFreq;
		tmpFreqH=2*stdStopFreq;
		while((StopFreq-tmpFreqH)>0.000001)
		{
			tmpRatio=log10(tmpFreqH/tmpFreqL)/totalRatio;
			xtmp+=tmpRatio*totalLen;
			pDC->MoveTo(xtmp,ORIGIN_Y);
			pDC->LineTo(xtmp,COORD_TOP);    // 绘制虚线
			if(tmpRatio>=0.06)
			{
				note.Format(L"%4.2fM",tmpFreqH);
				pDC->TextOutW(xtmp-15,ORIGIN_Y+5,note);    // 坐标标注
			}

			// 如果两个频率之间的间隔所占长度大于等于横坐标全长的10%
			// 用灰色虚线画出全部中间频率线
			if(tmpRatio>=0.10)
			{
				CPen grayPen(PS_DOT,0.5,RGB(192,192,192));
				CPen* pDotPen=pDC->SelectObject(&grayPen);    // 灰色画笔

				double grayInc;
				if(tmpRatio<0.15)
					grayInc=(tmpFreqH-tmpFreqL)/2;    // 画1条灰色虚线
				else if(tmpRatio>=0.15 && tmpRatio<0.20)
					grayInc=(tmpFreqH-tmpFreqL)/4;    // 画3条灰色虚线
				else if(tmpRatio>=0.20)
					grayInc=(tmpFreqH-tmpFreqL)/6;    // 画5条灰色虚线

				double grayFreqH=tmpFreqL+grayInc;
				double grayFreqL=tmpFreqL;
				double grayLen=tmpRatio*totalLen;    // 要画灰色虚线频率之间的距离
				double xgray=xtmp-grayLen;
				double grayRatio;
				while(tmpFreqH-grayFreqH>0.000001)
				{
					grayRatio=log10(grayFreqH/grayFreqL)/log10(tmpFreqH/tmpFreqL);
					xgray+=grayRatio*grayLen;
					pDC->MoveTo(xgray,ORIGIN_Y);
					pDC->LineTo(xgray,COORD_TOP);    // 绘制灰色虚线
					grayFreqH+=grayInc;
					grayFreqL+=grayInc;
				}

				pDC->SelectObject(pDotPen);
			}

			tmpFreqL+=stdStopFreq;
			tmpFreqH+=stdStopFreq;
		}

		// 黑色虚线和终止频率之间的灰色虚线
		// 如果两个频率之间的间隔所占长度大于等于横坐标全长的10%
		// 用灰色虚线画出全部中间频率线
		tmpRatio=log10(StopFreq/tmpFreqL)/totalRatio;
		if(tmpRatio>=0.10)
		{
			CPen grayPen(PS_DOT,0.5,RGB(192,192,192));
			CPen* pDotPen=pDC->SelectObject(&grayPen);    // 灰色画笔

			double grayInc;
			if(tmpRatio<0.15)
				grayInc=(tmpFreqH-tmpFreqL)/2;    // 画1条灰色虚线
			else if(tmpRatio>=0.15 && tmpRatio<0.20)
				grayInc=(tmpFreqH-tmpFreqL)/4;    // 画2条灰色虚线
			else if(tmpRatio>=0.20  && tmpRatio<0.25)
				grayInc=(tmpFreqH-tmpFreqL)/6;    // 画5条灰色虚线
			else if(tmpRatio>=0.25  && tmpRatio<0.30)
				grayInc=(tmpFreqH-tmpFreqL)/8;    // 画7条灰色虚线
			else if(tmpRatio>=0.30 && tmpRatio<0.4)
				grayInc=(tmpFreqH-tmpFreqL)/9;    // 画8条灰色虚线
			else if(tmpRatio>=0.40)
				grayInc=(tmpFreqH-tmpFreqL)/10;    // 画9条灰色虚线

			double grayFreqH=tmpFreqL+grayInc;
			double grayFreqL=tmpFreqL;
			double xgray=xtmp;
			double grayRatio;
			while(StopFreq-grayFreqL>0.000001)
			{
				grayRatio=log10(grayFreqH/grayFreqL)/log10(StopFreq/tmpFreqL);
				xgray+=(COORD_RIGHT-xtmp)*grayRatio;
				pDC->MoveTo(xgray,ORIGIN_Y);
				pDC->LineTo(xgray,COORD_TOP);    // 绘制灰色虚线
				grayFreqH+=grayInc;
				grayFreqL+=grayInc;
			}

			pDC->SelectObject(pDotPen);
		}

		pDC->SelectObject(pOldPen);
	}
	else    // 起始频率和终止频率在同一数量级，如StartFreq=200，StopFreq=500
	{
		pDC->MoveTo(COORD_RIGHT,ORIGIN_Y);    
		pDC->LineTo(COORD_RIGHT,COORD_TOP);    // 最右端边界线
		note.Format(L"%4.2fM",StopFreq);
		pDC->TextOutW(COORD_RIGHT-15,ORIGIN_Y+5,note);    // 坐标标注

		CPen dotPen(PS_DOT,1,RGB(0,0,0));
		CPen* pOldPen=pDC->SelectObject(&dotPen);

		double unitLen=totalLen/totalRatio;    // 标准频率之间应有的距离
		double xStdStop=ORIGIN_X-log10(StartFreq/stdStopFreq)*unitLen;    // 标准终止频率(此时在起始频率的左边)的X坐标
		double xtmp=xStdStop;
		double tmpFreqH=2*stdStopFreq;
		double tmpFreqL=stdStopFreq;
		double tmpRatio;

		// 根据标准终止频率(此时在起始频率的左边)绘制标准虚线
		while((StartFreq-tmpFreqH>0.000001))    // 找到第一根标准虚线的X坐标
		{
			tmpRatio=log10(tmpFreqH/tmpFreqL);
			xtmp+=tmpRatio*unitLen;
			tmpFreqH+=stdStopFreq;
			tmpFreqL+=stdStopFreq;
		}

		while(StopFreq-tmpFreqL>0.000001)
		{            
			tmpRatio=log10(tmpFreqH/tmpFreqL);            
			if(tmpFreqL>StartFreq)
			{
				pDC->MoveTo(xtmp,ORIGIN_Y);
				pDC->LineTo(xtmp,COORD_TOP);    // 绘制虚线
				if(tmpRatio>=0.06)
				{
					note.Format(L"%4.2fM",tmpFreqL);
					pDC->TextOutW(xtmp-15,ORIGIN_Y+5,note);    //坐标标注
				}
			}

			// 如果两个频率之间的间隔所占长度大于等于横坐标全长的10%
			// 用灰色虚线画出全部中间频率线
			double grayTmpRatio=tmpRatio/log10(StopFreq/StartFreq);
			if(grayTmpRatio>=0.10)
			{
				CPen grayPen(PS_DOT,0.5,RGB(192,192,192));
				CPen* pDotPen=pDC->SelectObject(&grayPen);    // 灰色画笔

				double grayInc;
				if(grayTmpRatio<0.15)
					grayInc=(tmpFreqH-tmpFreqL)/2;    // 画1条灰色虚线
				else if(grayTmpRatio>=0.15 && grayTmpRatio<0.20)
					grayInc=(tmpFreqH-tmpFreqL)/4;    // 画2条灰色虚线
				else if(grayTmpRatio>=0.20  && grayTmpRatio<0.25)
					grayInc=(tmpFreqH-tmpFreqL)/6;    // 画5条灰色虚线
				else if(grayTmpRatio>=0.25  && grayTmpRatio<0.30)
					grayInc=(tmpFreqH-tmpFreqL)/8;    // 画7条灰色虚线
				else if(grayTmpRatio>=0.30 && grayTmpRatio<0.35)
					grayInc=(tmpFreqH-tmpFreqL)/9;    // 画8条灰色虚线
				else if(grayTmpRatio>=0.35)
					grayInc=(tmpFreqH-tmpFreqL)/10;    // 画9条灰色虚线

				double grayFreqH=tmpFreqL+grayInc;
				double grayFreqL=tmpFreqL;
				double grayLen=tmpRatio*unitLen;    // 要画的灰色虚线频率之间的距离
				double xgray=xtmp;
				double grayRatio;
				while((tmpFreqH-grayFreqH>0.000001) && grayFreqH<StopFreq)
				{
					grayRatio=log10(grayFreqH/grayFreqL)/log10(tmpFreqH/tmpFreqL);
					xgray+=grayRatio*grayLen;
					pDC->MoveTo(xgray,ORIGIN_Y);
					pDC->LineTo(xgray,COORD_TOP);    // 绘制灰色虚线
					grayFreqH+=grayInc;
					grayFreqL+=grayInc;
				}

				pDC->SelectObject(pDotPen);
			}

			xtmp+=tmpRatio*unitLen;
			tmpFreqH+=stdStopFreq;
			tmpFreqL+=stdStopFreq;    
		}

		pDC->SelectObject(pOldPen);
	}
}
*/

void CWxMapCtrl::DrawTrack(CDC *pDC, int x0, int y0, int x, int y, COLORREF color)
{
	CPen PenLine, *pOldPen;
	PenLine.CreatePen(PS_DASHDOT, 1, color); 
	pOldPen = pDC->SelectObject(&PenLine); 
	pDC->MoveTo(x0, y0);
	pDC->LineTo(x, y);
	pDC->SelectObject(pOldPen);
}

void CWxMapCtrl::DrawPoint(CDC *pDC, int x, int y, int r, int s, COLORREF c)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, c); 
	CPen *pOldPen = pDC->SelectObject(&pen); 

	if(s == 0)
	{
		CBrush *pOldBrush;
		CBrush brsh; 
		brsh.CreateSolidBrush(c); 

		pOldBrush = pDC->SelectObject(&brsh); 
		pDC->Ellipse(x-r, y-r, x+r,  y+r); 
		pDC->SelectObject(pOldBrush); 

	}
	else if(s == 1)
	{
		CBrush brsh; 
		CBrush *pOldBrush = (CBrush *)pDC->SelectStockObject(NULL_BRUSH);
		for(int i = r; i>1; i -= 2)
			pDC->Ellipse(x-i, y-i, x+i, y+i); 
		pDC->SelectObject(pOldBrush); 

		pDC->MoveTo(x-r/2, y+ 2*r);
		pDC->LineTo(x,y);
		pDC->LineTo(x+r/2, y+2*r);
		pDC->LineTo(x-r/2, y+2*r);

		pDC->SelectObject(pOldBrush); 
	}
	else if(s == 2)
	{
		pDC->MoveTo(x-r, y);
		pDC->LineTo(x+r, y);
		pDC->MoveTo(x, y-r);
		pDC->LineTo(x, y+r);
	}
	else if(s == 3)  // 空心
	{
		CBrush *pOldBrush = (CBrush *)pDC->SelectStockObject(NULL_BRUSH);

		pDC->Ellipse(x-r, y-r, x+r,  y+r); 
		pDC->SelectObject(pOldBrush); 

	}
	else if(s == -1)
	{
		pDC->SetPixel(x,y,c);
	}

	pDC->SelectObject(pOldPen);
}




BOOL CWxMapCtrl::Save(LPCTSTR lpszFilename)
{
	return (m_imgLoader1.Save(lpszFilename) == S_OK);
}

BOOL CWxMapCtrl::Load(LPCTSTR lpszFilename)
{
	m_imgLoader1.Destroy();
	
	BOOL bMapLoad = (m_imgLoader1.Load(lpszFilename) == S_OK);
	if(bMapLoad)
	{
//		m_bmWidth = m_imgLoader1.GetWidth();
//		m_bmHeight = m_imgLoader1.GetHeight();
		m_bRectsNeedRecalculate = TRUE;

	}

	m_bAnchorDraw = FALSE;
	m_bCtrlAlwaysFocus = FALSE;
//	m_fZoom = 1.0f;
	//		m_bShowGrid = FALSE;
	m_PntOffset = CPoint(0,0);
	OnPaint();

	return bMapLoad;
}

BOOL CWxMapCtrl::Load(LPCTSTR szPath, CRect realRect, COLORREF crMask)
{
	ImageObject* imgobj=new ImageObject;
	BOOL bMapLoad = (imgobj->Load(szPath) == S_OK);
	if(bMapLoad)
	{
		imgobj->zoomX = (double)realRect.Width()*m_coefx/(100.00f*imgobj->GetWidth());
		imgobj->zoomY = (double)realRect.Height()*m_coefy/(100.00f*imgobj->GetHeight());

		imgobj->offX = -realRect.left*m_coefx/100.00f;
		imgobj->offY = -realRect.top*m_coefx/100.00f;

		m_bRectsNeedRecalculate = TRUE;
		m_vecImg.push_back(imgobj);

	}

	m_bAnchorDraw = FALSE;
	m_bCtrlAlwaysFocus = FALSE;
	OnPaint();

	return bMapLoad;
}

BOOL CWxMapCtrl::Load(UINT nResID,LPCTSTR lpTyp)
{
	// 查找资源
	HRSRC hRsrc = ::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(nResID), lpTyp);
	if (hRsrc == NULL) 
		return FALSE;
	// 加载资源
	HGLOBAL hImgData = ::LoadResource(AfxGetResourceHandle(), hRsrc);
	if (hImgData == NULL)
	{
		::FreeResource(hImgData);
		return FALSE;
	}
	// 锁定内存中的指定资源
	LPVOID lpVoid = ::LockResource(hImgData);
	LPSTREAM pStream = NULL;
	DWORD dwSize = ::SizeofResource(AfxGetResourceHandle(), hRsrc);
	HGLOBAL hNew = ::GlobalAlloc(GHND, dwSize);
	LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);
	::memcpy(lpByte, lpVoid, dwSize);
	// 解除内存中的指定资源
	::GlobalUnlock(hNew);
	// 从指定内存创建流对象
	HRESULT ht = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
	if ( ht != S_OK )
	{
		GlobalFree(hNew);
	}
	else
	{
		// 加载图片
		ht=m_imgLoader1.Load(pStream);
		GlobalFree(hNew);
	}
	// 释放资源
	::FreeResource(hImgData);

	if(ht == S_OK)
	{
//		m_bmWidth = m_imgLoader1.GetWidth();
//		m_bmHeight = m_imgLoader1.GetHeight();

		m_imgLoader1.GetHeight();
		m_imgLoader1.GetWidth();
		m_imgLoader1.GetBPP();

	//	m_bRectsNeedRecalculate = TRUE;
	}
	else
	{
		m_bShowThumb = FALSE;
		//		m_bShowRuler = FALSE;
//		m_bmWidth = 0;
//		m_bmHeight = 0;
	//	m_nDpi = 96;

	}


	m_bAnchorDraw = FALSE;

	m_bCtrlAlwaysFocus = FALSE;
//	m_fZoom = 1.0f;
	//		m_bShowGrid = FALSE;
	m_PntOffset = CPoint(0,0);
	OnPaint();
	return (ht==S_OK);
}


BOOL CWxMapCtrl::SaveUserRect(LPCTSTR lpszFilename)
{
	CPoint pntOffOld = m_PntOffset;
	float fZoom = m_fZoom;

	int cx = m_rectUser.Width() + MAPCTRL_IMGBDR_WIDTH*2;
	int cy = m_rectUser.Height() + MAPCTRL_IMGBDR_WIDTH*2;

	m_PntOffset.x = -(m_rectUser.left+MAPCTRL_IMGBDR_WIDTH);
	m_PntOffset.y = -(m_rectUser.top+MAPCTRL_IMGBDR_WIDTH);
	m_fZoom = 1.00f;

	m_bRectsNeedRecalculate = TRUE;
	RecalculateRects(cx,cy);

	CImage img;
	img.Create(cx,cy,24,0);

	CDC *pDC=CDC::FromHandle(img.GetDC());
	pDC->FillSolidRect(CRect(0,0,cx,cy), m_crBack);

	DrawCtrl(pDC, MDRAW_OUTPUT);

	img.ReleaseDC();

	m_PntOffset = pntOffOld;
	m_fZoom = fZoom;

	m_bRectsNeedRecalculate = TRUE;

	return (img.Save(lpszFilename) == S_OK);
}
