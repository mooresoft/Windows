
// TopologyXView.h : CTopologyXView ��Ľӿ�
//

#pragma once
#include "TopologyXDoc.h"
#include "WxMapCtrl.h"

class CTopologyXView : public CView
{
protected: // �������л�����
	CTopologyXView();
	DECLARE_DYNCREATE(CTopologyXView)

//��Ա
protected:
	CWxMapCtrl *m_pMapCtrl;
	CBitmap m_bmAnchorOn;
	CBitmap m_bmAnchorOff;
	CFont m_font;

// ����
public:
	CTopologyXDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CTopologyXView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	afx_msg LRESULT OnMapCtrlMsg(WPARAM wp, LPARAM lp);//{return 0;};
protected: // for view
	INT m_nShowAnchor;
private:
	BOOL DrawAnchors(CDC *pDC);
};

#ifndef _DEBUG  // TopologyXView.cpp �еĵ��԰汾
inline CTopologyXDoc* CTopologyXView::GetDocument() const
   { return reinterpret_cast<CTopologyXDoc*>(m_pDocument); }
#endif

