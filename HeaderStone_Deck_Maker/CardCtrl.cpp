#include "stdafx.h"
#include "CardCtrl.h"
#include "CardListMgr.h"

#include "Card.h"
#include "RegistryUtil.h"

#include <direct.h>

#define _USE_MATH_DEFINES
#include <math.h>


#define TOP_OFFSET		-20
#define LEFT_OFFSET		-10
#define BOTTOM_OFFSET	-25
#define RIGHT_OFFSET	-10

#define BOOKMARK_SIZE 60

CCardCtrl::CCardCtrl() :
	CCardNotifier(NTM_CARDCTRL), m_pCard(nullptr), m_dRatio(1.0)
{
	
}


CCardCtrl::~CCardCtrl()
{
}

BEGIN_MESSAGE_MAP(CCardCtrl, CStatic)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

void CCardCtrl::SetCardData(CCard * pCard)
{
	if (pCard == NULL)
		ASSERT(0);

	m_pCard = pCard;
	if (pCard->m_CardImage == NULL)
	{
		TCHAR path[_MAX_PATH] = _T("");
		GetModuleFileName(NULL, path, _MAX_PATH);

		std::wstring temp = path;
		std::string pathT; pathT.assign(temp.begin(), temp.end());
		std::string UpperPath = pathT.substr(0, pathT.rfind(L'\\'));

		std::string imgPath = UpperPath + "\\Image\\";
		_mkdir(imgPath.c_str());

		pCard->DownloadImg(imgPath);
	}
	m_pCardImage = &pCard->m_CardImage;

	m_Offset.top = TOP_OFFSET;
	m_Offset.left = LEFT_OFFSET;
	m_Offset.bottom = BOTTOM_OFFSET;
	m_Offset.right = RIGHT_OFFSET;

	if (pCard->eType == E_CARDTYPE_HERO)
	{
		m_Offset.top -= 15;
		m_Offset.bottom += 15;
		m_Offset.left -= 8;
		m_Offset.right += 8;
	}
}

BOOL CCardCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (m_pCardImage->GetBPP() == 32)
	{
		long w = m_pCardImage->GetWidth();
		long h = m_pCardImage->GetHeight();
		unsigned char * pCol = 0;
		for (long y = 0; y < h; y++)
		{
			for (long x = 0; x < w; x++)
			{
				pCol = (unsigned char *)m_pCardImage->GetPixelAddress(x, y);
				unsigned char alpha = pCol[3];
				if (alpha != 255)
				{
					pCol[0] = ((pCol[0] * alpha) + 128) >> 8;
					pCol[1] = ((pCol[1] * alpha) + 128) >> 8;
					pCol[2] = ((pCol[2] * alpha) + 128) >> 8;
				}
			}
		}
		m_pCardImage->SetHasAlphaChannel(true);
	}

	return 0;
}

BOOL CCardCtrl::Create(const RECT & rect, CWnd * pParentWnd)
{
	if (m_pCard == NULL || m_pCardImage == NULL)
	{
		ASSERT(0);
		return FALSE;
	}
	CRect rectTemp = rect;
	long w = m_pCardImage->GetWidth();
	long h = m_pCardImage->GetHeight();

	rectTemp.right = rect.left + w + m_Offset.left + m_Offset.right;
	rectTemp.bottom = rect.top + h + m_Offset.top + m_Offset.bottom;

	return CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rectTemp, pParentWnd, -1);
}

void CCardCtrl::OnPaint()
{
	if (m_bDrawCard == FALSE)
		return CWnd::OnPaint();

	CPaintDC dc(this);

	CRect rect;
	GetClientRect(rect);

#if defined(_DEBUG)
	dc.Rectangle(rect);
#endif

	if (m_pCardImage->GetBits() != NULL)
	{
		if ( m_pCardImage != NULL )
			m_pCardImage->Destroy();

		TCHAR path[_MAX_PATH] = _T("");
		GetModuleFileName(NULL, path, _MAX_PATH);

		std::wstring temp = path;
		std::string pathT; pathT.assign(temp.begin(), temp.end());
		std::string UpperPath = pathT.substr(0, pathT.rfind(L'\\'));

		std::string imgPath = UpperPath + "\\Image\\";
		_mkdir(imgPath.c_str());

		m_pCard->DownloadImg(imgPath);
	}

	//if ( m_pCardImage->GetBits() != NULL )
	//{
	//	long w = m_pCardImage->GetWidth();
	//	long h = m_pCardImage->GetHeight();
	//	m_pCardImage->Draw(dc.GetSafeHdc(), rect.left + (m_Offset.left * m_dRatio), rect.top + (m_Offset.top * m_dRatio), w * m_dRatio, h * m_dRatio);
	//}
	DrawCard(&dc);
}

