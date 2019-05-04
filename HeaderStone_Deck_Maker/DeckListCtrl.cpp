#include "stdafx.h"
#include "DeckListCtrl.h"
#include <algorithm>

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

		if ((itrMap = m_mapCards.find(pCard)) == m_mapCards.end())
		{
			m_mapCards[pCard] = 1;
			m_nCardCnt++;
			RemakeCardListVector();
		}
		else
		{
			if (pCard->eRarity != E_CARDRARITY::E_CARDRARITY_LEGENDARY)
			{
				if (m_mapCards[pCard] != 2)
				{
					m_mapCards[pCard] = 2;
					m_nCardCnt++;
					RemakeCardListVector();
				}
			}
		}
		return TRUE;
	}
	default:
		return FALSE;
	}
}

void CDeckListCtrl::RemakeCardListVector()
{
	m_vecCards = std::vector<std::pair<CCard*, int>>(m_mapCards.begin(), m_mapCards.end());
	std::sort(m_vecCards.begin(), m_vecCards.end(), m_fCompare);
	Invalidate();
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
		rtCost.left = 0; rtCost.right = ITEM_HEIGHT;	//���簢������ ��--�� �ϰ�
		rtCost.DeflateRect(5, 5);

		CString strCost;
		int nCost = pCard->nCost < 0 ? 0 : pCard->nCost;
		strCost.Format(_T("%d"), nCost);

		CBrush brCost(RGB(80, 155, 211));
		CBrush* pOldBrush = pDC->SelectObject(&brCost);
		pDC->Rectangle(rtCost);
		pDC->SelectObject(pOldBrush);

		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->DrawText(strCost, rtCost, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	// Name
	{
		CRect rtName = rtItem;
		rtName.left = ITEM_HEIGHT; rtName.right -= ITEM_HEIGHT;	//������ Cost ĭ��ŭ ������, �������� ī�� ���� ��ŭ ���̰� !
		rtName.DeflateRect(5, 5);

		CString strName;
		strName.Format(_T("%s"), pCard->strName.c_str());

		CBrush brCost(RGB(255, 255, 255));
		CBrush* pOldBrush = pDC->SelectObject(&brCost);
		pDC->Rectangle(rtName);
		pDC->SelectObject(pOldBrush);

		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->DrawText(strName, rtName, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	// Card Count
	if (nCount > 1)
	{
		CRect rtCnt = rtItem;
		rtCnt.left = rtCnt.right - ITEM_HEIGHT; rtCnt.right;	//�����ʿ��� ������ ���̸�ŭ ����
		rtCnt.DeflateRect(5, 5);

		CString strName;
		strName.Format(_T("%d"), nCount);

		CPen NewCntPen(PS_SOLID, 1, RGB(0, 0, 0));
		CPen* pOldCntPen = pDC->SelectObject(&NewCntPen);
		pDC->MoveTo(CPoint(rtCnt.left, rtCnt.top));
		pDC->LineTo(CPoint(rtCnt.left, rtCnt.bottom));
		pDC->SelectObject(pOldCntPen);

		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->DrawText(strName, rtCnt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	////////////////////////////////////////////////////////////////////////////////////
	pDC->SelectObject(pOldPen);
	pDC->RestoreDC(nSaveDC);
}

void CDeckListCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� __super::OnPaint()��(��) ȣ������ ���ʽÿ�.

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


void CDeckListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CPoint itemPos = point;
	itemPos.y -= ITEM_TOP_OFFSET;

	int nItemIdx = (itemPos.y / ITEM_HEIGHT) + m_nStartIdx;

	if (nItemIdx >= m_vecCards.size())
		return;

	std::pair<CCard*, int> pairCard = m_vecCards[nItemIdx];
	m_mapCards[pairCard.first]--;
	m_nCardCnt--;
	SendNotify(NULL, (LPARAM)pairCard.first);

	if (m_mapCards[pairCard.first] == 0)
		m_mapCards.erase(pairCard.first);

	RemakeCardListVector();
	__super::OnRButtonDown(nFlags, point);
}


BOOL CDeckListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	BOOL bRedraw = FALSE;
	if ( m_vecCards.size() == 0 )
		return __super::OnMouseWheel(nFlags, zDelta, pt);

	if (zDelta < 0)
	{
		if (m_nStartIdx < m_vecCards.size() - 1)
		{
			m_nStartIdx++;
			bRedraw = TRUE;
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