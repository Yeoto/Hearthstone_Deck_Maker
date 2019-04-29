#include "stdafx.h"
#include "CardCtrl.h"
#include "CardData.h"

CCardCtrl::CCardCtrl() :
	m_pCard(nullptr), m_dRatio(1.0)
{
}


CCardCtrl::~CCardCtrl()
{
}

BEGIN_MESSAGE_MAP(CCardCtrl, CStatic)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CCardCtrl::SetCardData(CCard * pCard)
{
	if (pCard == NULL)
		ASSERT(0);

	m_pCard = pCard;

	if (NULL != m_CardImage)
	{
		m_CardImage.Destroy();
	}

	std::wstring temp;
	temp.assign(pCard->imgfilePath.begin(), pCard->imgfilePath.end());
	CString strImgPath = temp.c_str();

	HRESULT hResult = m_CardImage.Load(strImgPath);

	if (FAILED(hResult)) {
		CString strTmp = _T("ERROR: Failed to load :");
		strTmp += strImgPath;
		return;
	}
}

BOOL CCardCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (this->GetBitmap() == NULL)
	{
		if (m_CardImage.GetBPP() == 32)
		{
			if (m_pCard->imgfilePath.substr(m_pCard->imgfilePath.length() - 3, 3).compare("png") == 0)
			{
				long w = m_CardImage.GetWidth();
				long h = m_CardImage.GetHeight();
				unsigned char * pCol = 0;
				for (long y = 0; y < h; y++)
				{
					for (long x = 0; x < w; x++)
					{
						pCol = (unsigned char *)m_CardImage.GetPixelAddress(x, y);
						unsigned char alpha = pCol[3];
						if (alpha != 255)
						{
							pCol[0] = ((pCol[0] * alpha) + 128) >> 8;
							pCol[1] = ((pCol[1] * alpha) + 128) >> 8;
							pCol[2] = ((pCol[2] * alpha) + 128) >> 8;
						}
					}
				}
			}
			m_CardImage.SetHasAlphaChannel(true);
		}
	}

	return 0;
}

BOOL CCardCtrl::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID)
{
	if (m_pCard == NULL || m_CardImage == NULL)
	{
		ASSERT(0);
		return FALSE;
	}
	CRect rectTemp = rect;
	long w = m_CardImage.GetWidth();
	long h = m_CardImage.GetHeight();

	rectTemp.right = rect.left + w;
	rectTemp.bottom = rect.top + h;

	return CStatic::Create(lpszText, dwStyle, rectTemp, pParentWnd, nID);
}

void CCardCtrl::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(rect);

#if defined(_DEBUG)
	dc.Rectangle(rect);
#endif

	long w = m_CardImage.GetWidth();
	long h = m_CardImage.GetHeight();

	m_CardImage.Draw(dc.GetSafeHdc(), rect.left, rect.top - (60 * m_dRatio), w * m_dRatio, h * m_dRatio);
	return CStatic::OnPaint();
}