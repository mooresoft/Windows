#pragma once
#include "WxMapDefs.h"
#include "WxRgn.h"

// CWxMapCtrl

class CWxMapCtrl : public CWnd
{
	//DECLARE_DYNAMIC(CWxMapCtrl)
	DECLARE_DYNCREATE(CWxMapCtrl)

public:
	CWxMapCtrl();
	virtual ~CWxMapCtrl();

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void PreSubclassWindow();
public:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg VOID OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg VOID OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg VOID OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
protected:
	DECLARE_MESSAGE_MAP()

	LRESULT SendUserMessage(UINT nMsg, LPARAM lp);
protected:
	CImage m_imgLoader1;
	std::vector<ImageObject *> m_vecImg;
public:
	BOOL Load(LPCTSTR lpszResourceName);
	BOOL Load(UINT nResID,LPCTSTR lpTyp);
	BOOL IsNull(){return m_imgLoader1.IsNull();};
	BOOL Save(LPCTSTR lpszFileName);
	CImage *GetImagePntr(){return m_imgLoader1.IsNull()?NULL:&m_imgLoader1;}
	BOOL Load(LPCTSTR szPath, CRect realRect, COLORREF crMask);
	void Clear();

public:

	void SetColor(COLORREF crBack, BOOL bRedraw = FALSE);

	void Zoom(BOOL bIncrease);


	void ShowBorder(BOOL bShow);
	void ShowRuler(INT nRuler);
	void ShowGrid(INT nGrid);
	void ShowScale(INT nScale);

	HCURSOR SetMyCursor(HCURSOR hCursor);
	CPoint GetCursorPos(){return m_CursorPos;};

//	void GetWidthAndHeight(int &nWidth, int &nHeight){nWidth = m_bmWidth; nHeight = m_bmHeight;};
	void SetCenterPos(double xPos, double yPos); // 厘米为单位的位置坐标
	void SetCenterPixel(POINT &pnt);
	void OffsetView(int deltaX, int deltaY);
	void DrawText(CDC *pdc, CString strText, int x, int y, DWORD dwType);
	void DrawCtrl(CDC *pDC, UINT drawMode=MDRAW_DEFAULT);
public:
	INT GetRuler();
	INT GetGrid();
	INT GetScale();
private:
	void MoveFloatLayer(CRect &rectLayer, CPoint point);
	int CheckPointPos(CPoint point);
	void DrawRectRgn(CDC *pDC, int x, int y, int w, int h, COLORREF crPen);
	void RecalculateRects(int cx, int cy);
	void DrawThumbnail(CDC *pDC,  CRect &rectv, CRect &rect);
//	void DrawRuler(CDC *pDC,  CRect &rectShow, CRect &rectView, double dZoom);
	void DrawRuler(CDC *pDC);
	void DrawPathLine(CDC *pDC, int x, int y, int x0, int y0, int r, COLORREF color, unsigned char path);


//	void DrawGridLine(CDC *pDC,  CRect &rectShow, CRect &rectView, double dZoom);
	void DrawGridLine(CDC *pDC);
	void DrawTrack(CDC *pDC, int x0, int y0, int x, int y, COLORREF color);
	void DrawPoint(CDC *pDC, int x, int y, int r, int s, COLORREF c);

	double CalcLogarithms(double inVal, double throshVal);

	void DrawLogCoords(CDC* pDC, double StartFreq, double StopFreq);

	void DrawVelocity(CDC *pDC, POINT &pt, int w, int h, int velo);

private:
	int m_nPix;
	int m_nSelect;
	int m_nY;
	int m_nMovePos;
//	int m_nRulerWidth;
	BOOL m_bCtrlAlwaysFocus;
	BOOL m_bMouseTracking;
	CRect m_rectThumb;			//小图区域
	CRect m_rectPic;			//图形显示区域
	CRect m_rectXRuler;
	CRect m_rectYRuler;
	CRect m_rectRedraw;

	BOOL m_bAnchorDraw;
	CString m_strBmpName;

	COLORREF m_crBack;

	COLORREF m_crLine;
	COLORREF m_crGrid;
	COLORREF m_crTrack;
	COLORREF m_crPoint;


	CPoint m_CursorPos;
	CPoint m_PntS0;
	CPoint m_PntS1;
	CPoint m_PntS2;

	CPoint m_PntOffset;
	CPoint m_PntOffsetAdd;
	BOOL m_bHasBorder;
	BOOL m_bRectsNeedRecalculate;

	float m_fZoomAdd;

