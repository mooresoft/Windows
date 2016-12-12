
// MsActiveMQ.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MsActiveMQ.h"
#include "MsActiveMQDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMsActiveMQApp

BEGIN_MESSAGE_MAP(CMsActiveMQApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMsActiveMQApp ����

CMsActiveMQApp::CMsActiveMQApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	m_pProducer = NULL;
	m_pConsumer = NULL;
	m_pProducerThread = NULL;
	m_pConsumerThread = NULL;
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMsActiveMQApp ����

CMsActiveMQApp theApp;


// CMsActiveMQApp ��ʼ��

BOOL CMsActiveMQApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	activemq::library::ActiveMQCPP::initializeLibrary();
	
	m_pProducer = new ActiveMQProducer("tcp://192.168.70.18:61616?transport.useAsyncSend=true");
	m_pConsumer = new ActiveMQConsumer("tcp://192.168.70.18:61616?transport.useAsyncSend=true");
	//m_pConsumer = new ActiveMQConsumer("failover:(tcp://192.168.70.18:61616)?timeout=3000");

	m_pProducerThread = new Thread(m_pProducer);
	m_pConsumerThread = new Thread(m_pConsumer);

	m_pProducerThread->start();
	m_pConsumerThread->start();
	
	if (false == m_pConsumer->waitUntilReady())
	{
		AfxMessageBox(_T("ActiveMQ ERROR"));
		return FALSE;
	}

	AllocConsole();
	CMsActiveMQDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	m_pConsumer->close();
	m_pProducer->close();

	m_pProducerThread->join();
	m_pConsumerThread->join();


	delete m_pProducerThread;
	delete m_pConsumerThread;

	m_pProducerThread = NULL;
	m_pConsumerThread = NULL;

	delete m_pProducer;
	delete m_pConsumer;

	m_pProducer = NULL;
	m_pConsumer = NULL;

	activemq::library::ActiveMQCPP::shutdownLibrary();
	FreeConsole();
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}