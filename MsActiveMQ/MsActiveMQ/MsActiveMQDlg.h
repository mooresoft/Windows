
// MsActiveMQDlg.h : ͷ�ļ�
//

#pragma once

// CMsActiveMQDlg �Ի���
class CMsActiveMQDlg : public CDialogEx
{
// ����
public:
	CMsActiveMQDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MSACTIVEMQ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	static DWORD WINAPI SendThread(LPVOID lpParam);
// ʵ��
protected:
	HICON m_hIcon;
	HANDLE m_hSendThread;
	BOOL   m_bSend;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnProducer();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnConsumer();
};
