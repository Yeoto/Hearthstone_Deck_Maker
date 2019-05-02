#include "stdafx.h"
#include "DeckListCtrl.h"
#include <algorithm>

#define ITEM_HEIGHT 40

CDeckListCtrl::CDeckListCtrl() : CCardNotifier(NTM_DECKLISTCTRL), m_nStartIdx(0)
{
}

CDeckListCtrl::~CDeckListCtrl()
{
}

BEGIN_MESSAGE_MAP(CDeckListCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

BOOL CDeckListCtrl::Create(CRect rect, CWnd * pParent)
{
	return CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParent, -1);
}

BOOL CDeckListCtrl::ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam)
{
	switch (eSender)
	{
	case NTM_CARDCTRL:
	{
		CCard* pCard = (CCard*)lParam;
		std::map<CCard*, int>::iterator itrMap;
		if ((itrMap = m_mapCards.find(pCard)) == m_mapCards.end())
		{
			m_mapCards[pCard] = 1;
		}
		else
		{
			if (pCard->eRarity != E_CARDRARITY::E_CARDRARITY_LEGENDARY)
			{
				m_mapCards[pCard] = 2;
			}
		}
		Invalidate();
		return TRUE;
	}
	default:
		return FALSE;
	}
}

void CDeckListCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� __super::OnPaint()��(��) ȣ������ ���ʽÿ�.

	CRect rtList;
	GetClientRect(rtList);
#if defined(_DEBUG)
	dc.Rectangle(rtList);
#endif

	std::vector<std::pair<CCard*, int>> vecCardList(m_mapCards.begin(), m_mapCards.end());
	auto sort = [](std::pair<CCard*, int>& card1, std::pair<CCard*, int>& card2) -> BOOL
	{
		CCard* pCard1 = card1.first;
		CCard* pCard2 = card2.first;

		if (pCard1->nCost < pCard2->nCost)
		{
			return TRUE;
		}
		else if (pCard1->nCost == pCard2->nCost)
		{
			if (pCard1->strName < pCard2->strName)
				return TRUE;
		}
		else
		{
			return FALSE;
		}
	};
	//qsort(&vecCardList.begin(), vecCardList.size(), sizeof(std::pair<CCard*,int>), sortByCost);
	std::sort(vecCardList.begin(), vecCardList.end(), sort);

	int lCardCnt = vecCardList.size();
	for (int i = 0; i < lCardCnt; i++)
	{
		CCard* pCard = vecCardList[i].first;
		int nCard = vecCardList[i].second;

		CRect rectTemp(rtList);
		rectTemp.top = ITEM_HEIGHT * i;
		rectTemp.bottom = ITEM_HEIGHT * (i + 1);
		dc.Rectangle(rectTemp);
		CString strCard;
		strCard.Format(_T("%d | %ws | %d"), pCard->nCost, pCard->strName.c_str(), nCard);
		rectTemp.DeflateRect(10, 0, 10, 0);
		dc.DrawText(strCard, rectTemp, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
}


void CDeckListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	__super::OnRButtonDown(nFlags, point);
}


BOOL CDeckListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return __super::OnMouseWheel(nFlags, zDelta, pt);
}
