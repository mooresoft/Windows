#pragma once
#include "HPVec.h"

class CTopoDev
{
public:
	CTopoDev(void);
	~CTopoDev(void);
public:
	CTopoDev &operator=(const CTopoDev &s);
public:
	UINT64 m_id;
	CHPVec2D m_pos;
	time_t m_tmFound;
	time_t m_tmUpdate;
	wstring m_name;
public:
	BOOL IsOnline(){return (time(NULL)-m_tmUpdate<30);}; //³¬¹ý30ÃëÀëÏß
};

