
// MsActiveMQ.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include "ActiveMQProducer.h"
#include "ActiveMQConsumer.h"
// CMsActiveMQApp:
// �йش����ʵ�֣������ MsActiveMQ.cpp
//

class CMsActiveMQApp : public CWinApp
{
public:
	CMsActiveMQApp();

// ��д
public:
	virtual BOOL InitInstance();

	ActiveMQProducer* m_pProducer;
	ActiveMQConsumer* m_pConsumer;

// ʵ��

	DECLARE_MESSAGE_MAP()

private:
	Thread* m_pProducerThread;
	Thread* m_pConsumerThread;
};

extern CMsActiveMQApp theApp;