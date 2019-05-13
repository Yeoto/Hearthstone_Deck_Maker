#include "stdafx.h"
#include "DeckListCtrl.h"
#include <algorithm>

#include "Card.h"
#include "CardListMgr.h"


#define ITEM_TOP_OFFSET 80
#define ITEM_HEIGHT 40

CDeckListCtrl::CDeckListCtrl() : CCardNotifier(NTM_DECKLISTCTRL), m_nStartIdx(0), m_nCardCnt(0), m_eDeckClass(E_CARDCLASS_NONE)
{
	WNDCLASS    wndcls;
	HINSTANCE   hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, _T("DeckListCtrl"), &wndcls)))
	{
		wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance = hInst;
		wndcls.hIcon = NULL;
		wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = _T("DeckListCtrl");

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return ;
		}
	}

	m_fCompare = [](std::pair<CCard*, int>& card1, std::pair<CCard*, int>& card2) -> BOOL
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

		return FALSE;
	};
}

CDeckListCtrl::~CDeckListCtrl()
{
}

BEGIN_MESSAGE_MAP(CDeckListCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

BOOL CDeckListCtrl::Create(CRect rect, CWnd * pParent)
{
	return CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParent, -1);
}

BOOL CDeckListCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext /*= NULL*/)
{
	return Create(rect, pParentWnd);
}

BOOL CDeckListCtrl::ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam)
{
	switch (eSender)
	{
	case NTM_CARDCTRL:
	{
		CCard* pCard = (CCard*)lParam;
		std::map<CCard*, int>::iterator itrMap;

		if (m_nCardCnt == 30)
			return FALSE;

		if (pCard->eClass != E_CARDCLASS_NONE && m_eDeckClass != E_CARDCLASS_NONE && m_eDeckClass != pCard->eClass)
			return FALSE;

		if ((itrMap = m_mapCards.find(pCard)) == m_mapCards.end())
		{
			m_mapCards[pCard] = 1;
			m_nCardCnt++;
			SendNotify(1, (LPARAM)pCard);
			RemakeCardListVector();
			Invalidate();
		}
		else
		{
			if (pCard->eRarity != E_CARDRARITY::E_CARDRARITY_LEGENDARY)
			{
				if (m_mapCards[pCard] != 2)
				{
					m_mapCards[pCard] = 2;
					m_nCardCnt++;
					SendNotify(1, (LPARAM)pCard);
					RemakeCardListVector();
					Invalidate();
				}
			}
		}
		return TRUE;
	}
	case NTM_IMPORTDLG:
	case NTM_IMPORTMETADECKDLG:
		SetDeckClass((E_CARDCLASS)wParam);
		SetDeck(*(std::map<CCard*, int>*)lParam);
		return TRUE;
	default:
		return FALSE;
	}
}

void CDeckListCtrl::ResetDeck()
{
	m_vecCards.clear();
	m_mapCards.clear();
	m_nStartIdx = 0;
	m_nCardCnt = 0;
	m_eDeckClass = E_CARDCLASS_NONE;
	Invalidate();
	UpdateWindow();
}

void CDeckListCtrl::SetDeck(std::map<CCard*, int> mapDeckList)
{
	m_mapCards = mapDeckList;
	RemakeCardListVector();
	Invalidate();
}

void CDeckListCtrl::RemakeCardListVector()
{
	m_vecCards = std::vector<std::pair<CCard*, int>>(m_mapCards.begin(), m_mapCards.end());
	std::sort(m_vecCards.begin(), m_vecCards.end(), m_fCompare);
	m_nCardCnt = 0;
	for (auto itr : m_vecCards)
		m_nCardCnt += itr.second;
}

