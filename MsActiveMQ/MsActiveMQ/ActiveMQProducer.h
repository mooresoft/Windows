#ifndef _ACTIVEMQ_PRODUCER_H
#define _ACTIVEMQ_PRODUCER_H

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

class ActiveMQProducer : public Runnable 
{

public:
	ActiveMQProducer(const string& brokerURI, bool useTopic = false, bool sessionTransacted = false);
	virtual ~ActiveMQProducer();
	void     init(void);
	void     close();
	void     send(const unsigned char* msg, unsigned int msg_size);
	virtual void run();
	
private:
	void cleanup();
	ActiveMQProducer(const ActiveMQProducer&);
	ActiveMQProducer& operator=(const ActiveMQProducer&);

private:

	Connection* m_pconnection;
	Session* m_psession;
	Destination* m_pdestination;
	MessageProducer* m_pproducer;
	bool m_useTopic;
	bool m_sessionTransacted;
	string m_brokerURI;

	bool m_bLoop;
};


#endif