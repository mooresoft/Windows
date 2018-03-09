
// MsActiveMQ.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include "ActiveMQProducer.h"
#include "ActiveMQConsumer.h"
// CMsActiveMQApp:
// 有关此类的实现，请参阅 MsActiveMQ.cpp
//

class CMsActiveMQApp : public CWinApp
{
public:
	CMsActiveMQApp();

// 重写
public:
	virtual BOOL InitInstance();

	ActiveMQProducer* m_pProducer;
	ActiveMQConsumer* m_pConsumer;

// 实现

	DECLARE_MESSAGE_MAP()

private:
	Thread* m_pProducerThread;
	Thread* m_pConsumerThread;
};

extern CMsActiveMQApp theApp;