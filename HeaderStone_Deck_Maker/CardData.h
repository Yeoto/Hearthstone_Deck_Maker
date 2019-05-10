#pragma once

#include <map>
#include <vector>
#include <string>
#include <algorithm>

enum E_CARDCLASS
{
	E_CARDCLASS_NONE	= 0x001,	// �߸�ī��
	E_CARDCLASS_WARRIOR = 0x002,	// ����ī��
	E_CARDCLASS_SHAMAN	= 0x004,	// �ּ���ī��
	E_CARDCLASS_ROGUE	= 0x008,	// ����ī��
	E_CARDCLASS_PALADIN = 0x010,	// �����ī��
	E_CARDCLASS_HUNTER	= 0x020,	// ��ɲ�ī��
	E_CARDCLASS_DRUID	= 0x040,	// ����̵�ī��
	E_CARDCLASS_WARLOCK = 0x080,	// �渶����ī��
	E_CARDCLASS_MAGE	= 0x100,	// ������ī��
	E_CARDCLASS_PREIST	= 0x200,	// ����ī��
	E_CARDCLASS_ALL		= 0x3FF
};
enum E_CARDSET
{
	E_CARDSET_NONE							= 0x00000,	// ������ �ȵ�
	E_CARDSET_BASIC							= 0x00001,	// �⺻ī��
	E_CARDSET_CLASSIC						= 0x00002,	// ��������ī��
	E_CARDSET_HALL_OF_FAME					= 0x00004,	// ���� ����
	E_CARDSET_NAXXRAMAS						= 0x00008,	// �����󸶽�
	E_CARDSET_GOBLIN_VS_GNOMES				= 0x00010,	// ��� �� ���
	E_CARDSET_BLACKCROCK_MOUNTAIN			= 0x00020,	// ���� ���� ��
	E_CARDSET_THE_GRAND_TOURNAMENT			= 0x00040,	// �� �������
	E_CARDSET_THE_ELAGUE_OF_EXPLORERS		= 0x00080,	// Ž�谡 ����
	E_CARDSET_WHISPERS_OF_THE_OLD_GODS		= 0x00100,	// ������ �ӻ���
	E_CARDSET_ONE_NIGHT_IN_KARAZHAN			= 0x00200,	// �� ���� ���� ī����
	E_CARDSET_MEAN_STREETS_OF_GADGETZAN		= 0x00400,	// ���� �Ÿ��� ������
	E_CARDSET_JOURNEY_TO_UNGORO				= 0x00800,	// ���θ� ���� ����
	E_CARDSET_KNIGHTS_OF_THE_FROZEN_THRONE	= 0x01000,	// ������ ������ ���� 
	E_CARDSET_KOBOLDS_AND_CATACOMBS			= 0x02000,	// �ں�Ʈ�� ���Ϲ̱�
	E_CARDSET_THE_WITCHWOOD					= 0x04000,	// ���ལ
	E_CARDSET_THE_BOOMSDAY_PROJECT			= 0x08000,	// �ڻ� ���� ���ɸ��� ������Ʈ
	E_CARDSET_RASTAKHAN_S_RUMBLE			= 0x10000,	// ��Ÿĭ�� �볭��
	E_CARDSET_RISE_OF_SHADOWS				= 0x20000,	// ����� �ݰ�
	E_CARDSET_REGULAR						= 0x3C003,
	E_CARDSET_ALL							= 0x3FFFF
};

enum E_CARDTYPE
{
	E_CARDTYPE_NONE			= 0x00,	// ������ �ȵ�
	E_CARDTYPE_HERO			= 0x01,	// ����
	E_CARDTYPE_MINION		= 0x02,	// �ϼ���
	E_CARDTYPE_SPELL		= 0x04,	// �ֹ�
	E_CARDTYPE_ENCHANTMENT	= 0x08,	// �ϼ���
	E_CARDTYPE_WEAPON		= 0x10,	// �ϼ���
	E_CARDTYPE_HERO_POWER	= 0x20,	// 
	E_CARDTYPE_ALL			= 0x3F
};

enum E_CARDRARITY
{
	E_CARDRARITY_FREE		= 0x0,	// ����ī��
	E_CARDRARITY_COMMON		= 0x1,	// �Ϲ�ī��
	E_CARDRARITY_RARE		= 0x2,	// ���ī��
	E_CARDRARITY_EPIC		= 0x4,	// Ư��ī��
	E_CARDRARITY_LEGENDARY	= 0x8,	// ����ī��
	E_CARDRARITY_ALL		= 0xF
};

