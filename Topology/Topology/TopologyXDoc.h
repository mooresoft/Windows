
// TopologyXDoc.h : CTopologyXDoc 类的接口
//


#pragma once
#include "TopoDev.h"

class CTopologyXDoc : public CDocument
{
protected: // 仅从序列化创建
	CTopologyXDoc();
	DECLARE_DYNCREATE(CTopologyXDoc)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CTopologyXDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
protected:
	std::map<UINT64, CTopoDev> m_mapDev;
	std::vector<UINT> m_vecDevId;
	double r_matrix[MAX_DIM*MAX_DIM];
	HPVEC2D pos_arr[MAX_DIM];
public:
	std::map<UINT64, CTopoDev> *GetDevMap(){return &(m_mapDev);}
	std::vector<UINT> *GetDevIDs(){return &m_vecDevId;};
	double *GetRMatrix(){return r_matrix;}
	HPVEC2D *GetPosArr(){return pos_arr;}
};
