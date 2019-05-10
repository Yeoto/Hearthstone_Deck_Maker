#include "stdafx.h"
#include "CardData.h"
#include "ProgressDlg.h"

#include <direct.h>

#include "Base64.h"
#include "MCurl.h"

E_CARDSET CCard::GetCardSetByStr(std::wstring str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	if (str.compare(_T("basic")) == 0)
		return E_CARDSET_BASIC;
	else if (str.compare(_T("classic")) == 0)
		return E_CARDSET_CLASSIC;
	else if (str.compare(_T("hall of Fame")) == 0)
		return E_CARDSET_HALL_OF_FAME;
	else if (str.compare(_T("naxxramas")) == 0)
		return E_CARDSET_NAXXRAMAS;
	else if (str.compare(_T("goblins vs gnomes")) == 0)
		return E_CARDSET_GOBLIN_VS_GNOMES;
	else if (str.compare(_T("blackrock mountain")) == 0)
		return E_CARDSET_BLACKCROCK_MOUNTAIN;
	else if (str.compare(_T("the grand tournament")) == 0)
		return E_CARDSET_THE_GRAND_TOURNAMENT;
	else if (str.compare(_T("the league of explorers")) == 0)
		return E_CARDSET_THE_ELAGUE_OF_EXPLORERS;
	else if (str.compare(_T("whispers of the old gods")) == 0)
		return E_CARDSET_WHISPERS_OF_THE_OLD_GODS;
	else if (str.compare(_T("one night in karazhan")) == 0)
		return E_CARDSET_ONE_NIGHT_IN_KARAZHAN;
	else if (str.compare(_T("mean streets of gadgetzan")) == 0)
		return E_CARDSET_MEAN_STREETS_OF_GADGETZAN;
	else if (str.compare(_T("journey to un'goro")) == 0)
		return E_CARDSET_JOURNEY_TO_UNGORO;
	else if (str.compare(_T("knights of the frozen throne")) == 0)
		return E_CARDSET_KNIGHTS_OF_THE_FROZEN_THRONE;
	else if (str.compare(_T("kobolds & catacombs")) == 0)
		return E_CARDSET_KOBOLDS_AND_CATACOMBS;
	else if (str.compare(_T("the witchwood")) == 0)
		return E_CARDSET_THE_WITCHWOOD;
	else if (str.compare(_T("the boomsday project")) == 0)
		return E_CARDSET_THE_BOOMSDAY_PROJECT;
	else if (str.compare(_T("rastakhan's rumble")) == 0)
		return E_CARDSET_RASTAKHAN_S_RUMBLE;
	else if (str.compare(_T("rise of shadows")) == 0)
		return E_CARDSET_RISE_OF_SHADOWS;
	else
		return E_CARDSET_NONE;

	return E_CARDSET_NONE;
}

E_CARDTYPE CCard::GetCardTypeByStr(std::wstring str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	if (str.compare(_T("hero")) == 0)
		return E_CARDTYPE_HERO;
	else if (str.compare(_T("minion")) == 0)
		return E_CARDTYPE_MINION;
	else if (str.compare(_T("spell")) == 0)
		return E_CARDTYPE_SPELL;
	else if (str.compare(_T("enchantment")) == 0)
		return E_CARDTYPE_ENCHANTMENT;
	else if (str.compare(_T("weapon")) == 0)
		return E_CARDTYPE_WEAPON;
	else if (str.compare(_T("hero power")) == 0)
		return E_CARDTYPE_HERO_POWER;
	else
		return E_CARDTYPE_NONE;

	return E_CARDTYPE_NONE;
}

E_CARDRACE CCard::GetCardRaceByStr(std::wstring str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	if (str.compare(_T("")) == 0)
		return E_CARDRACE_NONE;
	else if (str.compare(_T("demon")) == 0)
		return E_CARDRACE_DEMON;
	else if (str.compare(_T("dragon")) == 0)
		return E_CARDRACE_DRAGON;
	else if (str.compare(_T("mech")) == 0)
		return E_CARDRACE_MECH;
	else if (str.compare(_T("murloc")) == 0)
		return E_CARDRACE_MURLOC;
	else if (str.compare(_T("beast")) == 0)
		return E_CARDRACE_BEAST;
	else if (str.compare(_T("pirate")) == 0)
		return E_CARDRACE_PIRATE;
	else if (str.compare(_T("totem")) == 0)
		return E_CARDRACE_TOTEM;
	else if (str.compare(_T("elemental")) == 0)
		return E_CARDRACE_ELEMENTAL;
	else
		return E_CARDRACE_NONE;

	return E_CARDRACE_NONE;
}

E_CARDRARITY CCard::GetCardRarityByStr(std::wstring str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	if (str.compare(_T("free")) == 0)
		return E_CARDRARITY_FREE;
	else if (str.compare(_T("common")) == 0)
		return E_CARDRARITY_COMMON;
	else if (str.compare(_T("rare")) == 0)
		return E_CARDRARITY_RARE;
	else if (str.compare(_T("epic")) == 0)
		return E_CARDRARITY_EPIC;
	else if (str.compare(_T("legendary")) == 0)
		return E_CARDRARITY_LEGENDARY;
	else
		return E_CARDRARITY_FREE;

	return E_CARDRARITY_FREE;
}

