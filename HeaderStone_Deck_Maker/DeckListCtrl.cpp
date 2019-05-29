#include "stdafx.h"
#include "DeckListCtrl.h"
#include <algorithm>

#include "Card.h"
#include "CardListMgr.h"
#include "ImportByLocalDlg.h"
#include "ImportByMetaDeck.h"

#define IDC_DECKNAME_EDT 1

#define ITEM_TOP_OFFSET 80
#define ITEM_HEIGHT 40

CDeckListCtrl::CDeckListCtrl() : CCardNotifier(NTM_DECKLISTCTRL), m_nStartIdx(0), m_nCardCnt(0), m_eDeckClass(E_CARDCLASS_NONE), m_nKey(-1)
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
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
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
		if ( wParam == 0 && lParam != NULL )
			Add2DeckList((CCard*)lParam);
		return TRUE;
	}
	case NTM_IMPORTDLG:
		SetDeckClass((E_CARDCLASS)wParam);
		SetDeck(*(std::map<CCard*, int>*)lParam);
		return TRUE;
	case NTM_IMPORTLOCALDLG:
	case NTM_IMPORTMETADECKDLG:
		return ExecuteNotifyFromDlg(wParam, lParam);
	default:
		return FALSE;
	}
}

BOOL CDeckListCtrl::ExecuteNotifyFromDlg(WPARAM wParam, LPARAM lParam)
{
	if (lParam == NULL)
		return FALSE;

	switch (wParam)
	{
	case LocalDeckMsg::MSG_SETLOCALDECKMODIFY:
	{
		m_nKey = *(int*)lParam;
		return TRUE;
	}
	case LocalDeckMsg::MSG_SETLOCALDECKSTRING:
	case MetaDeckMsg::MSG_SETMETADECKSTRING:
	{
		CString strDeckName = *(CString*)lParam;
		m_ctrlDeckName.SetWindowText(strDeckName);
		return TRUE;
	}
	case LocalDeckMsg::MSG_SETLOCALDECKCLASS:
	case MetaDeckMsg::MSG_SETMETADECKCLASS:
	{
		E_CARDCLASS eClass = *(E_CARDCLASS*)lParam;
		SetDeckClass(eClass);
		return TRUE;
	}
	case LocalDeckMsg::MSG_SETLOCALDECKLIST:
	case MetaDeckMsg::MSG_SETMETADECKLIST:
	{
		std::map<CCard*, int> mapDeckList = *(std::map<CCard*, int>*)lParam;
		SetDeck(mapDeckList);
		return TRUE;
	}
	default:
		return FALSE;
	}
}

void CDeckListCtrl::ResetDeck()
{
	m_nKey = -1;
	m_vecCards.clear();
	m_mapCards.clear();
	m_nStartIdx = 0;
	m_nCardCnt = 0;
	m_eDeckClass = E_CARDCLASS_NONE;
	m_ctrlDeckName.SetWindowText(_T(""));
	Invalidate();
	UpdateWindow();
}

void CDeckListCtrl::SetDeck(std::map<CCard*, int> mapDeckList)
{
	m_mapCards = mapDeckList;
	RemakeCardListVector();
	Invalidate();
}

void CDeckListCtrl::GetDeck(CString& strDeck, std::map<CCard*, int>& mapDeckList)
{
	m_ctrlDeckName.GetWindowText(strDeck);

	if (strDeck.GetLength() == 0)
	{
		AfxMessageBox(_T("덱 이름은 필수입니다. 임의의 이름으로 지정됩니다."));
		strDeck = _T("나만의 덱");
	}
	mapDeckList = m_mapCards;
}

void CDeckListCtrl::Add2DeckList(CCard* pCard)
{
	std::map<CCard*, int>::iterator itrMap;

	if (m_nCardCnt == 30)
		return ;

	if (pCard->eClass != E_CARDCLASS_NONE && m_eDeckClass != E_CARDCLASS_NONE && m_eDeckClass != pCard->eClass)
		return ;

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
}

