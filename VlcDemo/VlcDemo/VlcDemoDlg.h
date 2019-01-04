
// VlcDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "vlc/vlc.h"

// CVlcDemoDlg �Ի���
class CVlcDemoDlg : public CDialogEx
{
// ����
public:
	CVlcDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VLCDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	libvlc_media_t*			m_vlcMedia;
	libvlc_media_player_t*	m_vlcMplay;
	libvlc_instance_t*		m_vlcInst;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnPlay();
};
