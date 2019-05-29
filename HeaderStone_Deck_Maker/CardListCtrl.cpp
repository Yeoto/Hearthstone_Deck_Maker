#include "stdafx.h"
#include "CardListCtrl.h"

#include "Card.h"
#include "CardListMgr.h"
#include "CardCtrl.h"

#define SIDEBTN_OFFSET 50

CCardListCtrl::CCardListCtrl() : CCardNotifier(NTM_CARDLISTCTRL), m_dRatio(1.0)
{
	WNDCLASS    wndcls;
	HINSTANCE   hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, _T("CardListCtrl"), &wndcls)))
	{
		wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance = hInst;
		wndcls.hIcon = NULL;
		wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = _T("CardListCtrl");

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return;
		}
	}
}


CCardListCtrl::~CCardListCtrl()
{
}

BEGIN_MESSAGE_MAP(CCardListCtrl, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

BOOL CCardListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CCardCtrl* pCardCtrl = new CCardCtrl();

	pCardCtrl->SetCardData(CCardListMgr::GetInstance()->GetTempCard());
	pCardCtrl->Create(CRect(0, 0, 0, 0), this);
	pCardCtrl->SetDrawCard(FALSE);

	m_pTempCtrl = pCardCtrl;

	std::wstring imgPath = CCardListMgr::GetInstance()->GetImgPath();
	imgPath += _T("bookmark.png");

	HRESULT hResult = m_ImgBookmark.Load(imgPath.c_str());

	if (FAILED(hResult))
	{
		AfxMessageBox(_T("Bookmark 이미지 파일이 존재하지 않습니다. 기능이 제한됩니다."));
	}

	ModifyCardData();
	CalcColRowMaxPage();
	return TRUE;
}

BOOL CCardListCtrl::Create(CRect rect, CWnd * pParent)
{
	return CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParent, -1);
}

BOOL CCardListCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext /*= NULL*/)
{
	return Create(rect, pParentWnd);
}

void CCardListCtrl::CalcColRowMaxPage(BOOL bRepos)
{
	CRect rectCard;
	m_pTempCtrl->GetClientRect(rectCard);
	int nCardWidth = rectCard.Width();
	int nCardHeight = rectCard.Height();

	CRect rectList;
	this->GetClientRect(rectList);
	int nListWidth = rectList.Width() - (SIDEBTN_OFFSET * 2);
	int nListHeight = rectList.Height();

	m_nCol = nListWidth / nCardWidth;
	m_nRow = nListHeight / nCardHeight;

	if (m_nCol < 1) m_nCol = 1;
	if (m_nRow < 1) m_nRow = 1;

	int nAllCardCnt = CCardListMgr::GetInstance()->GetFilteredList().size();
	m_nMaxIdx = nAllCardCnt;

	if (m_vecCardCtrl.size() < m_nCol * m_nRow)
	{
		CCardListMgr* pCardListMgr = CCardListMgr::GetInstance();
		int nCurSize = m_vecCardCtrl.size();
		for (int i = 0; i < (m_nCol * m_nRow) - nCurSize; i++)
		{
			CCardCtrl* pCardCtrl = new CCardCtrl();

			pCardCtrl->SetCardData(pCardListMgr->GetTempCard());
			pCardCtrl->Create(CRect(0, 0, 0, 0), this);
			pCardCtrl->SetDrawCard(FALSE);
			pCardCtrl->SetRatio(m_dRatio);
			pCardCtrl->SetBookMarkImage(&m_ImgBookmark);
			m_vecCardCtrl.push_back(pCardCtrl);
		}
	}
	else if (m_vecCardCtrl.size() > m_nCol * m_nRow)
	{
		CCardListMgr* pCardListMgr = CCardListMgr::GetInstance();
		while ((m_vecCardCtrl.size() - (m_nCol * m_nRow)) > 0)
		{
			CWnd* pWnd = m_vecCardCtrl.back();
			pWnd->DestroyWindow();
			delete pWnd;
			m_vecCardCtrl.pop_back();
		}
	}

	if (bRepos)
	{
		ReposCards();
		ModifyCardData();
	}
}

void CCardListCtrl::ReposCards()
{
	CRect cardrect;
	CRect rtCardList;
	this->GetClientRect(rtCardList);
	m_pTempCtrl->GetClientRect(cardrect);
	int nCardWidth = cardrect.Width();
	int nCardHeight = cardrect.Height();
	int nCardListCnt = m_vecCardCtrl.size();

	int nTopOffset = (rtCardList.Height() - (m_nRow * nCardHeight)) / 2;
	int nLeftOffset = (rtCardList.Width() - (m_nCol * nCardWidth) - (SIDEBTN_OFFSET * 2)) / 2;
	for (int i = 0; i < nCardListCnt; i++)
	{
		CWnd* pWnd = m_vecCardCtrl[i];
		int nX = (i % m_nCol) * nCardWidth + nLeftOffset;
		int nY = (i / m_nCol) * nCardHeight + nTopOffset;

		pWnd->MoveWindow(nX + SIDEBTN_OFFSET, nY, nCardWidth, nCardHeight);
	}
}

void CCardListCtrl::ModifyCardData()
{
	ASSERT(m_nCol * m_nRow == m_vecCardCtrl.size());
	std::vector<CCard*> vecCardList = CCardListMgr::GetInstance()->GetFilteredList();
	std::sort(vecCardList.begin(), vecCardList.end(), 
		[](const CCard* pCard1, const CCard* pCard2)
	{
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
	});
	m_vecFilteredCard = vecCardList;
	ModifyCardCtrl();
}

