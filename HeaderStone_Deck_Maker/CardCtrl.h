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
	BOOL Create(const RECT& rect, CWnd* pParentWnd, BOOL bUseTemp = FALSE);
	virtual afx_msg void OnPaint();

	void SetRatio(double dRatio);

	void SetDrawCard(BOOL bDrawCard) { m_bDrawCard = bDrawCard; Invalidate(); }
	void DrawCard(CDC* pDC);
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bDrawCard;
	double m_dRatio;
	CCard* m_pCard;
	CImage m_CardImage;
};

