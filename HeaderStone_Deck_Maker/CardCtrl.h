#pragma once

#include "CardData.h"
#include "CardNotifier.h"
#include "NotifyMsg.h"

class CCardCtrl :
	public CWnd, CCardNotifier
{
public:
	CCardCtrl();
	~CCardCtrl();

	DECLARE_MESSAGE_MAP()

public:
	void SetCardData(CCard* pCard);
	
	virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL Create(const RECT& rect, CWnd* pParentWnd);
	virtual afx_msg void OnPaint();
	void SetRatio(double dRatio);

	void SetDrawCard(BOOL bDrawCard);
	void DrawCard(CDC* pDC);

	virtual BOOL ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam);

private:
	BOOL m_bDrawCard;
	double m_dRatio;
	CCard* m_pCard;
	CImage m_CardImage;
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

