#pragma once

#include <string>
#include <vector>

#include "CardEnum.h"

class CCard
{
public:
	std::wstring strCardID;
	int ndbfID;
	std::wstring strName;
	E_CARDSET eCardSet;
	E_CARDTYPE eType;
	E_CARDRACE eRace;
	E_CARDRARITY eRarity;
	E_CARDCLASS eClass;
	int nCost;
	int nHealth;
	int nAttack;
	std::wstring strText;
	std::wstring strFlavor;
	std::wstring strArtist;
	std::wstring urlImg;
	std::wstring urlImgGold;
	//std::string imgfilePath;
	std::vector<std::wstring> vecMechanics;
	CImage m_CardImage;

	BOOL m_bDownloadingImg;
	CCard() { Init(); }
	void Init();
	void Trace();
public:
	BOOL ImportCardData(std::wstring name, std::wstring value);
	BOOL ImportCardID(std::wstring value);
	BOOL ImportDbfID(std::wstring value);
	BOOL ImportName(std::wstring value);
	BOOL ImportCardSet(std::wstring value);
	BOOL ImportCardType(std::wstring value);
	BOOL ImportCardRace(std::wstring value);
	BOOL ImportCardRarity(std::wstring value);
	BOOL ImportCardClass(std::wstring value);
	BOOL ImportCost(std::wstring value);
	BOOL ImportHealth(std::wstring value);
	BOOL ImportAttack(std::wstring value);
	BOOL ImportText(std::wstring value);
	BOOL ImportFlavor(std::wstring value);
	BOOL ImportArtist(std::wstring value);
	BOOL ImportImgURL(std::wstring value);
	BOOL ImportImgGoldURL(std::wstring value);

	//¿¹¿Ü. È¥ÀÚ List·Î ³ª¿È
	BOOL ImportMechanics(std::vector<std::string> vecValue);

	void DownloadImg(std::wstring filePath);
public:
	// String to Enum Data
	static E_CARDSET	GetCardSetByStr(std::wstring str);
	static E_CARDTYPE	GetCardTypeByStr(std::wstring str);
	static E_CARDRACE	GetCardRaceByStr(std::wstring str);
	static E_CARDRARITY GetCardRarityByStr(std::wstring str);
	static E_CARDCLASS	GetCardClassByStr(std::wstring str);
};