
// TopologyXDoc.h : CTopologyXDoc ��Ľӿ�
//


#pragma once
#include "TopoDev.h"

class CTopologyXDoc : public CDocument
{
protected: // �������л�����
	CTopologyXDoc();
	DECLARE_DYNCREATE(CTopologyXDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CTopologyXDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
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
