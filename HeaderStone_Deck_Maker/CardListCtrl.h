#pragma once
#include <afxwin.h>

#include <vector>
#include "CardCtrl.h"
#include "CardData.h"
#include "CardNotifier.h"

class CCardListCtrl :
	public CWnd, public CCardNotifier
{
public:
	CCardListCtrl();
	virtual ~CCardListCtrl();

	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL Create(CRect rect, CWnd* pParent);

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();

	virtual BOOL ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam);
public:
	void CalcColRowMaxPage(BOOL bRepos = TRUE);
	void ReposCards();
	void ModifyCardData();
	void SetStartIndex(int nIdx) { m_nStartIdx = nIdx; }
private:
	void InvalidateAll();
private:
	CWnd* m_pTempCtrl;
	std::vector<CWnd*> m_vecCardCtrl;

	double m_dRatio;
	int m_nCol;
	int m_nRow;
	int m_nStartIdx;
	int m_nMaxIdx;
public:
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

