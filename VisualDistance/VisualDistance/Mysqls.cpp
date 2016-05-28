#include "stdafx.h"
#include "Mysqls.h"


CMysqls::CMysqls()
{	
	connect();	
}


CMysqls::~CMysqls()
{
	close();
}



BOOL CMysqls::connect()
{
	mysql_init(&m_mysql);
	if (NULL == mysql_real_connect(&m_mysql, "localhost", "shen", "shenshumao",
		"woxutest", 0, NULL, 0)) 
	{
		printf("mysql_real_connect ??:\nError %u (%s)\n",
			mysql_errno(&m_mysql), mysql_error(&m_mysql));

		mysql_close(&m_mysql);
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CMysqls::query(const char* strsql)
{
	if (NULL == strsql)
	{
		return FALSE;
	}
	else
	{
		if (0 != mysql_query(&m_mysql, strsql))
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
}

void CMysqls::close()
{
	mysql_close(&m_mysql);
	return;
}