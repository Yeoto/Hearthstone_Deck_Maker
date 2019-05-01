#include "stdafx.h"
#include "CardData.h"

#include <direct.h>

#include "MCurl.h"

E_CARDSET CCard::GetCardSetByStr(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	if (str.compare("basic") == 0)
		return E_CARDSET_BASIC;
	else if (str.compare("classic") == 0)
		return E_CARDSET_CLASSIC;
	else if (str.compare("hall of Fame") == 0)
		return E_CARDSET_HALL_OF_FAME;
	else if (str.compare("naxxramas") == 0)
		return E_CARDSET_NAXXRAMAS;
	else if (str.compare("goblins vs gnomes") == 0)
		return E_CARDSET_GOBLIN_VS_GNOMES;
	else if (str.compare("blackrock mountain") == 0)
		return E_CARDSET_BLAKCROCK_MOUNTAIN;
	else if (str.compare("the grand tournament") == 0)
		return E_CARDSET_THE_GRAND_TOURNAMENT;
	else if (str.compare("the league of explorers") == 0)
		return E_CARDSET_THE_ELAGUE_OF_EXPLORERS;
	else if (str.compare("whispers of the old gods") == 0)
		return E_CARDSET_WHISPERS_OF_THE_OLD_GODS;
	else if (str.compare("one night in karazhan") == 0)
		return E_CARDSET_ONE_NIGHT_IN_KARAZHAN;
	else if (str.compare("mean streets of gadgetzan") == 0)
		return E_CARDSET_MEAN_STREETS_OF_GADGETZAN;
	else if (str.compare("journey to un'goro"))
		return E_CARDSET_JOURNEY_TO_UNGORO;
	else if (str.compare("knights of the frozen throne") == 0)
		return E_CARDSET_KNIGHTS_OF_THE_FROZEN_THRONE;
	else if (str.compare("kobolds & catacombs"))
		return E_CARDSET_KOBOLDS_AND_CATACOMBS;
	else if (str.compare("the witchwood") == 0)
		return E_CARDSET_THE_WITCHWOOD;
	else if (str.compare("the boomsday project") == 0)
		return E_CARDSET_THE_BOOMSDAY_PROJECT;
	else if (str.compare("rastakhan's rumble"))
		return E_CARDSET_RASTAKHAN_S_RUMBLE;
	else if (str.compare("rise of shadows") == 0)
		return E_CARDSET_RISE_OF_SHADOWS;
	else
		return E_CARDSET_NONE;

	return E_CARDSET_NONE;
}

E_CARDTYPE CCard::GetCardTypeByStr(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	if (str.compare("hero") == 0)
		return E_CARDTYPE_HERO;
	else if (str.compare("minion") == 0)
		return E_CARDTYPE_MINION;
	else if (str.compare("spell") == 0)
		return E_CARDTYPE_SPELL;
	else if (str.compare("enchantment") == 0)
		return E_CARDTYPE_ENCHANTMENT;
	else if (str.compare("weapon") == 0)
		return E_CARDTYPE_WEAPON;
	else if (str.compare("hero power") == 0)
		return E_CARDTYPE_HERO_POWER;
	else
		return E_CARDTYPE_NONE;

	return E_CARDTYPE_NONE;
}

E_CARDRACE CCard::GetCardRaceByStr(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	if (str.compare(""))
		return E_CARDRACE_NONE;
	else if (str.compare("demon") == 0)
		return E_CARDRACE_DEMON;
	else if (str.compare("dragon") == 0)
		return E_CARDRACE_DRAGON;
	else if (str.compare("mech") == 0)
		return E_CARDRACE_MECH;
	else if (str.compare("murloc") == 0)
		return E_CARDRACE_MURLOC;
	else if (str.compare("beast") == 0)
		return E_CARDRACE_BEAST;
	else if (str.compare("pirate") == 0)
		return E_CARDRACE_PIRATE;
	else if (str.compare("totem") == 0)
		return E_CARDRACE_TOTEM;
	else
		return E_CARDRACE_NONE;

	return E_CARDRACE_NONE;
}

E_CARDRARITY CCard::GetCardRarityByStr(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	if (str.compare("free") == 0)
		return E_CARDRARITY_FREE;
	else if (str.compare("common") == 0)
		return E_CARDRARITY_COMMON;
	else if (str.compare("rare") == 0)
		return E_CARDRARITY_RARE;
	else if (str.compare("epic") == 0)
		return E_CARDRARITY_EPIC;
	else if (str.compare("legendary") == 0)
		return E_CARDRARITY_LEGENDARY;
	else
		return E_CARDRARITY_FREE;

	return E_CARDRARITY_FREE;
}

