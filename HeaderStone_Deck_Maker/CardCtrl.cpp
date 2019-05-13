#include "stdafx.h"
#include "CardCtrl.h"
#include "CardListMgr.h"

#include "Card.h"

#include <direct.h>

#define TOP_OFFSET		-20
#define LEFT_OFFSET		-10
#define BOTTOM_OFFSET	-25
#define RIGHT_OFFSET	-10
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

	if ( m_pCardImage->GetBits() != NULL )
	{
		long w = m_pCardImage->GetWidth();
		long h = m_pCardImage->GetHeight();
		m_pCardImage->Draw(dc.GetSafeHdc(), rect.left + (m_Offset.left * m_dRatio), rect.top + (m_Offset.top * m_dRatio), w * m_dRatio, h * m_dRatio);
	}
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
