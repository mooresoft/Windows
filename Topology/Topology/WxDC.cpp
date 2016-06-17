#include "stdafx.h"
#include "WxDC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWxDC::CWxDC(CDC* pDC, CRect* pRect/*=NULL*/, CBitmap* pBitmap/*=NULL*/) : 
    m_bPrinting(FALSE), 
    m_Offset(0, 0) {

	m_pCurBitmap = NULL;
    CreateCompatibleDC(pDC);

    m_crBG      = GetBkColor();
    m_crTX      = GetTextColor();
    m_pPen      = GetCurrentPen();
    m_pBrush    = GetCurrentBrush();
    m_pFont     = GetCurrentFont();
	m_pBitmap   = GetCurrentBitmap();

    if (pRect) {
        int nRectWidth(pRect->Width());
        int nRectHeight(pRect->Height());

        if (pBitmap) {
            pBitmap->CreateCompatibleBitmap(pDC, nRectWidth, nRectHeight);
    
            SelectObject(pBitmap);
			m_pCurBitmap = pBitmap;
            }
        }
    
    return;
    }

CWxDC::~CWxDC() {

        SetBkColor(m_crBG);
        SetTextColor(m_crTX);
        SelectObject(m_pPen);
        SelectObject(m_pBrush);
        SelectObject(m_pFont);
        SelectObject(m_pBitmap);
	if(m_pCurBitmap)
		m_pCurBitmap->DeleteObject();

    DeleteDC();

    return;
    }

void CWxDC::FillSolidRect(LPCRECT lpRect, COLORREF clr) {
    
    CDC::FillSolidRect(&CRect(CRect(lpRect)+m_Offset), clr);

    return;
    }

int CWxDC::DrawText(LPCTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat) {

    if (nFormat!=DT_CALCRECT) {
        CRect Rect(lpRect);
        Rect.OffsetRect(m_Offset);
        return CDC::DrawText(lpszString, nCount, &Rect, nFormat);
        }
    
    return CDC::DrawText(lpszString, nCount, lpRect, nFormat);
    }

BOOL CWxDC::TextOut(int x, int y, LPCTSTR lpszString, int nCount) {

    return CDC::TextOut(x+m_Offset.x, y+m_Offset.y, lpszString, nCount);
    }

CPoint CWxDC::MoveTo(int x, int y) {

    return CDC::MoveTo(x+m_Offset.x, y+m_Offset.y);
    }

CPoint CWxDC::LineTo(int x, int y) {

    return CDC::LineTo(x+m_Offset.x, y+m_Offset.y);
    }

BOOL CWxDC::FillRgn(CRgn* pRgn, CBrush* pBrush) {

    int nReturn(pRgn->OffsetRgn(m_Offset));
    
    if (ERROR==nReturn)
        VERIFY(FALSE);

    return CDC::FillRgn(pRgn, pBrush);
    }

BOOL CWxDC::Polygon(LPPOINT lpPoints, int nCount) 
{

    POINT* pPoints = new POINT[nCount];
    
    for (int i(0);i<nCount;i++) {
        (*(pPoints+i)).x = (*(lpPoints+i)).x + m_Offset.x;
        (*(pPoints+i)).y = (*(lpPoints+i)).y + m_Offset.y;
        }    
    BOOL bResult(CDC::Polygon(pPoints, nCount));
    delete [] pPoints;

    return bResult;
    }

BOOL CWxDC::Polyline(LPPOINT lpPoints, int nCount) {

    POINT* pPoints = new POINT[nCount];
    
    for (int i(0);i<nCount;i++) {
        (*(pPoints+i)).x = (*(lpPoints+i)).x + m_Offset.x;
        (*(pPoints+i)).y = (*(lpPoints+i)).y + m_Offset.y;
        }
    
    BOOL bResult(CDC::Polyline(pPoints, nCount));
    delete [] pPoints;

    return bResult;
    }

BOOL CWxDC::Ellipse(int x1, int y1, int x2, int y2) {

    return CDC::Ellipse(x1+m_Offset.x, y1+m_Offset.y, x2+m_Offset.x, y2+m_Offset.y);
    }

BOOL CWxDC::Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {

    return CDC::Pie(x1+m_Offset.x, y1+m_Offset.y, x2+m_Offset.x, y2+m_Offset.y, x3+m_Offset.x, y3+m_Offset.y, x4+m_Offset.x, y4+m_Offset.y);
    }

BOOL CWxDC::DrawIcon(int x, int y, HICON hIcon) {

    return CDC::DrawIcon(x+m_Offset.x, y+m_Offset.y, hIcon);
    }

