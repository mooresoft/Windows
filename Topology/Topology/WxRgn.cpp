#include "StdAfx.h"
#include "WxRgn.h"

////////////////////////////////////////////////////////////////
CWxRgn *NewRgn(std::wstring rty, std::wstring rval)
{
	CWxRgn *ret = NULL;

	int nRgnType = wcstol(rty.c_str(), NULL, 10);
	switch(nRgnType)
	{
	case 1:		ret = new CWxEllipticRgn();	break;	//椭圆
	case 2:		ret = new CWxRectRgn();		break;	//矩形
	case 3:		ret = new CWxPolygonRgn();	break;	//三角形\四边形\多边形等
	default:	return ret;					break;
	}

	if(ret)
	{
		if(FALSE == ret->Init(rval))
			ReleaseObjSafe(ret);
	}
	return ret;
};

//////////////////////////////////////////////////////////////////
CWxRgn::CWxRgn(void)
{
}

CWxRgn::~CWxRgn(void)
{
}

//////////////////////////////////////////////////////////////////
CWxEllipticRgn::CWxEllipticRgn(void)
{
	m_x0 = m_y0 = m_rx = m_ry = 0.00f;
}

CWxEllipticRgn::~CWxEllipticRgn(void)
{
}

BOOL CWxEllipticRgn::PtInRegion(double x, double y)
{
	//椭圆/圆
	return ((x - m_x0)*(x-m_x0) / (m_rx*m_rx) + (y - m_y0)*(y-m_y0) / (m_ry*m_ry) <= 1);
}

BOOL CWxEllipticRgn::Init(double x1, double y1, double x2, double y2)
{
	m_x0 = (x1+x2)/2.00f;
	m_y0 = (y1+y2)/2.00f;
	m_rx = fabs(x2-x1)/2.00f;
	m_ry = fabs(y2-y1)/2.00f;
	return TRUE;
}

BOOL CWxEllipticRgn::Init(std::wstring str)
{
	double x1, y1, x2, y2;
	wchar_t t;

	if(4==swscanf(str.c_str(), L"%lf,%lf,%lf,%lf%c", &x1, &y1, &x2, &y2, &t))
	{
		return Init(x1,y1,x2,y2);
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////
CWxRectRgn::CWxRectRgn(void)
{
	m_left = m_top = m_width = m_height = 0.00f;
}

CWxRectRgn::~CWxRectRgn(void)
{
}

BOOL CWxRectRgn::PtInRegion(double x, double y)
{
	//矩形
	return (x >= m_left && x <=m_left+m_width && y >= m_top && y<= m_top+m_height); 
}
BOOL CWxRectRgn::Init(double x1, double y1, double x2, double y2)
{
	m_left = min(x1,x2);
	m_top = min(y1,y2);
	m_width = fabs(x2-x1);
	m_height = fabs(y2-y1);
	return TRUE;
}

BOOL CWxRectRgn::Init(std::wstring str)
{
	double x1, y1, x2, y2;
	wchar_t t;

	if(4==swscanf(str.c_str(), L"%lf,%lf,%lf,%lf%c", &x1, &y1, &x2, &y2, &t))
	{
		return Init(x1,y1,x2,y2);
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////
CWxPolygonRgn::CWxPolygonRgn(void)
{
	m_polys = NULL;
}

CWxPolygonRgn::~CWxPolygonRgn(void)
{
	if(m_polys)
	{
		m_polys->clear();
		delete m_polys;
		m_polys = NULL;
	}
}
BOOL CWxPolygonRgn::PtInRegion(double x, double y)
{
	if(m_polys)
	{
		//不规则多边形
		int nCross = 0;    // 定义变量，统计目标点向右画射线与多边形相交次数

		for (UINT i = 0; i < m_polys->size(); i++) {   //遍历多边形每一个节点

			CHPVec2D p1;
			CHPVec2D p2;

			p1 = m_polys->at(i);
			p2 = m_polys->at((i+1)%m_polys->size());  // p1是这个节点，p2是下一个节点，两点连线是多边形的一条边
			// 以下算法是用是先以y轴坐标来判断的

			if ( p1.y == p2.y )
				continue;   //如果这条边是水平的，跳过

			if ( y < min(p1.y, p2.y)) //如果目标点低于这个线段，跳过
				continue; 

			if ( y >= max(p1.y, p2.y)) //如果目标点高于这个线段，跳过
				continue; 
			//那么下面的情况就是：如果过p1画水平线，过p2画水平线，目标点在这两条线中间
			double dx = (double)(y - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x; 
			// 这段的几何意义是 过目标点，画一条水平线，x是这条线与多边形当前边的交点x坐标
			if ( dx > x ) 
				nCross++; //如果交点在右边，统计加一。这等于从目标点向右发一条射线（ray），与多边形各边的相交（crossing）次数
		} 

		//如果是奇数，说明在多边形里
		//否则在多边形外 或 边上
		return ((nCross&0x1) == 0x1);
	}
	return TRUE;
}

BOOL CWxPolygonRgn::Init(std::wstring str)
{
	std::vector<std::wstring>vecs;
	std::wstring strp = str;
	UINT j=strp.find_first_of(L';');
	while(j!=wstring::npos)
	{
		vecs.push_back(strp.substr(0,j));
		strp = strp.substr(j+1);
		j=strp.find_first_of(L';');
	}
	vecs.push_back(strp);

	m_polys->clear();
	for(std::vector<std::wstring>::iterator itr=vecs.begin(); itr!=vecs.end(); itr++)
	{
		CHPVec2D hp;
		if(2==swscanf(itr->c_str(), L"%lf,%lf", &hp.x,&hp.y))
		{
			m_polys->push_back(hp);
		}
	}
	
	if(m_polys->size()<3) return FALSE;

	if(m_polys->front() == m_polys->back())
		m_polys->pop_back();
	return (m_polys->size()>=3);
}
