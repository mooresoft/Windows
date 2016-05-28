#pragma once
#include <mysql.h>
#include <string>
using namespace std;

class CMysqls
{
public:
	CMysqls();
	~CMysqls();

	BOOL connect();
	BOOL query(const char* strsql);
	void close();


private:
	MYSQL m_mysql;
};

