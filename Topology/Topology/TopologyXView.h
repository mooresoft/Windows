
// TopologyXView.h : CTopologyXView 类的接口
//

#pragma once
#include "TopologyXDoc.h"
#include "WxMapCtrl.h"

class CTopologyXView : public CView
{
protected: // 仅从序列化创建
	CTopologyXView();
	DECLARE_DYNCREATE(CTopologyXView)

//成员
protected:
	CWxMapCtrl *m_pMapCtrl;
	CBitmap m_bmAnchorOn;
	CBitmap m_bmAnchorOff;
	CFont m_font;

// 特性
public:
	CTopologyXDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CTopologyXView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // TopologyXView.cpp 中的调试版本
inline CTopologyXDoc* CTopologyXView::GetDocument() const
   { return reinterpret_cast<CTopologyXDoc*>(m_pDocument); }
#endif

