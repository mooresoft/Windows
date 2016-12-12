#include "stdafx.h"
#include "ActiveMQConsumer.h"
#include <conio.h>

ActiveMQConsumer::ActiveMQConsumer(const string& brokerURI,
	                               bool useTopic/* = false*/, 
	                               bool sessionTransacted/* = false*/, 
	                               int waitMillis/* = 30000*/) : m_latch(1)
{
	m_pconnection = NULL;
	m_psession = NULL;
	m_pdestination = NULL;
	m_pconsumer = NULL;

	m_brokerURI = brokerURI;
	m_useTopic  = useTopic;
	m_sessionTransacted = sessionTransacted;
	m_waitMillis = waitMillis;
}

ActiveMQConsumer::~ActiveMQConsumer()
{
	cleanup();
}

void ActiveMQConsumer::init(void)
{
	// Create a ConnectionFactory
	auto_ptr<ConnectionFactory> connectionFactory(
		ConnectionFactory::createCMSConnectionFactory(m_brokerURI));

	// Create a Connection
	m_pconnection = connectionFactory->createConnection();
	m_pconnection->start();
	m_pconnection->setExceptionListener(this);

	// Create a Session
	if (this->m_sessionTransacted == true)
	{
		m_psession = m_pconnection->createSession(Session::SESSION_TRANSACTED);
	} else
	{
		m_psession = m_pconnection->createSession(Session::AUTO_ACKNOWLEDGE);
	}

	// Create the m_pdestination (Topic or Queue)
	if (m_useTopic)
	{
		m_pdestination = m_psession->createTopic("shen.FOO");
	} 
	else 
	{
		m_pdestination = m_psession->createQueue(source_foo);
	}

	// Create a MessageConsumer from the Session to the Topic or Queue
	//m_pconsumer = m_psession->createConsumer(m_pdestination, "JMSXGroupID='1'");
	m_pconsumer = m_psession->createConsumer(m_pdestination);
	m_pconsumer->setMessageListener(this);

	// Indicate we are ready for messages.
	m_latch.countDown();

	// Wait while asynchronous messages come in.
	//m_doneLatch.await(m_waitMillis);
}

void ActiveMQConsumer::close()
{
	this->cleanup();
}

bool ActiveMQConsumer::waitUntilReady()
{
    bool bReady = false;
	try
	{
		bReady = m_latch.await(3000);

		if (false == bReady)
		{
			TRACE("Consumer Ready failed.\r\n");
		}
	}
	catch (CMSException& e)
	{
		TRACE("Consumer Ready failed.\r\n");
		e.printStackTrace();
	}	

	return bReady;
}

void ActiveMQConsumer::run()
{
	try 
	{
		init();
	}
	catch (CMSException& e) 
	{
		// Indicate we are ready for messages.
		m_latch.countDown();
		e.printStackTrace();
	}
}

#define MAX_UDP_BUFFER_SIZE  800
char RecvBuffer[MAX_UDP_BUFFER_SIZE] = {0};

// Called from the m_pconsumer since this class is a registered MessageListener.
void ActiveMQConsumer::onMessage(const Message* message)
{
	static int count = 0;
	try 
	{
		count++;
		const BytesMessage* bytesMessage =
			dynamic_cast< const BytesMessage* >( message );		

		unsigned char* recv_data = bytesMessage->getBodyBytes();
	    int recv_size = bytesMessage->getBodyLength();
		if (NULL != bytesMessage && 0 < recv_size && NULL != recv_data)
		{
			memcpy(RecvBuffer, recv_data, recv_size);
		} 

		//_cprintf("Message #%d Received: %s\n", count, RecvBuffer);

		if (NULL != recv_data)
		{
			delete recv_data;
		}
	}
	catch (CMSException& e) 
	{
		e.printStackTrace();
	}

	// Commit all messages.
	if (this->m_sessionTransacted)
	{
		m_psession->commit();
	}

	// No matter what, tag the count down m_latch until done.
	//m_doneLatch.countDown();
}

// If something bad happens you see it here as this class is also been
// registered as an ExceptionListener with the m_pconnection.
void ActiveMQConsumer::onException(const CMSException& ex AMQCPP_UNUSED)
{
	printf("CMS Exception occurred.  Shutting down client.\n");
	ex.printStackTrace();
	exit(1);
}


void ActiveMQConsumer::cleanup()
{
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
		delete m_pconsumer;
		m_pconsumer = NULL;
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