E_CARDCLASS CCard::GetCardClassByStr(std::wstring str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	if (str.compare(_T("")) == 0)
		return E_CARDCLASS_NONE;
	else if (str.compare(_T("warrior")) == 0)
		return E_CARDCLASS_WARRIOR;
	else if (str.compare(_T("shaman")) == 0)
		return E_CARDCLASS_SHAMAN;
	else if (str.compare(_T("rogue")) == 0)
		return E_CARDCLASS_ROGUE;
	else if (str.compare(_T("paladin")) == 0)
		return E_CARDCLASS_PALADIN;
	else if (str.compare(_T("hunter")) == 0)
		return E_CARDCLASS_HUNTER;
	else if (str.compare(_T("druid")) == 0)
		return E_CARDCLASS_DRUID;
	else if (str.compare(_T("warlock")) == 0)
		return E_CARDCLASS_WARLOCK;
	else if (str.compare(_T("mage")) == 0)
		return E_CARDCLASS_MAGE;
	else if (str.compare(_T("priest")) == 0)
		return E_CARDCLASS_PREIST;
	else
		return E_CARDCLASS_NONE;

	return E_CARDCLASS_NONE;
}

void CCard::Init()
{
	this->strCardID = L"";
	this->ndbfID = -1;
	this->strName = L"";
	this->eCardSet = E_CARDSET_NONE;
	this->eType = E_CARDTYPE_NONE;
	this->eRace = E_CARDRACE_NONE;
	this->eRarity = E_CARDRARITY_FREE;
	this->eClass = E_CARDCLASS_NONE;
	this->nCost = -1;
	this->nHealth = -1;
	this->nAttack = -1;
	this->strText = L"";
	this->strFlavor = L"";
	this->strArtist = L"";
	this->urlImg = L"";
	this->urlImgGold = L"";
	this->vecMechanics.clear();
	m_bDownloadingImg = 0;
}

void CCard::Trace()
{
	TRACE(L"name:%s, cost:%d, atk:%d, hlt:%d, text:%s\n", strName.c_str(), nCost, nAttack, nHealth, strText.c_str());
}

BOOL CCard::ImportCardData(std::wstring name, std::wstring value)
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);

	if (name.compare(_T("cardid")) == 0)
		return ImportCardID(value);
	else if (name.compare(_T("dbfid")) == 0)
		return ImportDbfID(value);
	else if (name.compare(_T("name")) == 0)
		return ImportName(value);
	else if (name.compare(_T("cardset")) == 0)
		return ImportCardSet(value);
	else if (name.compare(_T("type")) == 0)
		return ImportCardType(value);
	else if (name.compare(_T("cost")) == 0)
		return ImportCost(value);
	else if (name.compare(_T("attack")) == 0)
		return ImportAttack(value);
	else if (name.compare(_T("health")) == 0)
		return ImportHealth(value);
	else if (name.compare(_T("artist")) == 0)
		return ImportArtist(value);
	else if (name.compare(_T("playerclass")) == 0)
		return ImportCardClass(value);
	else if (name.compare(_T("img")) == 0)
		return ImportImgURL(value);
	else if (name.compare(_T("imggold")) == 0)
		return ImportImgGoldURL(value);
	else if (name.compare(_T("text")) == 0)
		return ImportText(value);
	else if (name.compare(_T("flavor")) == 0)
		return ImportFlavor(value);
	else if (name.compare(_T("rarity")) == 0)
		return ImportCardRarity(value);
	else if (name.compare(_T("race")) == 0)
		return ImportCardRace(value);
	else
		return FALSE;

	return TRUE;
}

BOOL CCard::ImportCardID(std::wstring value)
{
	this->strCardID.assign(value.begin(), value.end());
	this->urlImg = L"https://art.hearthstonejson.com/v1/render/latest/koKR/256x/" + this->strCardID + L".png";
	//황카는 보류
	//this->urlImgGold;
	return TRUE;
}

BOOL CCard::ImportDbfID(std::wstring value)
{
	this->ndbfID = std::stoi(value);
	return TRUE;
}

BOOL CCard::ImportName(std::wstring value)
{
	this->strName.assign(value.begin(), value.end());
	return TRUE;
}

BOOL CCard::ImportCardSet(std::wstring value)
{
	this->eCardSet = CCard::GetCardSetByStr(value);
	return TRUE;
}

BOOL CCard::ImportCardType(std::wstring value)
{
	this->eType = CCard::GetCardTypeByStr(value);
	return TRUE;
}

BOOL CCard::ImportCardRace(std::wstring value)
{
	this->eRace = CCard::GetCardRaceByStr(value);
	return TRUE;
}

BOOL CCard::ImportCardRarity(std::wstring value)
{
	this->eRarity = CCard::GetCardRarityByStr(value);
	return TRUE;
}