void CCardCtrl::SetRatio(double dRatio)
{
	m_dRatio = dRatio;

	if (m_pCardImage != NULL)
	{
		long w = m_pCardImage->GetWidth();
		long h = m_pCardImage->GetHeight();
		SetWindowPos(&wndTop, 0, 0, w*m_dRatio + ((m_Offset.right + m_Offset.left) * m_dRatio), h*m_dRatio + ((m_Offset.bottom + m_Offset.top) * m_dRatio), SWP_NOZORDER | SWP_NOMOVE);
	}
}

void CCardCtrl::SetDrawCard(BOOL bDrawCard)
{
	m_bDrawCard = bDrawCard; 
	const CWnd* temp = (bDrawCard == TRUE) ? &wndTopMost : &wndBottom;
	ShowWindow(bDrawCard);
	SetWindowPos(temp, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	Invalidate();
}

void CCardCtrl::DrawCard(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);

	if (m_pCardImage != NULL)
	{
		long w = m_pCardImage->GetWidth();
		long h = m_pCardImage->GetHeight();
		m_pCardImage->Draw(pDC->GetSafeHdc(), rect.left + (m_Offset.left * m_dRatio), rect.top + (m_Offset.top * m_dRatio), w * m_dRatio, h * m_dRatio);
	}

	if (CRegistryUtil::IsBookMark(m_pCard->ndbfID))
	{
		CRect rtBookmark;
		double r = BOOKMARK_SIZE * m_dRatio;
		this->GetClientRect(rtBookmark);
		rtBookmark.left = rtBookmark.right - r;
		rtBookmark.bottom = rtBookmark.top + r;

#if defined(_DEBUG)
		pDC->Rectangle(rtBookmark);
#endif

		CBrush brStar(RGB(245, 231, 99));
		CBrush* pOldBrush = pDC->SelectObject(&brStar);

		POINT pt[11];
		pt[0].x  = rtBookmark.left + r/2;
		pt[0].y  = rtBookmark.top + 0;
		pt[1].x  = rtBookmark.left + 19;
		pt[1].y  = rtBookmark.top + 15;
		pt[2].x  = rtBookmark.left + 25;
		pt[2].y  = rtBookmark.top + 0;
		pt[3].x  = rtBookmark.left + 31;
		pt[3].y  = rtBookmark.top + 15;
		pt[4].x  = rtBookmark.left + 55;
		pt[4].y  = rtBookmark.top + 15;
		pt[5].x  = rtBookmark.left + 35;
		pt[5].y  = rtBookmark.top + 25;
		pt[6].x  = rtBookmark.left + 45;
		pt[6].y  = rtBookmark.top + 45;
		pt[7].x  = rtBookmark.left + 25;
		pt[7].y  = rtBookmark.top + 32;
		pt[8].x  = rtBookmark.left + 5 ;
		pt[8].y  = rtBookmark.top + 45;
		pt[9].x  = rtBookmark.left + 15;
		pt[9].y  = rtBookmark.top + 25;
		pt[10].x = rtBookmark.left + 0 ;
		pt[10].y = rtBookmark.top + 15;

		pDC->Polygon(pt, 11);

// 		pDC->BeginPath();
// 		for (int i = 0; i < 10; i+=2)
// 		{
// 			int x1 = sin(72.0 * i * M_PI / 180) * r / 2;
// 			int y1 = cos(72.0 * i * M_PI / 180) * r / 2;
// 			int x2 = sin(72.0 * ((i + 2) % 5) * M_PI / 180) * r / 2;
// 			int y2 = cos(72.0 * ((i + 2) % 5) * M_PI / 180) * r / 2;
// 
// 			pDC->MoveTo(rtBookmark.left + r/2 + x1, rtBookmark.top + r / 2 - y1);
// 			pDC->LineTo(rtBookmark.left + r/2 + x2, rtBookmark.top + r / 2 - y2);
// 		}
// 		pDC->EndPath();
// 		pDC->StrokeAndFillPath();

		pDC->SelectObject(pOldBrush);
	}
}

BOOL CCardCtrl::ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}


void CCardCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if ( m_bDrawCard == TRUE )
		SendNotify(NULL, (LPARAM)m_pCard);

	__super::OnLButtonUp(nFlags, point);
}


void CCardCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!CRegistryUtil::SetBookMark(m_pCard->ndbfID))
	{
		ASSERT(0);
	}
	Invalidate(TRUE);
	UpdateWindow();
	__super::OnRButtonUp(nFlags, point);
}
