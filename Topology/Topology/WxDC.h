#pragma once

class CWxDC : public CDC
{
public:
    CWxDC(CDC* pDC, CRect* pRect=NULL, CBitmap* pBitmap=NULL);
    CWxDC() : m_bPrinting(TRUE), m_Offset(0, 0) {};
    virtual ~CWxDC();

    void        FillSolidRect(LPCRECT lpRect, COLORREF clr);
    int         DrawText(LPCTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat);
    int         DrawText(const CString& str, LPRECT lpRect, UINT nFormat) {return CWxDC::DrawText((LPCTSTR)str, str.GetLength(), lpRect, nFormat);};
    BOOL        TextOut(int x, int y, LPCTSTR lpszString, int nCount);
    BOOL        TextOut(int x, int y, const CString& str) {return CWxDC::TextOut(x, y, (LPCTSTR)str, str.GetLength());};
    CPoint      MoveTo(POINT point) {return CWxDC::MoveTo(point.x, point.y);};
    CPoint      MoveTo(int x, int y);
    CPoint      LineTo(POINT point) {return CWxDC::LineTo(point.x, point.y);};
    CPoint      LineTo(int x, int y);

    BOOL        FillRgn(CRgn* pRgn, CBrush* pBrush);
    BOOL        Polygon(LPPOINT lpPoints, int nCount);
    BOOL        Polyline(LPPOINT lpPoints, int nCount);

    BOOL        Ellipse(int x1, int y1, int x2, int y2);
    BOOL        Ellipse(LPCRECT lpRect) {return CWxDC::Ellipse(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);};

    BOOL        Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
    BOOL        Pie(LPCRECT lpRect, POINT ptStart, POINT ptEnd) {return CWxDC::Pie(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);};

    BOOL        DrawIcon(int x, int y, HICON hIcon);
    BOOL        DrawIcon(POINT point, HICON hIcon) {return CWxDC::DrawIcon(point.x, point.y, hIcon);};

    BOOL        BitBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, DWORD dwRop);

    CPen*       SelectObject(CPen* pPen);
    CBrush*     SelectObject(CBrush* pBrush);
    CFont*      SelectObject(CFont* pFont);
    CBitmap*    SelectObject(CBitmap* pBitmap);
    int         SelectObject(CRgn* pRgn);

private:
    COLORREF    m_crBG;
    COLORREF    m_crTX;
    CPen*       m_pPen;
    CBrush*     m_pBrush;
    CFont*      m_pFont;
	CBitmap*    m_pBitmap;

    BOOL        m_bPrinting;
	CBitmap* m_pCurBitmap;

public:
    CPoint      m_Offset;
    
};


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
					 );

int RotateDC(HDC hDc, int iAngle, POINT centerPt);
int RotateDC(HDC hDc, double fangle, POINT centerPt);
void RestoreRotatedDC(HDC hDc, int nGraphicsMode);
