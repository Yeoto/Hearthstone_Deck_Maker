#pragma once
#include <afxwin.h>

#include <vector>
#include <functional>
#include <map>
#include "CardData.h"
#include "CardNotifier.h"
#include "NotifyMsg.h"

class CDeckListCtrl :
	public CWnd, CCardNotifier
{
public:
	CDeckListCtrl();
	~CDeckListCtrl();

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL Create(CRect rect, CWnd* pParent);
	virtual BOOL ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam);

	void SetDeckClass(E_CARDCLASS eClass) { m_eDeckClass = eClass; }
	E_CARDCLASS GetDeckClass() { return m_eDeckClass; }

private:
	void RemakeCardListVector();
	void DrawDeckType(CDC* pDC, CRect rtItem);
	void DrawItem(CDC* pDC, CRect rtItem, CCard* pCard, int nCard);
private:
	std::map<CCard*, int> m_mapCards;
	std::vector<std::pair<CCard*, int>> m_vecCards;
	std::function<BOOL(std::pair<CCard*, int>&, std::pair<CCard*, int>&)> m_fCompare;
	int m_nStartIdx;
	int m_nCardCnt;

	E_CARDCLASS m_eDeckClass;
public:
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

