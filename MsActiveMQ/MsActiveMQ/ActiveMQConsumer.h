#ifndef _ACTIVEMQ_CONSUMER_H
#define _ACTIVEMQ_CONSUMER_H

#include <activemq/library/ActiveMQCPP.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/System.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/util/Config.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <memory>

using namespace activemq::core;
using namespace decaf::util::concurrent;
using namespace decaf::util;
using namespace decaf::lang;
using namespace cms;
using namespace std;

const char source_foo[] = "MOORE.FOO";

class ActiveMQConsumer : public ExceptionListener,
	                     public MessageListener,
	                     public Runnable 
{
public:
	ActiveMQConsumer(const string& brokerURI,
		bool useTopic = false, 
		bool sessionTransacted = false, 
		int waitMillis = 30000);
	~ActiveMQConsumer();
	void init(void);
	void close();
	bool waitUntilReady();
	void run();
	void onMessage(const Message* message);
	void onException(const CMSException& ex AMQCPP_UNUSED);

private:
	void cleanup();
	ActiveMQConsumer(const ActiveMQConsumer&);
	ActiveMQConsumer& operator=(const ActiveMQConsumer&);

private:
	CountDownLatch m_latch;
	Connection* m_pconnection;
	Session* m_psession;
	Destination* m_pdestination;
	MessageConsumer* m_pconsumer;
	long m_waitMillis;
	bool m_useTopic;
	bool m_sessionTransacted;
	string m_brokerURI;
};


#endif