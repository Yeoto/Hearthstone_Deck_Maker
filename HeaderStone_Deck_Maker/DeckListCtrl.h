#pragma once
#include <afxwin.h>

#include <vector>
#include <functional>
#include <map>
#include "CardEnum.h"
#include "CardNotifier.h"
#include "NotifyMsg.h"

class CCardListMgr;
class CCard;

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
	BOOL ExecuteNotifyFromDlg(WPARAM wParam, LPARAM lParam);

	void SetDeckClass(E_CARDCLASS eClass) { m_eDeckClass = eClass; }
	E_CARDCLASS GetDeckClass() { return m_eDeckClass; }

	void ResetDeck();
	void SetDeck(std::map<CCard*, int> mapDeckList);
	void GetLocalDeckKey(int& nKey) { nKey = m_nKey; }
	void GetDeck(CString& strDeck, std::map<CCard*, int>& mapDeckList);

	void Add2DeckList(CCard* pCard);
private:
	void GetDeckTypeClr(COLORREF& clrDeck, COLORREF& clrCardCnt);
	void RemakeCardListVector();
	void DrawDeckType(CDC* pDC, CRect rtItem);
	void DrawItem(CDC* pDC, CRect rtItem, CCard* pCard, int nCard);
private:
	int m_nKey;
	std::map<CCard*, int> m_mapCards;
	std::vector<std::pair<CCard*, int>> m_vecCards;
	std::function<BOOL(std::pair<CCard*, int>&, std::pair<CCard*, int>&)> m_fCompare;
	int m_nStartIdx;
	int m_nCardCnt;

	E_CARDCLASS m_eDeckClass;
	CEdit m_ctrlDeckName;
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

