#include "stdafx.h"
#include "CardListMgr.h"

#include <direct.h>

#include "Base64.h"
#include "MCurl.h"
#include "Card.h"
#include "ProgressDlg.h"
#include "MetaDeckData.h"

CCardListMgr* CCardListMgr::m_pInstance;

CCardListMgr::CCardListMgr()
{
	MakeTempCard();
}

CCardListMgr::~CCardListMgr()
{
	for (CCard* pCard : m_vecCardList)
	{
		if (pCard != NULL)
		{
			delete pCard;
			pCard = NULL;
		}
	}
	for (CCard* pCard : m_vecHeroSkinList)
	{
		if (pCard != NULL)
		{
			delete pCard;
			pCard = NULL;
		}
	}
	for (std::pair<E_CARDCLASS, std::vector<CMetaDeckData*>> pairMetaDeck : m_mapMetaDeckList)
	{
		for (CMetaDeckData* pMetaDeck : pairMetaDeck.second)
		{
			if (pMetaDeck != NULL)
			{
				delete pMetaDeck;
				pMetaDeck = NULL;
			}
		}
	}
	delete m_pTempCard;
}

CCardListMgr * CCardListMgr::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CCardListMgr();
		atexit(destroy);
	}

	return m_pInstance;
}

std::vector<CCard*> CCardListMgr::GetFilteredList(std::vector<CCard*> vecCardList)
{
	std::vector<CCard*> vecFiltered;
	for (auto itr : vecCardList)
	{
		if (m_Filter.IsAgree(itr))
			vecFiltered.push_back(itr);
	}
	return vecFiltered;
}

std::vector<CCard*> CCardListMgr::GetFilteredList()
{
	return GetFilteredList(m_vecCardList);
}

BOOL CCardListMgr::CheckCardData(CCard* pCard)
{
	return TRUE;
}

void CCardListMgr::AddCard(CCard * pCard)
{
	if (pCard->nCost < 0)
		m_vecHeroSkinList.push_back(pCard);
	else
		m_vecCardList.push_back(pCard);
}

void CCardListMgr::MakeTempCard()
{
	TCHAR path[_MAX_PATH] = _T("");
	GetModuleFileName(NULL, path, _MAX_PATH);

	std::wstring temp = path;
	std::string pathT; pathT.assign(temp.begin(), temp.end());
	std::string UpperPath = pathT.substr(0, pathT.rfind(L'\\'));

	std::string imgPath = UpperPath + "\\Image\\";

	m_pTempCard = new CCard();
	m_pTempCard->strCardID = _T("temp");
	m_pTempCard->urlImg = _T("/temp.png");
	//m_pTempCard->imgfilePath = imgPath;
	m_pTempCard->DownloadImg(imgPath);
}

void CCardListMgr::TraceAll()
{
	for (CCard* pCard : m_vecCardList)
		pCard->Trace();
}

void CCardListMgr::DownloadAllImg(BOOL* bContinu)
{
	TCHAR path[_MAX_PATH] = _T("");
	GetModuleFileName(NULL, path, _MAX_PATH);

	std::wstring temp = path;
	std::string pathT; pathT.assign(temp.begin(), temp.end());
	std::string UpperPath = pathT.substr(0, pathT.rfind(L'\\'));

	std::string imgPath = UpperPath + "\\Image\\";
	_mkdir(imgPath.c_str());

	for (CCard* pCard : m_vecCardList)
	{
		if ( bContinu == FALSE )
			break;

		if ( pCard->m_CardImage == NULL )
			pCard->DownloadImg(imgPath);
	}
}

void CCardListMgr::AddMetaDeck(E_CARDCLASS eCardClass, CMetaDeckData* pMetaDeck)
{
	if (m_mapMetaDeckList.find(eCardClass) == m_mapMetaDeckList.end())
		m_mapMetaDeckList[eCardClass] = std::vector<CMetaDeckData*>();

	m_mapMetaDeckList[eCardClass].push_back(pMetaDeck);
}

