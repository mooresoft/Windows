#include "stdafx.h"
#include "TopoDev.h"


CTopoDev::CTopoDev(void)
{
}


CTopoDev::~CTopoDev(void)
{
}

CTopoDev &CTopoDev::operator=(const CTopoDev &s)
{
	m_id = s.m_id;
	m_pos = s.m_pos;
	m_tmFound = s.m_tmFound;
	m_tmUpdate = s.m_tmUpdate;
	m_name = s.m_name;
	return (*this);
};