E_CARDCLASS CCard::GetCardClassByStr(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	if (str.compare(""))
		return E_CARDCLASS_NONE;
	else if (str.compare("warrior") == 0)
		return E_CARDCLASS_WARRIOR;
	else if (str.compare("shaman") == 0)
		return E_CARDCLASS_SHAMAN;
	else if (str.compare("thief") == 0)
		return E_CARDCLASS_THIEF;
	else if (str.compare("paladin") == 0)
		return E_CARDCLASS_PALADIN;
	else if (str.compare("hunter") == 0)
		return E_CARDCLASS_HUNTER;
	else if (str.compare("druid") == 0)
		return E_CARDCLASS_DRUID;
	else if (str.compare("warlock") == 0)
		return E_CARDCLASS_WARLOCK;
	else if (str.compare("mage") == 0)
		return E_CARDCLASS_MAGE;
	else if (str.compare("preist") == 0)
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
}

void CCard::Trace()
{
	TRACE(L"name:%s, cost:%d, atk:%d, hlt:%d, text:%s\n", strName.c_str(), nCost, nAttack, nHealth, strText.c_str());
}

BOOL CCard::ImportCardData(std::string name, std::string value)
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);

	if (name.compare("cardid") == 0)
		return ImportCardID(value);
	else if (name.compare("dbfid") == 0)
		return ImportDbfID(value);
	else if (name.compare("name") == 0)
		return ImportName(value);
	else if (name.compare("cardset") == 0)
		return ImportCardSet(value);
	else if (name.compare("type") == 0)
		return ImportCardType(value);
	else if (name.compare("cost") == 0)
		return ImportCost(value);
	else if (name.compare("attack") == 0)
		return ImportAttack(value);
	else if (name.compare("health") == 0)
		return ImportHealth(value);
	else if (name.compare("artist") == 0)
		return ImportArtist(value);
	else if (name.compare("playerclass") == 0)
		return ImportCardClass(value);
	else if (name.compare("img") == 0)
		return ImportImgURL(value);
	else if (name.compare("imggold") == 0)
		return ImportImgGoldURL(value);
	else if (name.compare("text") == 0)
		return ImportText(value);
	else if (name.compare("flavor") == 0)
		return ImportFlavor(value);
	else if (name.compare("rarity") == 0)
		return ImportCardRarity(value);
	else if (name.compare("race") == 0)
		return ImportCardRace(value);
	else
		return FALSE;

	return TRUE;
}

BOOL CCard::ImportCardID(std::string value)
{
	this->strCardID.assign(value.begin(), value.end());
	this->urlImg = L"https://art.hearthstonejson.com/v1/render/latest/koKR/256x/" + this->strCardID + L".png";
	//황카는 보류
	//this->urlImgGold;
	return TRUE;
}

BOOL CCard::ImportDbfID(std::string value)
{
	this->ndbfID = std::stoi(value);
	return TRUE;
}

BOOL CCard::ImportName(std::string value)
{
	this->strName.assign(value.begin(), value.end());
	return TRUE;
}

BOOL CCard::ImportCardSet(std::string value)
{
	this->eCardSet = CCard::GetCardSetByStr(value);
	return TRUE;
}

BOOL CCard::ImportCardType(std::string value)
{
	this->eType = CCard::GetCardTypeByStr(value);
	return TRUE;
}

BOOL CCard::ImportCardRace(std::string value)
{
	this->eRace = CCard::GetCardRaceByStr(value);
	return TRUE;
}

BOOL CCard::ImportCardRarity(std::string value)
{
	this->eRarity = CCard::GetCardRarityByStr(value);
	return TRUE;
}

BOOL CCard::ImportCardClass(std::string value)
{
	this->eClass = CCard::GetCardClassByStr(value);
	return TRUE;
}

BOOL CCard::ImportCost(std::string value)
{
	this->nCost = std::stoi(value);
	return TRUE;
}

BOOL CCard::ImportHealth(std::string value)
{
	this->nHealth = std::stoi(value);
	return TRUE;
}

BOOL CCard::ImportAttack(std::string value)
{
	this->nAttack = std::stoi(value);
	return TRUE;
}

BOOL CCard::ImportText(std::string value)
{
	this->strText.assign(value.begin(), value.end());
	return TRUE;
}

BOOL CCard::ImportFlavor(std::string value)
{
	this->strFlavor.assign(value.begin(), value.end());
	return TRUE;
}

BOOL CCard::ImportArtist(std::string value)
{
	this->strArtist.assign(value.begin(), value.end());
	return TRUE;
}

BOOL CCard::ImportImgURL(std::string value)
{
	//this->urlImg.assign(value.begin(), value.end());
	//DownloadImg();
	return TRUE;
}

BOOL CCard::ImportImgGoldURL(std::string value)
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
	MCurl* pCurl = MCurl::GetInstance();
	std::string temp;
	temp.assign(urlImg.begin(), urlImg.end());
	imgfilePath = pCurl->DownloadImg(imgPath, temp);
}

CCardListMgr* CCardListMgr::m_pInstance;

CCardListMgr::~CCardListMgr()
{
	for (CCard* pCard : vecCardList)
	{
		if (pCard != NULL)
		{
			delete pCard;
			pCard = NULL;
		}
	}
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

void CCardListMgr::AddCard(CCard * pCard)
{
	vecCardList.push_back(pCard);
}

void CCardListMgr::TraceAll()
{
	for (CCard* pCard : vecCardList)
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

	for (CCard* pCard : vecCardList)
		pCard->DownloadImg(imgPath);
}