void CDeckListCtrl::GetDeckTypeClr(COLORREF& clrDeck, COLORREF& clrCardCnt)
{
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
	GetDeckTypeClr(clrDeck, clrCardCnt);

	CPen penNone(PS_SOLID, 1, RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&penNone);

	CBrush brCost(clrDeck);
	CBrush* pOldBrush = pDC->SelectObject(&brCost);
	pDC->Rectangle(rtItem);
	pDC->SelectObject(pOldBrush);

	CString strCardCnt;
	strCardCnt.Format(_T("%d / 30"), m_nCardCnt);
	rtItem.DeflateRect(10, 10);

	CFont cFont;
	cFont.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("나눔고딕"));
	CFont* pOldFont = pDC->SelectObject(&cFont);

	pDC->SetTextColor(RGB(0, 0, 0));
	for (int nOffX = -1; nOffX <= 1; nOffX++)
	{
		for (int nOffY = -1; nOffY <= 1; nOffY++)
		{
			CRect rtTemp = rtItem;
			rtTemp.left -= nOffX;
			rtTemp.right -= nOffX;
			rtTemp.top -= nOffY;
			rtTemp.bottom -= nOffY;
			pDC->DrawText(strCardCnt, rtTemp, DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);
		}
	}

	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->DrawText(strCardCnt, rtItem, DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);

	pDC->SelectObject(pOldFont);
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
		CRect rtRarity = rtItem;
		CRect rtCost = rtItem;
		rtRarity.left = 0; rtRarity.right = ITEM_HEIGHT * 0.5;
		rtRarity.DeflateRect(5, 5, 0, 5);
		rtCost.left = ITEM_HEIGHT * 0.5; rtCost.right = ITEM_HEIGHT * 1.5;
		rtCost.DeflateRect(5, 5);

		CString strCost;
		int nCost = pCard->nCost < 0 ? 0 : pCard->nCost;
		E_CARDRARITY eRarity = pCard->eRarity;

		COLORREF clrRarity = RGB(255, 255, 255);
		switch (eRarity)
		{
		case E_CARDRARITY_FREE:
			clrRarity = RGB(202, 174, 136);
			break;
		case E_CARDRARITY_COMMON:
			clrRarity = RGB(244, 251, 255);
			break;
		case E_CARDRARITY_RARE:
			clrRarity = RGB(52, 123, 233);
			break;
		case E_CARDRARITY_EPIC:
			clrRarity = RGB(214, 59, 252);
			break;
		case E_CARDRARITY_LEGENDARY:
			clrRarity = RGB(255, 150, 15);
			break;
		default:
			ASSERT(0);
		}

		CBrush brRarity(clrRarity);
		CBrush* pOldBrush = pDC->SelectObject(&brRarity);
		pDC->Rectangle(rtRarity);
		pDC->SelectObject(pOldBrush);

		CBrush brCost(RGB(80, 155, 211));
		pOldBrush = pDC->SelectObject(&brCost);
		pDC->Rectangle(rtCost);
		pDC->SelectObject(pOldBrush);

		strCost.Format(_T("%d"), nCost);

		CFont cFont;
		cFont.CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("나눔고딕"));
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
		rtName.left = ITEM_HEIGHT * 1.5; rtName.right -= ITEM_HEIGHT;	//왼쪽은 Cost 칸만큼 넓히고, 오른쪽은 카드 갯수 만큼 줄이고 !
		rtName.DeflateRect(5, 5);

		CString strName;
		strName.Format(_T("%s"), pCard->strName.c_str());

		CBrush brCost(RGB(255, 255, 255));
		CBrush* pOldBrush = pDC->SelectObject(&brCost);
		pDC->Rectangle(rtName);
		pDC->SelectObject(pOldBrush);

		CFont cFont;
		cFont.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("나눔고딕"));
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
		cFont.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("나눔고딕"));
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
		CRect rtList;
		GetClientRect(rtList);
		rtList.top = ITEM_TOP_OFFSET;

		InvalidateRect(rtList);
		UpdateWindow();
	}
	return __super::OnMouseWheel(nFlags, zDelta, pt);
}


int CDeckListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CRect rect(lpCreateStruct->x, lpCreateStruct->y, lpCreateStruct->x + lpCreateStruct->cx, lpCreateStruct->y + lpCreateStruct->cy);
	CRect rtName;
	rtName.left = 20;
	rtName.top = 20;
	rtName.right = rect.Width() - 60;
	rtName.bottom = rtName.top + 20;

	m_ctrlDeckName.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, rtName, this, IDC_DECKNAME_EDT);
	return 0;
}

BOOL CDeckListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (::GetDlgCtrlID(pMsg->hwnd) == IDC_DECKNAME_EDT)
	{
		// 에디트 컨트롤 IDC_EDIT9 번의 이벤트 중에서 아래 이벤트가 발생하면
		// 에디트 컨트롤 영역만큼 부모 윈도우의 화면을 갱신한다.
		switch (pMsg->message)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_KILLFOCUS:
		{
			CRect rc;
			m_ctrlDeckName.GetWindowRect(rc);
			ScreenToClient(rc);
			InvalidateRect(rc);
		}
		}
	}
	return __super::PreTranslateMessage(pMsg);
}
