#pragma once

#include "CardData.h"

class CCardCtrl :
	public CWnd
{
public:
	CCardCtrl();
	~CCardCtrl();

	void SetCardData(CCard* pCard);
	
	virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL Create(const RECT& rect, CWnd* pParentWnd);
	virtual afx_msg void OnPaint();

	void SetRatio(double dRatio);
	DECLARE_MESSAGE_MAP()
private:
	double m_dRatio;
	CCard* m_pCard;
	CImage m_CardImage;
};

