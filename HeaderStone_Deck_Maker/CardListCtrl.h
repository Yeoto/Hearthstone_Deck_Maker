#pragma once
#include <afxwin.h>

#include <vector>
#include "CardCtrl.h"
#include "CardData.h"

class CCardListCtrl :
	public CWnd
{
public:
	CCardListCtrl();
	virtual ~CCardListCtrl();

	BOOL Create(CRect rect, CWnd* pParent);

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();

public:
	void CalcColRowMaxPage(BOOL bRepos = TRUE);
	void ReposCards();
	void ModifyCardData();

private:
	void InvaliDateAll();
private:
	CWnd* m_pTempCtrl;
	std::vector<CWnd*> m_vecCardCtrl;

	double m_dRatio;
	int m_nCol;
	int m_nRow;
	int m_nStartIdx;
	int m_nMaxPage;
public:
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