	HCURSOR m_hCursorArrow;
	HCURSOR m_hCursorSizeall;
	HCURSOR m_hCursorHand;

	HCURSOR m_hMyCursor;	

	BOOL m_bSetTimer;

	BOOL m_bShowThumb;

	CFont m_font;
private:
	// 原始范围：加载图像的范围，(0,0)~(bmWidth,bmHeight)
	// 变化：锚点位置的坐标极值（转换为像素坐标），标签轨迹的坐标极值（转换为像素坐标）
	CRect m_rectUser;

	//标尺类型：横标尺0x1;纵标尺0x2;无标尺0x0;纵横标尺0x3
	INT m_nShowRuler;

	// 每格的宽度 ——最密网格为0.1米
	INT m_nGridRadiu;

	//比例尺，基准为1:100，1:50为2，1:200为0.5
	float m_fZoom;
	INT m_nScaleIndex; // 0~19

	//每厘米的像素数，比例尺1：100时，则为每米的像素数
	double m_coefx;
	double m_coefy;

	double m_fPicZoomX;
	double m_fPicZoomY;
	
	HPVEC2D m_pntPicOff;

	// 远距对数显示
	double m_dLogThresholdX;
	double m_dLogThresholdY;
	
private:
	void _Pix2Pos(double xPix, double yPix, double &xPos, double &yPos);
	void _Pos2Pix(double xPos, double yPos, double &dx, double &dy);
	void _View2Pix(HPVEC2D &ptv, double &xPix, double &yPix);

public:
	void SetPicZoom(double zoomX, double zoomY){m_fPicZoomX = zoomX; m_fPicZoomY = zoomY;};
	void SetPicOffset(HPVEC2D pntOff){m_pntPicOff.x=pntOff.x;m_pntPicOff.y=pntOff.y;};
	void SetLogThreshold(double x, double y){m_dLogThresholdX = x; m_dLogThresholdY = y;};
public:
	__inline BOOL PtInView(POINT &pt);
	void Pos2Pix(double xPos, double yPos, double &x, double &y);
	void Pix2View(double x, double y, HPVEC2D &pt);
	void Pos2View(double xPos, double yPos, HPVEC2D &pt);
	__inline void Pos2ViewIn(double xPos, double yPos, HPVEC2D &pt);
	void View2Pos(HPVEC2D &pt, double &xPos, double &yPos);
	__inline void View2PosIn(HPVEC2D &pt, double &xPos, double &yPos);
	void ViewLen2PosLen( double &coefx, double &coefy);
	void GetCoef( double &coefx, double &coefy);
	CRect GetPicRect(){return m_rectPic;};
	void DrawPosRound(CDC *pDC, CHPVec3D pos, double r, BOOL isVp, CHPVec2D ptt);
	void DrawPosHyperbola(CDC *pDC, CHPVec3D& pos0, CHPVec3D& pos1, double a, BOOL isVp, CHPVec2D ptt);
	void DrawPosRgn(CDC *pDC, CRgn *pRgn, BOOL bShowBorder, BOOL bFillColor, COLORREF cr);
	void DrawPosRgn(CDC *pDC, CWxRgn *pRgn, BOOL bShowBorder, BOOL bFillColor, COLORREF cr);
	void CreateRgnFromWxRgn(CRgn &rgn, CWxRgn *wxrgn, XFORM *xaf);
public:
	void DrawIcon(CDC *pDC, CDC *pSrc, POINT &ptCenter, int w, int h, CString strName=_T(""), CString strVal=_T(""));
	void DrawIconRotate(CDC *pDC, CDC *pSrc, POINT &ptCenter, int w, int h, double dangle, CString strName=_T(""), CString strVal=_T(""), INT velo=-1);
	void DrawPoint(CDC *pDC, POINT &pt, int r);
	void DrawArrow(CDC *pDC, POINT &pt, double azimuth, int r);
	void DrawRect2View(CDC *pDC, CHPRect &rectPix);
	void FillRect2View(CDC *pDC, CHPRect &rectPix, COLORREF cr, CHPRect &rectExclude);
//	void GetShowRects(CRect &rectSrc, CRect &rectDst);
	BOOL SaveUserRect(LPCTSTR lpszFileName);
private:
	CWnd *m_pMessageHandler;
public:
	void SetMessageHandler(CWnd *pWnd){m_pMessageHandler = pWnd;}
private:
	UINT m_dwPrs;
public:
	void SetPrs(DWORD dw){m_dwPrs = dw;};
protected:
	XFORM m_xform;
protected:
	__inline void RecalXform();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