BOOL CCard::ImportCardClass(std::wstring value)
{
	this->eClass = CCard::GetCardClassByStr(value);
	return TRUE;
}

BOOL CCard::ImportCost(std::wstring value)
{
	this->nCost = std::stoi(value);
	return TRUE;
}

BOOL CCard::ImportHealth(std::wstring value)
{
	this->nHealth = std::stoi(value);
	return TRUE;
}

BOOL CCard::ImportAttack(std::wstring value)
{
	this->nAttack = std::stoi(value);
	return TRUE;
}

BOOL CCard::ImportText(std::wstring value)
{
	this->strText.assign(value.begin(), value.end());
	return TRUE;
}

BOOL CCard::ImportFlavor(std::wstring value)
{
	this->strFlavor.assign(value.begin(), value.end());
	return TRUE;
}

BOOL CCard::ImportArtist(std::wstring value)
{
	this->strArtist.assign(value.begin(), value.end());
	return TRUE;
}

BOOL CCard::ImportImgURL(std::wstring value)
{
	//this->urlImg.assign(value.begin(), value.end());
	//DownloadImg();
	return TRUE;
}

BOOL CCard::ImportImgGoldURL(std::wstring value)
{
	//this->urlImgGold.assign(value.begin(), value.end());
	return TRUE;
}

BOOL CCard::ImportMechanics(std::vector<std::string> vecValue)
{
	this->vecMechanics.clear();
	for (std::string strValue : vecValue)
	{
		std::wstring temp;
		temp.assign(strValue.begin(), strValue.end());
		this->vecMechanics.push_back(temp);
	}
	return TRUE;
}

void CCard::DownloadImg(std::string imgPath)
{
	// 황금 카드 정보가 있긴 하지만
	// Gif 관련해서 설정하는건 귀찮으니까 안함
	if (m_bDownloadingImg == TRUE)
		return;

	m_bDownloadingImg = TRUE;

	MCurl* pCurl = MCurl::GetInstance();
	std::string temp;
	temp.assign(urlImg.begin(), urlImg.end());
	CString imgfilePath = pCurl->DownloadImg(imgPath, temp).c_str();

	HRESULT hResult = m_CardImage.Load(imgfilePath);
	
	m_bDownloadingImg = FALSE;

	if (FAILED(hResult)) {
		return;
	}

}

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

void CCardListMgr::DownloadAllImg()
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
		if ( pCard->m_CardImage == NULL )
			pCard->DownloadImg(imgPath);
	}
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

BOOL CCardFilter::IsAgree(CCard* pCard)
{
	BOOL bResult = TRUE;
	if (bUseCost)
		bResult &= nFromCost <= pCard->nCost && pCard->nCost <= nToCost;
	if (bUseAttack)
		bResult &= nFromAttack <= pCard->nAttack && pCard->nAttack <= nToAttack;
	if (bUseHealth)
		bResult &= nFromHealth <= pCard->nHealth && pCard->nHealth <= nToHealth;

	if ( bUseCardSet )
		bResult &= (pCard->eCardSet & nCardSet) > 0;
	if (bUseClass)
		bResult &= (pCard->eClass & nClass) > 0;
	if (bUseRarity)
		bResult &= (pCard->eRarity & nRarity) > 0;
	if (bUseRace)
		bResult &= (pCard->eRace & nRace) > 0;
	if (bUseType)
		bResult &= (pCard->eType & nType) > 0;

	if (bUseText)
	{
		BOOL bTemp = FALSE;
		std::size_t posName = pCard->strName.find(strSearch);
		if (posName != std::string::npos)
			bTemp = TRUE;

		std::size_t posText = pCard->strText.find(strSearch);
		if (posText != std::string::npos)
			bTemp = TRUE;

		bResult &= bTemp;
	}

	return bResult;
}

CCardFilter & CCardFilter::operator=(const CCardFilter &rhs)
{
	if (this == &rhs)      // 동일 객체?
		return *this;

	bUseText = rhs.bUseText;
	bUseCardSet = rhs.bUseCardSet;
	bUseType =		rhs.bUseType;
	bUseRace =		rhs.bUseRace;
	bUseRarity =	rhs.bUseRarity;
	bUseClass =		rhs.bUseClass;
	bUseCost =		rhs.bUseCost;
	bUseAttack =	rhs.bUseAttack;
	bUseHealth =	rhs.bUseHealth;

	strSearch = rhs.strSearch;
	nCardSet = rhs.nCardSet;
	nType = rhs.nType;
	nRace = rhs.nRace;
	nRarity = rhs.nRarity;
	nClass = rhs.nClass;

	bUseCost = rhs.bUseCost;
	nFromCost = rhs.nFromCost;
	nToCost = rhs.nToCost;

	bUseAttack = rhs.bUseAttack;
	nFromAttack = rhs.nFromAttack;
	nToAttack = rhs.nToAttack;

	bUseHealth = rhs.bUseHealth;
	nFromHealth = rhs.nFromHealth;
	nToHealth = rhs.nToHealth;
	return *this;
}