void CCardListMgr::DeckCode2CardList(std::string deckCode, E_CARDCLASS& eDeckClass, std::map<CCard*, int>& mapCards)
{
	mapCards.clear();

	CCardEncoder* pBase64 = CCardEncoder::GetInstance();
	int pTemp[500] = {};
	int nTemp = 0;
	pBase64->Decoding(deckCode.c_str(), pTemp, &nTemp);
	std::vector<int> vecResult(pTemp, pTemp + nTemp);

	int nResultSize = (int)vecResult.size();

	if (nResultSize < 4)
		return;

	int nWild = vecResult[2];
	int nHero = vecResult[3];
	ASSERT(nHero == 1);

	E_CARDCLASS eClass = E_CARDCLASS_NONE;
	int nStartIdx = 4;
	int nCard = 0;

	auto GetNextCard = [&nStartIdx, &vecResult](const std::vector<CCard*>& vecCardList) -> CCard*
	{
		int nData = 0;
		int nLsh = 0;
		while (((char)vecResult[nStartIdx] & 0x8000) > 0)
		{
			nData |= (vecResult[nStartIdx] & ~0x80) << (nLsh * 7);
			nLsh++;
			nStartIdx++;
		}
		nData |= vecResult[nStartIdx++] << (nLsh * 7);
		auto itr = std::find_if(vecCardList.begin(), vecCardList.end(), [&nData](CCard* pCard) { return pCard->ndbfID == nData;  });
		return *itr;
	};

	try
	{
		for (nCard = 0; nCard < nHero;)
		{
			CCard* pCard = GetNextCard(m_vecHeroSkinList);

			if (pCard != NULL)
			{
				eClass = pCard->eClass;
				nCard++;
			}
			else
			{
				ASSERT(0);
				break;
			}
		}
		eDeckClass = eClass;
		int nSingleCard = vecResult[nStartIdx++];
		for (nCard = 0; nCard < nSingleCard;)
		{
			CCard* pCard = GetNextCard(m_vecCardList);

			if (pCard != NULL)
			{
				mapCards[pCard] = 1;
				nCard++;
			}
			else
			{
				ASSERT(0);
				break;
			}
		}

		int nDoubleCard = vecResult[nStartIdx++];
		for (nCard = 0; nCard < nDoubleCard;)
		{
			CCard* pCard = GetNextCard(m_vecCardList);

			if (pCard != NULL)
			{
				mapCards[pCard] = 2;
				nCard++;
			}
			else
			{
				ASSERT(0);
				break;
			}
		}
	}
	catch (CMemoryException* e)
	{
		AfxMessageBox(_T("올바르지 않은 덱 코드입니다."));
	}
}

void CCardListMgr::CardList2DeckCode(E_CARDCLASS eDeckClass, std::map<CCard*, int> mapDeckList, CString& deckCode)
{
	std::vector<CCard*> vecSingle; vecSingle.clear();
	std::vector<CCard*> vecDouble; vecDouble.clear();

	BOOL bWild = FALSE;
	for (std::pair<CCard*, int> itrPair : mapDeckList)
	{
		if ((itrPair.first->eCardSet & ~E_CARDSET_REGULAR) > 0)
			bWild = TRUE;

		if (itrPair.second == 1)
			vecSingle.push_back(itrPair.first);
		else if (itrPair.second == 2)
			vecDouble.push_back(itrPair.first);
		else
		{ASSERT(0);continue;}
	}

	std::vector<int> vecDecode;
	auto PushBackByDbfID = [&](CCard* pCard)
	{
		int nDbfID = pCard->ndbfID;
		while (nDbfID > 0)
		{
			int n7bit = nDbfID & 0x0000007F;
			nDbfID >>= 7;
			if (nDbfID > 0)
				n7bit |= 0x80;
			vecDecode.push_back(n7bit);
		}

	};
	vecDecode.push_back(0);
	vecDecode.push_back(1);
	vecDecode.push_back(bWild == TRUE ? 1 : 2);
	vecDecode.push_back(1);//Hero Count;
	for (auto itrHeroSkin : m_vecHeroSkinList)
	{
		if (itrHeroSkin->eClass == eDeckClass)
		{
			PushBackByDbfID(itrHeroSkin);
			break;
		}
	}

	vecDecode.push_back((int)vecSingle.size());
	for (auto itrSingle : vecSingle)
		PushBackByDbfID(itrSingle);

	vecDecode.push_back((int)vecDouble.size());
	for (auto itrDouble : vecDouble)
		PushBackByDbfID(itrDouble);

	vecDecode.push_back(0);
	CCardEncoder* pBase64 = CCardEncoder::GetInstance();
	std::string strTemp;
	std::wstring wstrTemp;
	pBase64->Encoding(&vecDecode[0], vecDecode.size(), strTemp);
	wstrTemp.assign(strTemp.begin(), strTemp.end());
	deckCode = wstrTemp.c_str();
}