enum E_CARDRACE
{
	E_CARDRACE_NONE			= 0x001,	// ������
	E_CARDRACE_DEMON		= 0x002,	// �Ǹ�
	E_CARDRACE_DRAGON		= 0x004,	// ��
	E_CARDRACE_MECH			= 0x008,	// ���
	E_CARDRACE_MURLOC		= 0x010,	// �ַ�
	E_CARDRACE_BEAST		= 0x020,	// �߼�
	E_CARDRACE_PIRATE		= 0x040,	// ����
	E_CARDRACE_TOTEM		= 0x080,	// ����
	E_CARDRACE_ELEMENTAL	= 0x100,	// ����
	E_CARDRACE_ALL			= 0x1FF
};

class CProgressDlg;
class CCardEncoder;

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

	//����. ȥ�� List�� ����
	BOOL ImportMechanics(std::vector<std::string> vecValue);

	void DownloadImg(std::string filePath);
public:
	// String to Enum Data
	static E_CARDSET	GetCardSetByStr(std::wstring str);
	static E_CARDTYPE	GetCardTypeByStr(std::wstring str);
	static E_CARDRACE	GetCardRaceByStr(std::wstring str);
	static E_CARDRARITY GetCardRarityByStr(std::wstring str);
	static E_CARDCLASS	GetCardClassByStr(std::wstring str);
};

class CCardFilter
{
public:
	BOOL bUseText;
	std::wstring strSearch;

	BOOL bUseCardSet;
	int nCardSet;

	BOOL bUseType;
	int nType;

	BOOL bUseRace;
	int nRace;

	BOOL bUseRarity;
	int nRarity;

	BOOL bUseClass;
	int nClass;

	BOOL bUseCost;
	int nFromCost, nToCost;

	BOOL bUseAttack;
	int nFromAttack, nToAttack;

	BOOL bUseHealth;
	int nFromHealth, nToHealth;

	CCardFilter()
	{
		bUseText = FALSE;
		strSearch = _T("");
			
		bUseCardSet = TRUE;
		nCardSet = E_CARDSET_REGULAR;

		bUseType = FALSE;
		nType = 0;

		bUseRace = FALSE;
		nRace = 0;

		bUseRarity = FALSE;
		nRarity = E_CARDRARITY_FREE;

		bUseClass = TRUE;
		nClass = E_CARDCLASS_DRUID;

		bUseCost = FALSE;
		nFromCost = 0;
		nToCost = 0;

		bUseAttack = FALSE;
		nFromAttack = 0;
		nToAttack = 0;

		bUseHealth = FALSE;
		nFromHealth = 0;
		nToHealth = 0;
	};

public:
	BOOL IsAgree(CCard* pCard);
	CCardFilter & operator=(const CCardFilter &rhs);
};

class CCardListMgr
{
	CCardListMgr();
	~CCardListMgr();
public:
	static CCardListMgr* GetInstance();
	const std::vector<CCard*>& GetCardList() { return m_vecCardList; }
	const std::vector<CCard*>& GetHeroSkinList() { return m_vecHeroSkinList; }
	std::vector<CCard*> GetFilteredList();
	std::vector<CCard*> GetFilteredList(std::vector<CCard*> vecCardList);
	int GetCardListCnt() { return m_vecCardList.size(); }
	CCard* GetTempCard() { return m_pTempCard; }
	CCardFilter* GetCardFilter() { return &m_Filter;  }

	BOOL CheckCardData(CCard* pCard);
	void AddCard(CCard* pCard);
	void MakeTempCard();
	void TraceAll();
	void DownloadAllImg(BOOL* bBreak);

	void DeckCode2CardList(std::string deckCode, E_CARDCLASS& eDeckClass, std::map<CCard*, int>& m_mapCards);
	void CardList2DeckCode(E_CARDCLASS eDeckClass, std::map<CCard*, int> mapDeckList, CString& deckCode);
private:
	static CCardListMgr* m_pInstance;
	static void destroy() { delete m_pInstance; }
	std::vector<CCard*> m_vecHeroSkinList;
	std::vector<CCard*> m_vecCardList;
	CCard* m_pTempCard;
	CCardFilter m_Filter;
};