#include "StdAfx.h"
#include "WxRgn.h"

////////////////////////////////////////////////////////////////
CWxRgn *NewRgn(std::wstring rty, std::wstring rval)
{
	CWxRgn *ret = NULL;

	int nRgnType = wcstol(rty.c_str(), NULL, 10);
	switch(nRgnType)
	{
	case 1:		ret = new CWxEllipticRgn();	break;	//��Բ
	case 2:		ret = new CWxRectRgn();		break;	//����
	case 3:		ret = new CWxPolygonRgn();	break;	//������\�ı���\����ε�
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
	//��Բ/Բ
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
	//����
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
		//����������
		int nCross = 0;    // ���������ͳ��Ŀ������һ������������ཻ����

		for (UINT i = 0; i < m_polys->size(); i++) {   //���������ÿһ���ڵ�

			CHPVec2D p1;
			CHPVec2D p2;

			p1 = m_polys->at(i);
			p2 = m_polys->at((i+1)%m_polys->size());  // p1������ڵ㣬p2����һ���ڵ㣬���������Ƕ���ε�һ����
			// �����㷨����������y���������жϵ�

			if ( p1.y == p2.y )
				continue;   //�����������ˮƽ�ģ�����

			if ( y < min(p1.y, p2.y)) //���Ŀ����������߶Σ�����
				continue; 

			if ( y >= max(p1.y, p2.y)) //���Ŀ����������߶Σ�����
				continue; 
			//��ô�����������ǣ������p1��ˮƽ�ߣ���p2��ˮƽ�ߣ�Ŀ��������������м�
			double dx = (double)(y - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x; 
			// ��εļ��������� ��Ŀ��㣬��һ��ˮƽ�ߣ�x�������������ε�ǰ�ߵĽ���x����
			if ( dx > x ) 
				nCross++; //����������ұߣ�ͳ�Ƽ�һ������ڴ�Ŀ������ҷ�һ�����ߣ�ray���������θ��ߵ��ཻ��crossing������
		} 

		//�����������˵���ڶ������
		//�����ڶ������ �� ����
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