void CDeckListCtrl::DrawDeckType(CDC* pDC, CRect rtItem)
{
	int nSaveDC = pDC->SaveDC();
	pDC->SetBkMode(TRANSPARENT);

	COLORREF clrDeck = RGB(255, 255, 255);
	COLORREF clrCardCnt = RGB(0, 0, 0);
	switch (m_eDeckClass)
	{
	case E_CARDCLASS_NONE:
		clrDeck = RGB(255, 255, 255);
		clrCardCnt = RGB(0, 0, 0);
		break;
	case E_CARDCLASS_WARRIOR:
		clrDeck = RGB(224, 54, 5);
		clrCardCnt = RGB(0, 0, 0);
		break;
	case E_CARDCLASS_SHAMAN:
		clrDeck = RGB(51, 92, 255);
		clrCardCnt = RGB(0, 0, 0);
		break;
	case E_CARDCLASS_ROGUE:
		clrDeck = RGB(150, 150, 150);
		clrCardCnt = RGB(0, 0, 0);
		break;
	case E_CARDCLASS_PALADIN:
		clrDeck = RGB(240, 240, 0);
		clrCardCnt = RGB(0, 0, 0);
		break;
	case E_CARDCLASS_HUNTER:
		clrDeck = RGB(70, 170, 20);
		clrCardCnt = RGB(0, 0, 0);
		break;
	case E_CARDCLASS_DRUID:
		clrDeck = RGB(167, 108, 73);
		clrCardCnt = RGB(0, 0, 0);
		break;
	case E_CARDCLASS_WARLOCK:
		clrDeck = RGB(100, 20, 150);
		clrCardCnt = RGB(0, 0, 0);
		break;
	case E_CARDCLASS_MAGE:
		clrDeck = RGB(73, 214, 255);
		clrCardCnt = RGB(0, 0, 0);
		break;
	case E_CARDCLASS_PREIST:
		clrDeck = RGB(255, 255, 230);
		clrCardCnt = RGB(0, 0, 0);
		break;
	case E_CARDCLASS_ALL:
		ASSERT(0);
		break;
	default:
		break;
	}

	CPen penNone(PS_SOLID, 1, RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&penNone);

	CBrush brCost(clrDeck);
	CBrush* pOldBrush = pDC->SelectObject(&brCost);
	pDC->Rectangle(rtItem);
	pDC->SelectObject(pOldBrush);

	CString strCardCnt;
	strCardCnt.Format(_T("%d / 30"), m_nCardCnt);
	rtItem.DeflateRect(20, 20);
	pDC->DrawText(strCardCnt, rtItem, DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);

	pDC->SelectObject(pOldPen);
	pDC->RestoreDC(nSaveDC);
}

void CDeckListCtrl::DrawItem(CDC* pDC, CRect rtItem, CCard* pCard, int nCount)
{
	int nSaveDC = pDC->SaveDC();

	pDC->Rectangle(rtItem);

	CPen penNone(PS_SOLID, 0, RGB(255, 255, 255));
	CPen* pOldPen = pDC->SelectObject(&penNone);

	pDC->SetBkMode(TRANSPARENT);

	////////////////////////////////////////////////////////////////////////////////////
	// Cost
	{
		CRect rtCost = rtItem;
		rtCost.left = 0; rtCost.right = ITEM_HEIGHT;	//정사각형으로 편--안 하게
		rtCost.DeflateRect(5, 5);

		CString strCost;
		int nCost = pCard->nCost < 0 ? 0 : pCard->nCost;
		E_CARDRARITY eRarity = pCard->eRarity;

		strCost.Format(_T("%d"), nCost);

		COLORREF clrCost = RGB(255, 255, 255);
		switch (eRarity)
		{
		case E_CARDRARITY_FREE:
			clrCost = RGB(255, 255, 255);
			break;
		case E_CARDRARITY_COMMON:
			clrCost = RGB(244, 251, 255);
			break;
		case E_CARDRARITY_RARE:
			clrCost = RGB(52, 123, 233);
			break;
		case E_CARDRARITY_EPIC:
			clrCost = RGB(214, 59, 252);
			break;
		case E_CARDRARITY_LEGENDARY:
			clrCost = RGB(255, 150, 15);
			break;
		default:
			ASSERT(0);
		}

		CBrush brCost(clrCost);
		CBrush* pOldBrush = pDC->SelectObject(&brCost);
		pDC->Rectangle(rtCost);
		pDC->SelectObject(pOldBrush);

		CFont cFont;
		cFont.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("-사파이어IIM"));
		CFont* pOldFont = pDC->SelectObject(&cFont);

		pDC->SetTextColor(RGB(0,0,0));
		for (int nOffX = -1; nOffX <= 1; nOffX++)
		{
			for (int nOffY = -1; nOffY <= 1; nOffY++)
			{
				CRect rtTemp = rtCost;
				rtTemp.left -= nOffX;
				rtTemp.right -= nOffX;
				rtTemp.top -= nOffY;
				rtTemp.bottom -= nOffY;
				pDC->DrawText(strCost, rtTemp, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
		}

		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->DrawText(strCost, rtCost, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		pDC->SelectObject(pOldFont);
	}
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	// Name
	{
		CRect rtName = rtItem;
		rtName.left = ITEM_HEIGHT; rtName.right -= ITEM_HEIGHT;	//왼쪽은 Cost 칸만큼 넓히고, 오른쪽은 카드 갯수 만큼 줄이고 !
		rtName.DeflateRect(5, 5);

		CString strName;
		strName.Format(_T("%s"), pCard->strName.c_str());

		CBrush brCost(RGB(255, 255, 255));
		CBrush* pOldBrush = pDC->SelectObject(&brCost);
		pDC->Rectangle(rtName);
		pDC->SelectObject(pOldBrush);

		CFont cFont;
		cFont.CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("-사파이어IIM"));
		CFont* pOldFont = pDC->SelectObject(&cFont);

		//pDC->SetTextColor(RGB(0, 0, 0));
		//for (int nOffX = -1; nOffX <= 1; nOffX++)
		//{
		//	for (int nOffY = -1; nOffY <= 1; nOffY++)
		//	{
		//		CRect rtTemp = rtName;
		//		rtTemp.left -= nOffX;
		//		rtTemp.right -= nOffX;
		//		rtTemp.top -= nOffY;
		//		rtTemp.bottom -= nOffY;
		//		pDC->DrawText(strName, rtTemp, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//	}
		//}

		//pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->DrawText(strName, rtName, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		pDC->SelectObject(pOldFont);
	}
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	// Card Count
	if (nCount > 1)
	{
		CRect rtCnt = rtItem;
		rtCnt.left = rtCnt.right - ITEM_HEIGHT; rtCnt.right;	//오른쪽에서 아이템 높이만큼 빼고
		rtCnt.DeflateRect(5, 5);

		CString strName;
		strName.Format(_T("%d"), nCount);

		CPen NewCntPen(PS_SOLID, 1, RGB(0, 0, 0));
		CPen* pOldCntPen = pDC->SelectObject(&NewCntPen);
		pDC->MoveTo(CPoint(rtCnt.left, rtCnt.top));
		pDC->LineTo(CPoint(rtCnt.left, rtCnt.bottom));
		pDC->SelectObject(pOldCntPen);

		CFont cFont;
		cFont.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("-사파이어IIM"));
		CFont* pOldFont = pDC->SelectObject(&cFont);

		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->DrawText(strName, rtCnt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		pDC->SelectObject(pOldFont);
	}
	////////////////////////////////////////////////////////////////////////////////////
	pDC->SelectObject(pOldPen);
	pDC->RestoreDC(nSaveDC);
}

void CDeckListCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 __super::OnPaint()을(를) 호출하지 마십시오.

	CRect rtList;
	GetClientRect(rtList);
	dc.Rectangle(rtList);
	
	CRect rtTop(rtList);
	rtTop.top = 0; rtTop.bottom = ITEM_TOP_OFFSET;
	DrawDeckType(&dc, rtTop);

	int lCardCnt = m_vecCards.size();
	for (int i = m_nStartIdx; i < lCardCnt; i++)
	{
		CCard* pCard = m_vecCards[i].first;
		int nCard = m_vecCards[i].second;

		CRect rtItem(rtList);
		rtItem.top = ITEM_HEIGHT * (i - m_nStartIdx) + ITEM_TOP_OFFSET;
		rtItem.bottom = ITEM_HEIGHT * (i - m_nStartIdx + 1) + ITEM_TOP_OFFSET;
		DrawItem(&dc, rtItem, pCard, nCard);
	}
}


void CDeckListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CPoint itemPos = point;
	itemPos.y -= ITEM_TOP_OFFSET;

	if (itemPos.y < 0)
	{
		//덱 표시 하는 부분임
		return __super::OnLButtonDown(nFlags, point);
	}

	int nItemIdx = (itemPos.y / ITEM_HEIGHT) + m_nStartIdx;

	if (nItemIdx >= m_vecCards.size())
		return;

	std::pair<CCard*, int> pairCard = m_vecCards[nItemIdx];
	m_mapCards[pairCard.first]--;
	m_nCardCnt--;
	SendNotify(0, (LPARAM)pairCard.first);

	if (m_mapCards[pairCard.first] == 0)
		m_mapCards.erase(pairCard.first);

	RemakeCardListVector();
	Invalidate();
	__super::OnLButtonDown(nFlags, point);
}

void CDeckListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	CPoint itemPos = point;
	itemPos.y -= ITEM_TOP_OFFSET;

	if (itemPos.y < 0)
	{
		//덱 표시 하는 부분임
		return __super::OnRButtonDown(nFlags, point);
	}

	int nItemIdx = (itemPos.y / ITEM_HEIGHT) + m_nStartIdx;

	if (nItemIdx >= m_vecCards.size())
		return;

	CCard* pCard = m_vecCards[nItemIdx].first;
	SendNotify(-1, (LPARAM)pCard);
	__super::OnRButtonDown(nFlags, point);
}


BOOL CDeckListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	BOOL bRedraw = FALSE;
	if ( m_vecCards.size() == 0 )
		return __super::OnMouseWheel(nFlags, zDelta, pt);

	if (zDelta < 0)
	{
		if (m_nStartIdx < m_vecCards.size() - 1)
		{
			CRect rtDeck;
			GetClientRect(rtDeck);
			int nLastItemBot = (m_vecCards.size() - m_nStartIdx) * ITEM_HEIGHT + ITEM_TOP_OFFSET;
			if (nLastItemBot > rtDeck.bottom)
			{
				m_nStartIdx++;
				bRedraw = TRUE;
			}
		}
	}
	else if (zDelta > 0)
	{
		if (m_nStartIdx > 0)
		{
			m_nStartIdx--;
			bRedraw = TRUE;
		}
	}

	if (bRedraw == TRUE)
	{
		Invalidate();
		UpdateWindow();
	}
	return __super::OnMouseWheel(nFlags, zDelta, pt);
}