void CCardListCtrl::ModifyCardCtrl()
{
	for (int i = 0; i < m_nCol * m_nRow; i++)
	{
		CCardCtrl* pWnd = (CCardCtrl*)m_vecCardCtrl[i];
		int nCardIdx = m_nStartIdx + i;
		if (nCardIdx >= m_vecFilteredCard.size())
		{
			pWnd->SetDrawCard(FALSE);
		}
		else
		{
			pWnd->SetDrawCard(TRUE);
			pWnd->SetCardData(m_vecFilteredCard[nCardIdx]);
		}
	}
	InvalidateAll();
}

void CCardListCtrl::InvalidateAll()
{
	for (CWnd* pWnd : m_vecCardCtrl)
	{
		CRect rect;
		pWnd->GetClientRect(rect);
		pWnd->InvalidateRect(rect, TRUE);
	}
	this->Invalidate();
	UpdateWindow();
}

void CCardListCtrl::OnPaint()
{
	CPaintDC dc(this);

	CRect rect, rectTemp;
	GetClientRect(rect);
	rectTemp = rect;
	dc.Rectangle(rect);

#if defined(_DEBUG)
	rectTemp.right = rectTemp.left + SIDEBTN_OFFSET;
	dc.Rectangle(rectTemp);

	rectTemp = rect;
	rectTemp.left = rectTemp.right - SIDEBTN_OFFSET;
	dc.Rectangle(rectTemp);
#endif
}


BOOL CCardListCtrl::ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam)
{
	switch (eSender)
	{
	case NTM_CARDCTRL:
		if ( wParam == 1 && lParam == NULL )
			CalcColRowMaxPage(TRUE);
		return TRUE;
	case NTM_FILTERDLG:
		m_nStartIdx = 0;
		CalcColRowMaxPage(TRUE);
		return TRUE;
	case NTM_DECKLISTCTRL:
	{
		if (wParam != -1)
			return FALSE;

		CCard* pCard = (CCard*)lParam;
		auto it = std::find(m_vecFilteredCard.begin(), m_vecFilteredCard.end(), pCard);

		if (it == m_vecFilteredCard.end())
			return FALSE;

		int nIndex = it - m_vecFilteredCard.begin();

		if (m_nStartIdx < nIndex)
		{
			int nTemp = m_nStartIdx;
			while (m_nStartIdx <= nIndex)
			{
				nTemp = m_nStartIdx;
				m_nStartIdx += (m_nCol * m_nRow);
			}
			m_nStartIdx = nTemp;
		}
		else
		{
			while (m_nStartIdx > nIndex)
				m_nStartIdx -= (m_nCol * m_nRow);
		}

		ModifyCardData();
		InvalidateAll();
		return TRUE;
	}
	default:
		return FALSE;
	}
	return FALSE;
}

void CCardListCtrl::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_pTempCtrl->DestroyWindow();
	for (CWnd* pWnd : m_vecCardCtrl)
	{
		pWnd->DestroyWindow();
		delete pWnd;
	}
	delete m_pTempCtrl;
}


void CCardListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nX = point.x;
	int nY = point.y;

	CRect rtListCtrl;
	GetClientRect(rtListCtrl);

	CRect rtLeftBtn;
	rtLeftBtn.top = rtListCtrl.top;
	rtLeftBtn.bottom = rtListCtrl.bottom;
	rtLeftBtn.left = rtListCtrl.left;
	rtLeftBtn.right = SIDEBTN_OFFSET;

	CRect rtRightBtn;
	rtRightBtn.top = rtListCtrl.top;
	rtRightBtn.bottom = rtListCtrl.bottom;
	rtRightBtn.left = rtListCtrl.right - SIDEBTN_OFFSET;
	rtRightBtn.right = rtListCtrl.right;

	if (rtLeftBtn.PtInRect(point))
	{
		m_nStartIdx -= m_nCol * m_nRow;
		if (m_nStartIdx < 0)
			m_nStartIdx = 0;
		else
			ModifyCardCtrl();
	}
	else if (rtRightBtn.PtInRect(point))
	{
		m_nStartIdx += m_nCol * m_nRow;
		if (m_nStartIdx >= m_nMaxIdx)
			m_nStartIdx = m_nMaxIdx;
		else
			ModifyCardCtrl();
	}

	CWnd::OnLButtonDown(nFlags, point);
}


BOOL CCardListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
	{
		BOOL bRedraw = FALSE;
		if (zDelta > 0)
		{
			//휠 위로
			m_dRatio += 0.05;
			if (m_dRatio > 1.0)
				m_dRatio = 1.0;
			else
				bRedraw = TRUE;
		}
		else if (zDelta < 0)
		{
			//휠 아래로
			m_dRatio -= 0.05;
			if (m_dRatio < 0.2)
				m_dRatio = 0.2;
			else
				bRedraw = TRUE;
		}

		if (bRedraw == TRUE)
		{
			((CCardCtrl*)m_pTempCtrl)->SetRatio(m_dRatio);
			CalcColRowMaxPage(FALSE);
			for (CWnd* pWnd : m_vecCardCtrl)
				((CCardCtrl*)pWnd)->SetRatio(m_dRatio);
			ReposCards();
			ModifyCardCtrl();
		}
	}
	else
	{
		BOOL bRedraw = TRUE;
		if (zDelta > 0)
		{
			//휠 위로
			if (m_nStartIdx == 0)
				bRedraw = FALSE;

			m_nStartIdx -= m_nCol * m_nRow;
			if (m_nStartIdx < 0)
			{
				m_nStartIdx = 0;
			}
		}
		else if (zDelta < 0)
		{
			//휠 아래로
			int temp = m_nStartIdx;
			temp += m_nCol * m_nRow;
			bRedraw = FALSE;
			if (temp < m_nMaxIdx)
			{
				m_nStartIdx = temp;
				bRedraw = TRUE;
			}
		}

		if (bRedraw)
			ModifyCardCtrl();
	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
