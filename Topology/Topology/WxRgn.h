#pragma once

#include "HPVec.h"

#define ReleaseObjSafe(obj) {if(obj) delete obj; obj=NULL;}
#define ReleaseArraySafe(obj) {if(obj) delete []obj; obj=NULL;}


class CWxRgn
{
public:
	CWxRgn(void);
	~CWxRgn(void);
public:
	virtual ULONG GetRgnType(){return 0;};
	virtual BOOL PtInRegion(double x, double y){return TRUE;};
	virtual BOOL Init(double x1,double y1, double x2, double y2){return TRUE;};
	virtual BOOL Init(std::wstring str){return TRUE;};
};


class CWxEllipticRgn : public CWxRgn
{
public:
	CWxEllipticRgn(void);
	~CWxEllipticRgn(void);
public:
	ULONG GetRgnType(){return 1;};
	BOOL PtInRegion(double x, double y);
	BOOL Init(double x1,double y1, double x2, double y2);
	BOOL Init(std::wstring str);
public:
	double m_x0;
	double m_y0;
	double m_rx;
	double m_ry;
};

class CWxRectRgn : public CWxRgn
{
public:
	CWxRectRgn(void);
	~CWxRectRgn(void);
public:
	ULONG GetRgnType(){return 2;};
	BOOL PtInRegion(double x, double y);
	BOOL Init(double x1,double y1, double x2, double y2);
	BOOL Init(std::wstring str);
public:
	double m_left;
	double m_top;
	double m_width;
	double m_height;
};

class CWxPolygonRgn : public CWxRgn
{
public:
	CWxPolygonRgn(void);
	~CWxPolygonRgn(void);
public:
	ULONG GetRgnType(){return 3;};
	BOOL PtInRegion(double x, double y);
	BOOL Init(std::wstring str);
public:
	std::vector<CHPVec2D> *m_polys;
};


CWxRgn *NewRgn(std::wstring rty, std::wstring rval);
