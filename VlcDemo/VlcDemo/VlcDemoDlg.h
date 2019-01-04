
// VlcDemoDlg.h : 头文件
//

#pragma once
#include "vlc/vlc.h"

// CVlcDemoDlg 对话框
class CVlcDemoDlg : public CDialogEx
{
// 构造
public:
	CVlcDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VLCDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	libvlc_media_t*			m_vlcMedia;
	libvlc_media_player_t*	m_vlcMplay;
	libvlc_instance_t*		m_vlcInst;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnPlay();
};
