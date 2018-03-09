#include "stdafx.h"
#include "ActiveMQProducer.h"
#include "ActiveMQConsumer.h"

ActiveMQProducer::ActiveMQProducer(const string& brokerURI, bool useTopic/* = false*/, bool sessionTransacted/* = false*/)
{
	m_pconnection = NULL;
	m_psession = NULL;
	m_pdestination = NULL;
	m_pproducer = NULL;

	m_useTopic = useTopic;
	m_sessionTransacted = sessionTransacted;
	m_brokerURI = brokerURI;

	m_bLoop = false;
}


ActiveMQProducer::~ActiveMQProducer()
{
	cleanup();
}

void ActiveMQProducer::close()
{
	cleanup();
}

void ActiveMQProducer::init(void)
{
	// Create a ConnectionFactory
	auto_ptr<ConnectionFactory> connectionFactory(
		ConnectionFactory::createCMSConnectionFactory(m_brokerURI));

	// Create a Connection
	m_pconnection = connectionFactory->createConnection();
	m_pconnection->start();

	// Create a Session
	if (m_sessionTransacted)
	{
		m_psession = m_pconnection->createSession(Session::SESSION_TRANSACTED);
	} 
	else 
	{
		m_psession = m_pconnection->createSession(Session::AUTO_ACKNOWLEDGE);
	}

	// Create the m_pdestination (Topic or Queue)
	if (m_useTopic) 
	{
		m_pdestination = m_psession->createTopic("shen.FOO");
	} else 
	{
		m_pdestination = m_psession->createQueue(source_foo);
	}

	// Create a MessageProducer from the Session to the Topic or Queue
	m_pproducer = m_psession->createProducer(m_pdestination);
	m_pproducer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);

	if (NULL != m_psession)
	{
		m_bLoop = true;
	}
}

void ActiveMQProducer::run() 
{
	try 
	{
		init();

		while(true == m_bLoop)
		{
            Sleep(1);
		}
	} 
	catch (CMSException& e) 
	{
		e.printStackTrace();
	}
}

void ActiveMQProducer::send(const unsigned char* msg, unsigned int msg_size)
{
	// Create the Thread Id String
	if (NULL == msg || 0 == msg_size)
	{
		return;
	}

	if (NULL == m_psession || NULL == m_pproducer)
	{
		TRACE("Init ActiveMQ Producer failed.\r\n");
		return;
	}

	BytesMessage* message = m_psession->createBytesMessage(msg, msg_size);
	//message->setStringProperty("JMSXGroupID", "1");
	m_pproducer->send(message);

	if (NULL != message)
	{
		delete message;
	}
}

void ActiveMQProducer::cleanup() 
{
	m_bLoop = false;

	if (m_pconnection != NULL) 
	{
		try 
		{
			m_pconnection->close();
		} 
		catch (cms::CMSException& ex)
		{
			ex.printStackTrace();
		}
	}

	// Destroy resources.
	try 
	{
		delete m_pdestination;
		m_pdestination = NULL;
		delete m_pproducer;
		m_pproducer = NULL;
		delete m_psession;
		m_psession = NULL;
		delete m_pconnection;
		m_pconnection = NULL;
	} 
	catch (CMSException& e) 
	{
		e.printStackTrace();
	}
}

