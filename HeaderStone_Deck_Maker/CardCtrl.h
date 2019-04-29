#pragma once

#include "CardData.h"

class CCardCtrl :
	public CStatic
{
public:
	CCardCtrl();
	~CCardCtrl();

	void SetCardData(CCard* pCard);
	
	virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
	virtual afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
private:
	CCard* m_pCard;
	CImage m_CardImage;
};

