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
	CCardNotifier(NTM_CARDCTRL), m_pCard(nullptr), m_dRatio(1.0), m_pCardImage(nullptr)
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
		std::wstring imgPath = CCardListMgr::GetInstance()->GetImgPath();
		_wmkdir(imgPath.c_str());

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

void CCardCtrl::SetBookMarkImage(CImage* pImageBookmark)
{
	if (pImageBookmark == NULL)
	{
		ASSERT(0);
		return;
	}

	m_pBookmarkImgage = pImageBookmark;
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

		std::wstring imgPath = CCardListMgr::GetInstance()->GetImgPath();
		_wmkdir(imgPath.c_str());

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
		rtBookmark.right -= (20 * m_dRatio);
		rtBookmark.left = rtBookmark.right - r;
		rtBookmark.top += (20 * m_dRatio);
		rtBookmark.bottom = rtBookmark.top + r;

#if defined(_DEBUG)
		pDC->Rectangle(rtBookmark);
#endif
		if ( m_pBookmarkImgage != NULL )
			m_pBookmarkImgage->Draw(pDC->GetSafeHdc(), rtBookmark);
	}
}

BOOL CCardCtrl::ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}


void CCardCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if ( m_bDrawCard == TRUE )
		SendNotify(NULL, (LPARAM)m_pCard);

	__super::OnLButtonUp(nFlags, point);
}


void CCardCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (!CRegistryUtil::SetBookMark(m_pCard->ndbfID))
	{
		ASSERT(0);
	}
	SendNotify(1, NULL);
	Invalidate(TRUE);
	UpdateWindow();
	__super::OnRButtonUp(nFlags, point);
}