BOOL CWxDC::BitBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, DWORD dwRop) {

    return CDC::BitBlt(x+m_Offset.x, y+m_Offset.y, nWidth, nHeight, pSrcDC, xSrc, ySrc, dwRop);
    }

CPen* CWxDC::SelectObject(CPen* pPen) {
        return CDC::SelectObject(pPen);
    
    return NULL;
    }

CBrush* CWxDC::SelectObject(CBrush* pBrush) {
        return CDC::SelectObject(pBrush);
    
    return NULL;
    }

CFont* CWxDC::SelectObject(CFont* pFont) 
{
    if (!m_bPrinting)
        return CDC::SelectObject(pFont);
    return NULL;
 }

CBitmap* CWxDC::SelectObject(CBitmap* pBitmap) {
        return CDC::SelectObject(pBitmap);
    
    return NULL;
    }

int CWxDC::SelectObject(CRgn* pRgn) {

    //if (!m_bPrinting)
        return CDC::SelectObject(pRgn);
   
    return NULL;
    }

/////////////////////////////////////

#include <math.h>

// 将DC旋转一定的角度

int RotateDC(HDC hDc, double fangle, POINT centerPt)
{
	int nGraphicsMode = SetGraphicsMode(hDc, GM_ADVANCED);
	XFORM xform;
	if ( fangle != 0.000f )
	{
		xform.eM11 = (float)cos(fangle);
		xform.eM12 = (float)sin(fangle);
		xform.eM21 = (float)-sin(fangle);
		xform.eM22 = (float)cos(fangle);
		xform.eDx = (float)(centerPt.x - cos(fangle)*centerPt.x + sin(fangle)*centerPt.y);
		xform.eDy = (float)(centerPt.y - cos(fangle)*centerPt.y - sin(fangle)*centerPt.x);
		SetWorldTransform(hDc, &xform);
	}
	return nGraphicsMode;
}
int RotateDC(HDC hDc, int iAngle, POINT centerPt)
{
	return RotateDC(hDc, iAngle*CONST_PI/180.00f, centerPt);
}

// 恢复旋转过的DC
void RestoreRotatedDC(HDC hDc, int nGraphicsMode)
{
	XFORM xform;
	xform.eM11 = (float)1.0; 
	xform.eM12 = (float)0;
	xform.eM21 = (float)0;
	xform.eM22 = (float)1.0;
	xform.eDx = (float)0;
	xform.eDy = (float)0;

	SetWorldTransform(hDc, &xform);
	SetGraphicsMode(hDc, nGraphicsMode);
}

void TransparentBlt2( HDC hdcDest,      // 目标DC
					 int nXOriginDest,   // 目标X偏移
					 int nYOriginDest,   // 目标Y偏移
					 int nWidthDest,     // 目标宽度
					 int nHeightDest,    // 目标高度
					 HDC hdcSrc,         // 源DC
					 int nXOriginSrc,    // 源X起点
					 int nYOriginSrc,    // 源Y起点
					 int nWidthSrc,      // 源宽度
					 int nHeightSrc,     // 源高度
					 UINT crTransparent  // 透明色,COLORREF类型
					 )
{
	HBITMAP hOldImageBMP, hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);    // 创建兼容位图
	HBITMAP hOldMaskBMP, hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);            // 创建单色掩码位图
	HDC        hImageDC = CreateCompatibleDC(hdcDest);
	HDC        hMaskDC = CreateCompatibleDC(hdcDest);
	hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);
	hOldMaskBMP = (HBITMAP)SelectObject(hMaskDC, hMaskBMP);

	// 将源DC中的位图拷贝到临时DC中
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
		BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
	else
		StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, 
		hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);

	COLORREF crTextOld = GetTextColor(hdcDest);
	// 设置透明色
	SetBkColor(hImageDC, crTransparent);

	// 生成透明区域为白色，其它区域为黑色的掩码位图
	BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);

	// 生成透明区域为黑色，其它区域保持不变的位图
	SetBkColor(hImageDC, RGB(0,0,0));
	SetTextColor(hImageDC, RGB(255,255,255));
	BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

	// 透明部分保持屏幕不变，其它部分变成黑色
	SetBkColor(hdcDest,RGB(0xff,0xff,0xff));
	SetTextColor(hdcDest,RGB(0,0,0));
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

	// "或"运算,生成最终效果
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCPAINT);

	SelectObject(hImageDC, hOldImageBMP);
	DeleteDC(hImageDC);
	SelectObject(hMaskDC, hOldMaskBMP);
	DeleteDC(hMaskDC);
	DeleteObject(hImageBMP);
	DeleteObject(hMaskBMP);

	SetTextColor(hdcDest,crTextOld);

}