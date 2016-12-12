
// MsActiveMQDlg.h : 头文件
//

#pragma once

// CMsActiveMQDlg 对话框
class CMsActiveMQDlg : public CDialogEx
{
// 构造
public:
	CMsActiveMQDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MSACTIVEMQ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	static DWORD WINAPI SendThread(LPVOID lpParam);
// 实现
protected:
	HICON m_hIcon;
	HANDLE m_hSendThread;
	BOOL   m_bSend;
	// 生成的消息映射函数
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
