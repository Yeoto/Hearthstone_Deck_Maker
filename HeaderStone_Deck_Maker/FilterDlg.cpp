// FilterDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "HeaderStone_Deck_Maker.h"
#include "FilterDlg.h"
#include "afxdialogex.h"

// CFilterDlg 대화 상자

IMPLEMENT_DYNAMIC(CFilterDlg, CDialogEx)

CFilterDlg::CFilterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILTER_DLG, pParent), CCardNotifier(NTM_FILTERDLG)
{

}

CFilterDlg::CFilterDlg(CCardFilter* pFilter, CWnd* pParent /*= nullptr*/) :
	m_pFilter(pFilter), CDialogEx(IDD_FILTER_DLG, pParent), CCardNotifier(NTM_FILTERDLG)
{

}

CFilterDlg::~CFilterDlg()
{
}

void CFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//CButton m_ctrlCardType[4];		//Card Type : minions, spell, weapon, hero
	//CButton m_ctrlCardSet_Regular[5];	//Card Set : 오리, 마녀숲 , 폭심, 라스타칸, 어둠의반격
	//CButton m_ctrlCardSet_Wild[12];	//Card Set :	명전, 낙스, 고노 검바
	//									//				마상, 탐험가, 고대신, 카라잔
	//									//				가젯잔, 운고로, 얼왕기, 코볼트
	//CButton m_ctrlCardRarity[5];		//Card Rarity : 무료, 일반, 희귀, 특급, 전설
	//CButton m_ctrlCardClass[10];		//Card Class :	노루, 앰오, 법사, 좆사, 사제
	//									//				돚거, 씹랄, 흑마, 치킨, 중립
	//CButton m_ctrlCardRace[8];		//Card Race :	멀록, 악마, 야수, 용족
	//									//				토템, 해적, 기계, 정령
	//CEdit m_ctrlCardCost[2];			// From, To
	//CEdit m_ctrlCardAttack[2];		// From, To
	//CEdit m_ctrlCardHealth[2];		// From, To
	//
	//CEdit m_ctrlText;
	DDX_Control(pDX, IDC_CARDTYPE_MINION_CHK, m_ctrlCardType[0]);
	DDX_Control(pDX, IDC_CARDTYPE_SPELL_CHK, m_ctrlCardType[1]);
	DDX_Control(pDX, IDC_CARDTYPE_WEAPON_CHK, m_ctrlCardType[2]);
	DDX_Control(pDX, IDC_CARDTYPE_HERO_CHK, m_ctrlCardType[3]);

	DDX_Control(pDX, IDC_CARDSET_ORIGINAL_CHK, m_ctrlCardSet_Regular[0]);
	DDX_Control(pDX, IDC_CARDSET_WITCHWOOD_CHK, m_ctrlCardSet_Regular[1]);
	DDX_Control(pDX, IDC_CARDSET_BOOM_S_DAY_CHK, m_ctrlCardSet_Regular[2]);
	DDX_Control(pDX, IDC_CARDSET_RUMBLE_CHK, m_ctrlCardSet_Regular[3]);
	DDX_Control(pDX, IDC_CARDSET_RISE_OF_SHADOW_CHK, m_ctrlCardSet_Regular[4]);

	DDX_Control(pDX, IDC_CARDSET_HALL_OF_FAME_CHK, m_ctrlCardSet_Wild[0]);
	DDX_Control(pDX, IDC_CARDSET_NAXX_CHK, m_ctrlCardSet_Wild[1]);
	DDX_Control(pDX, IDC_CARDSET_GOBLIN_GNOMES_CHK, m_ctrlCardSet_Wild[2]);
	DDX_Control(pDX, IDC_CARDSET_BLACKROCK_CHK, m_ctrlCardSet_Wild[3]);
	DDX_Control(pDX, IDC_CARDSET_TOURNAMENT_CHK, m_ctrlCardSet_Wild[4]);
	DDX_Control(pDX, IDC_CARDSET_EXPLORERS_CHK, m_ctrlCardSet_Wild[5]);
	DDX_Control(pDX, IDC_CARDSET_OLDGOD_CHK, m_ctrlCardSet_Wild[6]);
	DDX_Control(pDX, IDC_CARDSET_KARAZHAN_CHK, m_ctrlCardSet_Wild[7]);
	DDX_Control(pDX, IDC_CARDSET_GADGETZAN_CHK, m_ctrlCardSet_Wild[8]);
	DDX_Control(pDX, IDC_CARDSET_UNGORO_CHK, m_ctrlCardSet_Wild[9]);
	DDX_Control(pDX, IDC_CARDSET_FROZEN_THRONE_CHK, m_ctrlCardSet_Wild[10]);
	DDX_Control(pDX, IDC_CARDSET_COBOLTS_CHK, m_ctrlCardSet_Wild[11]);

	DDX_Control(pDX, IDC_CARDRARIRY_FREE_CHK, m_ctrlCardRarity[0]);
	DDX_Control(pDX, IDC_CARDRARIRY_COMMON_CHK, m_ctrlCardRarity[1]);
	DDX_Control(pDX, IDC_CARDRARIRY_RARE_CHK, m_ctrlCardRarity[2]);
	DDX_Control(pDX, IDC_CARDRARIRY_EPIC_CHK, m_ctrlCardRarity[3]);
	DDX_Control(pDX, IDC_CARDRARIRY_LEGENDARY_CHK, m_ctrlCardRarity[4]);

	DDX_Control(pDX, IDC_CARDCLASS_DRUID_CHK, m_ctrlCardClass[0]);
	DDX_Control(pDX, IDC_CARDCLASS_HUNTER_CHK, m_ctrlCardClass[1]);
	DDX_Control(pDX, IDC_CARDCLASS_MAGE_CHK, m_ctrlCardClass[2]);
	DDX_Control(pDX, IDC_CARDCLASS_PALADIN_CHK, m_ctrlCardClass[3]);
	DDX_Control(pDX, IDC_CARDCLASS_PRIEST_CHK, m_ctrlCardClass[4]);
	DDX_Control(pDX, IDC_CARDCLASS_ROGUE_CHK, m_ctrlCardClass[5]);
	DDX_Control(pDX, IDC_CARDCLASS_SHAMAN_CHK, m_ctrlCardClass[6]);
	DDX_Control(pDX, IDC_CARDCLASS_WARLOCK_CHK, m_ctrlCardClass[7]);
	DDX_Control(pDX, IDC_CARDCLASS_WARRIOR_CHK, m_ctrlCardClass[8]);
	DDX_Control(pDX, IDC_CARDCLASS_NONE_CHK, m_ctrlCardClass[9]);

	DDX_Control(pDX, IDC_CARDRACE_MURLOC_CHK, m_ctrlCardRace[0]);
	DDX_Control(pDX, IDC_CARDRACE_DEMON_CHK, m_ctrlCardRace[1]);
	DDX_Control(pDX, IDC_CARDRACE_BEAST_CHK, m_ctrlCardRace[2]);
	DDX_Control(pDX, IDC_CARDRACE_DRAGON_CHK, m_ctrlCardRace[3]);
	DDX_Control(pDX, IDC_CARDRACE_TOTEM_CHK, m_ctrlCardRace[4]);
	DDX_Control(pDX, IDC_CARDRACE_PIRATE_CHK, m_ctrlCardRace[5]);
	DDX_Control(pDX, IDC_CARDRACE_MECH_CHK, m_ctrlCardRace[6]);
	DDX_Control(pDX, IDC_CARDRACE_ELEMENTAL_CHK, m_ctrlCardRace[7]);

	DDX_Control(pDX, IDC_CARDSTAT_COST_FROM_EDT, m_ctrlCardCost[0]);
	DDX_Control(pDX, IDC_CARDSTAT_COST_TO_EDT, m_ctrlCardCost[1]);
	
	DDX_Control(pDX, IDC_CARDSTAT_ATK_FROM_EDT, m_ctrlCardAttack[0]);
	DDX_Control(pDX, IDC_CARDSTAT_ATK_TO_EDT, m_ctrlCardAttack[1]);

	DDX_Control(pDX, IDC_CARDSTAT_HLT_FROM_EDT, m_ctrlCardHealth[0]);
	DDX_Control(pDX, IDC_CARDSTAT_HLT_TO_EDT, m_ctrlCardHealth[1]);

	DDX_Control(pDX, IDC_CARDSTAT_TEXT_EDT, m_ctrlText);

	DDX_Control(pDX, IDC_BOOKMARK_CHK, m_ctrlBookmarkChk);
	DDX_Control(pDX, IDC_CARDTYPE_CHK, m_ctrlCardTypeChk);
	DDX_Control(pDX, IDC_CARDSET_CHK, m_ctrlCardSetChk);
	DDX_Control(pDX, IDC_CARDRARITY_CHK, m_ctrlCardRarityChk);
	DDX_Control(pDX, IDC_CARDCLASS_CHK, m_ctrlCardClassChk);
	DDX_Control(pDX, IDC_CARDRACE_CHK, m_ctrlCardRaceChk);
	DDX_Control(pDX, IDC_CARDCOST_CHK, m_ctrlCardCostChk);
	DDX_Control(pDX, IDC_CARDATTACK_CHK, m_ctrlCardAttackChk);
	DDX_Control(pDX, IDC_CARDHEALTH_CHK, m_ctrlCardHealthChk);
	DDX_Control(pDX, IDC_CARDTEXT_CHK, m_ctrlCardTextChk);
}


BOOL CFilterDlg::ExecuteNotify(NOTIFYMSG eSender, WPARAM wParam, LPARAM lParam)
{
	switch (eSender)
	{
	case NTM_MAINDLG:
		m_ModFilter = *m_pFilter;
		Data2Dlg();
		break;
	default:
		break;
	}
	return TRUE;
}

BEGIN_MESSAGE_MAP(CFilterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RESET_BTN, OnClickReset)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_CARDTYPE_CHK, OnClickCardTypeChk)
	ON_BN_CLICKED(IDC_CARDSET_CHK, OnClickCardSetChk)
	ON_BN_CLICKED(IDC_CARDRARITY_CHK, OnClickCardRarityChk)
	ON_BN_CLICKED(IDC_CARDCLASS_CHK, OnClickCardClassChk)
	ON_BN_CLICKED(IDC_CARDRACE_CHK, OnClickCardRaceChk)
	ON_BN_CLICKED(IDC_CARDCOST_CHK, OnClickCardCostChk)
	ON_BN_CLICKED(IDC_CARDATTACK_CHK, OnClickCardAttackChk)
	ON_BN_CLICKED(IDC_CARDHEALTH_CHK, OnClickCardHealthChk)
	ON_BN_CLICKED(IDC_CARDTEXT_CHK, OnClickCardTextChk)
END_MESSAGE_MAP()


// CFilterDlg 메시지 처리기


void CFilterDlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CDialogEx::PostNcDestroy();
	//delete this;
}

void CFilterDlg::OnClickReset()
{
	m_ModFilter.init();
	Data2Dlg();
}

void CFilterDlg::Dlg2Data()
{
	if (GetSafeHwnd() == NULL)
		return;

	int nType = 0;
	nType |= m_ctrlCardType[0].GetCheck() == BST_CHECKED ? E_CARDTYPE_MINION : 0;
	nType |= m_ctrlCardType[1].GetCheck() == BST_CHECKED ? E_CARDTYPE_SPELL : 0;
	nType |= m_ctrlCardType[2].GetCheck() == BST_CHECKED ? E_CARDTYPE_WEAPON : 0;
	nType |= m_ctrlCardType[3].GetCheck() == BST_CHECKED ? E_CARDTYPE_HERO : 0;
	m_ModFilter.nType = nType;

	int nCardSet = 0;
	nCardSet |= m_ctrlCardSet_Regular[0].GetCheck() == BST_CHECKED ? (E_CARDSET_BASIC | E_CARDSET_CLASSIC) : 0;
	nCardSet |= m_ctrlCardSet_Regular[1].GetCheck() == BST_CHECKED ? E_CARDSET_THE_WITCHWOOD : 0;
	nCardSet |= m_ctrlCardSet_Regular[2].GetCheck() == BST_CHECKED ? E_CARDSET_THE_BOOMSDAY_PROJECT : 0;
	nCardSet |= m_ctrlCardSet_Regular[3].GetCheck() == BST_CHECKED ? E_CARDSET_RASTAKHAN_S_RUMBLE : 0;
	nCardSet |= m_ctrlCardSet_Regular[4].GetCheck() == BST_CHECKED ? E_CARDSET_RISE_OF_SHADOWS : 0;
	nCardSet |= m_ctrlCardSet_Wild[0].GetCheck() == BST_CHECKED ? E_CARDSET_HALL_OF_FAME : 0;
	nCardSet |= m_ctrlCardSet_Wild[1].GetCheck() == BST_CHECKED ? E_CARDSET_NAXXRAMAS : 0;
	nCardSet |= m_ctrlCardSet_Wild[2].GetCheck() == BST_CHECKED ? E_CARDSET_GOBLIN_VS_GNOMES : 0;
	nCardSet |= m_ctrlCardSet_Wild[3].GetCheck() == BST_CHECKED ? E_CARDSET_BLACKCROCK_MOUNTAIN : 0;
	nCardSet |= m_ctrlCardSet_Wild[4].GetCheck() == BST_CHECKED ? E_CARDSET_THE_GRAND_TOURNAMENT : 0;
	nCardSet |= m_ctrlCardSet_Wild[5].GetCheck() == BST_CHECKED ? E_CARDSET_THE_ELAGUE_OF_EXPLORERS : 0;
	nCardSet |= m_ctrlCardSet_Wild[6].GetCheck() == BST_CHECKED ? E_CARDSET_WHISPERS_OF_THE_OLD_GODS : 0;
	nCardSet |= m_ctrlCardSet_Wild[7].GetCheck() == BST_CHECKED ? E_CARDSET_ONE_NIGHT_IN_KARAZHAN : 0;
	nCardSet |= m_ctrlCardSet_Wild[8].GetCheck() == BST_CHECKED ? E_CARDSET_MEAN_STREETS_OF_GADGETZAN : 0;
	nCardSet |= m_ctrlCardSet_Wild[9].GetCheck() == BST_CHECKED ? E_CARDSET_JOURNEY_TO_UNGORO : 0;
	nCardSet |= m_ctrlCardSet_Wild[10].GetCheck() == BST_CHECKED ? E_CARDSET_KNIGHTS_OF_THE_FROZEN_THRONE : 0;
	nCardSet |= m_ctrlCardSet_Wild[11].GetCheck() == BST_CHECKED ? E_CARDSET_KOBOLDS_AND_CATACOMBS : 0;
	m_ModFilter.nCardSet = nCardSet;

	int nRarity = 0;
	nRarity |= m_ctrlCardRarity[0].GetCheck() == BST_CHECKED ? E_CARDRARITY_FREE : 0;
	nRarity |= m_ctrlCardRarity[1].GetCheck() == BST_CHECKED ? E_CARDRARITY_COMMON : 0;
	nRarity |= m_ctrlCardRarity[2].GetCheck() == BST_CHECKED ? E_CARDRARITY_RARE : 0;
	nRarity |= m_ctrlCardRarity[3].GetCheck() == BST_CHECKED ? E_CARDRARITY_EPIC : 0;
	nRarity |= m_ctrlCardRarity[4].GetCheck() == BST_CHECKED ? E_CARDRARITY_LEGENDARY : 0;
	m_ModFilter.nRarity = nRarity;

	int nClass = 0;
	nClass |= m_ctrlCardClass[0].GetCheck() == BST_CHECKED ? E_CARDCLASS_DRUID : 0;
	nClass |= m_ctrlCardClass[1].GetCheck() == BST_CHECKED ? E_CARDCLASS_HUNTER : 0;
	nClass |= m_ctrlCardClass[2].GetCheck() == BST_CHECKED ? E_CARDCLASS_MAGE : 0;
	nClass |= m_ctrlCardClass[3].GetCheck() == BST_CHECKED ? E_CARDCLASS_PALADIN : 0;
	nClass |= m_ctrlCardClass[4].GetCheck() == BST_CHECKED ? E_CARDCLASS_PREIST : 0;
	nClass |= m_ctrlCardClass[5].GetCheck() == BST_CHECKED ? E_CARDCLASS_ROGUE : 0;
	nClass |= m_ctrlCardClass[6].GetCheck() == BST_CHECKED ? E_CARDCLASS_SHAMAN : 0;
	nClass |= m_ctrlCardClass[7].GetCheck() == BST_CHECKED ? E_CARDCLASS_WARLOCK : 0;
	nClass |= m_ctrlCardClass[8].GetCheck() == BST_CHECKED ? E_CARDCLASS_WARRIOR : 0;
	nClass |= m_ctrlCardClass[9].GetCheck() == BST_CHECKED ? E_CARDCLASS_NONE : 0;
	m_ModFilter.nClass = nClass;

	int nRace = 0;
	nRace |= m_ctrlCardRace[0].GetCheck() == BST_CHECKED ? E_CARDRACE_MURLOC : 0;
	nRace |= m_ctrlCardRace[1].GetCheck() == BST_CHECKED ? E_CARDRACE_DEMON : 0;
	nRace |= m_ctrlCardRace[2].GetCheck() == BST_CHECKED ? E_CARDRACE_BEAST : 0;
	nRace |= m_ctrlCardRace[3].GetCheck() == BST_CHECKED ? E_CARDRACE_DRAGON : 0;
	nRace |= m_ctrlCardRace[4].GetCheck() == BST_CHECKED ? E_CARDRACE_TOTEM : 0;
	nRace |= m_ctrlCardRace[5].GetCheck() == BST_CHECKED ? E_CARDRACE_PIRATE : 0;
	nRace |= m_ctrlCardRace[6].GetCheck() == BST_CHECKED ? E_CARDRACE_MECH : 0;
	nRace |= m_ctrlCardRace[7].GetCheck() == BST_CHECKED ? E_CARDRACE_ELEMENTAL : 0;
	m_ModFilter.nRace = nRace;

	CString strTemp;
	m_ctrlCardCost[0].GetWindowText(strTemp); m_ModFilter.nFromCost = _ttoi(strTemp);
	m_ctrlCardCost[1].GetWindowText(strTemp); m_ModFilter.nToCost = _ttoi(strTemp);

	m_ctrlCardAttack[0].GetWindowText(strTemp); m_ModFilter.nFromAttack = _ttoi(strTemp);
	m_ctrlCardAttack[1].GetWindowText(strTemp); m_ModFilter.nToAttack = _ttoi(strTemp);

	m_ctrlCardHealth[0].GetWindowText(strTemp); m_ModFilter.nFromHealth = _ttoi(strTemp);
	m_ctrlCardHealth[1].GetWindowText(strTemp); m_ModFilter.nToHealth = _ttoi(strTemp);

	m_ctrlText.GetWindowText(strTemp);
	m_ModFilter.strSearch = strTemp;

	m_ModFilter.bBookmark = m_ctrlBookmarkChk.GetCheck() == TRUE ? BST_CHECKED : BST_UNCHECKED;
	m_ModFilter.bUseType = m_ctrlCardTypeChk.GetCheck() == TRUE ? BST_CHECKED : BST_UNCHECKED;
	m_ModFilter.bUseCardSet	= m_ctrlCardSetChk.GetCheck( ) == TRUE ? BST_CHECKED : BST_UNCHECKED;
	m_ModFilter.bUseRarity	= m_ctrlCardRarityChk.GetCheck( ) == TRUE ? BST_CHECKED : BST_UNCHECKED;
	m_ModFilter.bUseClass	= m_ctrlCardClassChk.GetCheck( ) == TRUE ? BST_CHECKED : BST_UNCHECKED;
	m_ModFilter.bUseRace	= m_ctrlCardRaceChk.GetCheck( ) == TRUE ? BST_CHECKED : BST_UNCHECKED;
	m_ModFilter.bUseCost	= m_ctrlCardCostChk.GetCheck( ) == TRUE ? BST_CHECKED : BST_UNCHECKED;
	m_ModFilter.bUseAttack	= m_ctrlCardAttackChk.GetCheck( ) == TRUE ? BST_CHECKED : BST_UNCHECKED;
	m_ModFilter.bUseHealth	= m_ctrlCardHealthChk.GetCheck( ) == TRUE ? BST_CHECKED : BST_UNCHECKED;
	m_ModFilter.bUseText	= m_ctrlCardTextChk.GetCheck( ) == TRUE ? BST_CHECKED : BST_UNCHECKED;

	*m_pFilter = m_ModFilter;
	SendNotify(NULL, NULL);
}

void CFilterDlg::Data2Dlg()
{
	if (GetSafeHwnd() == NULL)
		return;

	m_ctrlCardType[0].SetCheck((m_ModFilter.nType & E_CARDTYPE_MINION) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardType[1].SetCheck((m_ModFilter.nType & E_CARDTYPE_SPELL) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardType[2].SetCheck((m_ModFilter.nType & E_CARDTYPE_WEAPON) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardType[3].SetCheck((m_ModFilter.nType & E_CARDTYPE_HERO) > 0 ? BST_CHECKED : BST_UNCHECKED);

	m_ctrlCardSet_Regular[0].SetCheck((m_ModFilter.nCardSet & (E_CARDSET_BASIC | E_CARDSET_CLASSIC)) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardSet_Regular[1].SetCheck((m_ModFilter.nCardSet & E_CARDSET_THE_WITCHWOOD) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardSet_Regular[2].SetCheck((m_ModFilter.nCardSet & E_CARDSET_THE_BOOMSDAY_PROJECT) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardSet_Regular[3].SetCheck((m_ModFilter.nCardSet & E_CARDSET_RASTAKHAN_S_RUMBLE) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardSet_Regular[4].SetCheck((m_ModFilter.nCardSet & E_CARDSET_RISE_OF_SHADOWS) > 0 ? BST_CHECKED : BST_UNCHECKED);

	m_ctrlCardSet_Wild[0].SetCheck((m_ModFilter.nCardSet & E_CARDSET_HALL_OF_FAME) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardSet_Wild[1].SetCheck((m_ModFilter.nCardSet & E_CARDSET_NAXXRAMAS) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardSet_Wild[2].SetCheck((m_ModFilter.nCardSet & E_CARDSET_GOBLIN_VS_GNOMES) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardSet_Wild[3].SetCheck((m_ModFilter.nCardSet & E_CARDSET_BLACKCROCK_MOUNTAIN) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardSet_Wild[4].SetCheck((m_ModFilter.nCardSet & E_CARDSET_THE_GRAND_TOURNAMENT) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardSet_Wild[5].SetCheck((m_ModFilter.nCardSet & E_CARDSET_THE_ELAGUE_OF_EXPLORERS) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardSet_Wild[6].SetCheck((m_ModFilter.nCardSet & E_CARDSET_WHISPERS_OF_THE_OLD_GODS) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardSet_Wild[7].SetCheck((m_ModFilter.nCardSet & E_CARDSET_ONE_NIGHT_IN_KARAZHAN) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardSet_Wild[8].SetCheck((m_ModFilter.nCardSet & E_CARDSET_MEAN_STREETS_OF_GADGETZAN) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardSet_Wild[9].SetCheck((m_ModFilter.nCardSet & E_CARDSET_JOURNEY_TO_UNGORO) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardSet_Wild[10].SetCheck((m_ModFilter.nCardSet & E_CARDSET_KNIGHTS_OF_THE_FROZEN_THRONE) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardSet_Wild[11].SetCheck((m_ModFilter.nCardSet & E_CARDSET_KOBOLDS_AND_CATACOMBS) > 0 ? BST_CHECKED : BST_UNCHECKED);

	m_ctrlCardRarity[0].SetCheck((m_ModFilter.nRarity & E_CARDRARITY_FREE) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardRarity[1].SetCheck((m_ModFilter.nRarity & E_CARDRARITY_COMMON) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardRarity[2].SetCheck((m_ModFilter.nRarity & E_CARDRARITY_RARE) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardRarity[3].SetCheck((m_ModFilter.nRarity & E_CARDRARITY_EPIC) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardRarity[4].SetCheck((m_ModFilter.nRarity & E_CARDRARITY_LEGENDARY) > 0 ? BST_CHECKED : BST_UNCHECKED);

	m_ctrlCardClass[0].SetCheck((m_ModFilter.nClass & E_CARDCLASS_DRUID) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardClass[1].SetCheck((m_ModFilter.nClass & E_CARDCLASS_HUNTER) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardClass[2].SetCheck((m_ModFilter.nClass & E_CARDCLASS_MAGE) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardClass[3].SetCheck((m_ModFilter.nClass & E_CARDCLASS_PALADIN) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardClass[4].SetCheck((m_ModFilter.nClass & E_CARDCLASS_PREIST) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardClass[5].SetCheck((m_ModFilter.nClass & E_CARDCLASS_ROGUE) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardClass[6].SetCheck((m_ModFilter.nClass & E_CARDCLASS_SHAMAN) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardClass[7].SetCheck((m_ModFilter.nClass & E_CARDCLASS_WARLOCK) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardClass[8].SetCheck((m_ModFilter.nClass & E_CARDCLASS_WARRIOR) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardClass[9].SetCheck((m_ModFilter.nClass & E_CARDCLASS_NONE) > 0 ? BST_CHECKED : BST_UNCHECKED);

	m_ctrlCardRace[0].SetCheck((m_ModFilter.nRace & E_CARDRACE_MURLOC) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardRace[1].SetCheck((m_ModFilter.nRace & E_CARDRACE_DEMON) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardRace[2].SetCheck((m_ModFilter.nRace & E_CARDRACE_BEAST) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardRace[3].SetCheck((m_ModFilter.nRace & E_CARDRACE_DRAGON) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardRace[4].SetCheck((m_ModFilter.nRace & E_CARDRACE_TOTEM) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardRace[5].SetCheck((m_ModFilter.nRace & E_CARDRACE_PIRATE) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardRace[6].SetCheck((m_ModFilter.nRace & E_CARDRACE_MECH) > 0 ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardRace[7].SetCheck((m_ModFilter.nRace & E_CARDRACE_ELEMENTAL) > 0 ? BST_CHECKED : BST_UNCHECKED);

	CString strTemp;
	strTemp.Format(_T("%d"), m_ModFilter.nFromCost); m_ctrlCardCost[0].SetWindowText(strTemp);
	strTemp.Format(_T("%d"), m_ModFilter.nToCost); m_ctrlCardCost[1].SetWindowText(strTemp);

	strTemp.Format(_T("%d"), m_ModFilter.nFromAttack); m_ctrlCardAttack[0].SetWindowText(strTemp);
	strTemp.Format(_T("%d"), m_ModFilter.nToAttack); m_ctrlCardAttack[1].SetWindowText(strTemp);

	strTemp.Format(_T("%d"), m_ModFilter.nFromHealth); m_ctrlCardHealth[0].SetWindowText(strTemp);
	strTemp.Format(_T("%d"), m_ModFilter.nToHealth); m_ctrlCardHealth[1].SetWindowText(strTemp);

	m_ctrlText.SetWindowText(m_ModFilter.strSearch.c_str());

	m_ctrlBookmarkChk.SetCheck(m_ModFilter.bBookmark == TRUE ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardTypeChk.SetCheck(m_ModFilter.bUseType == TRUE ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardSetChk.SetCheck(m_ModFilter.bUseCardSet == TRUE ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardRarityChk.SetCheck(m_ModFilter.bUseRarity == TRUE ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardClassChk.SetCheck(m_ModFilter.bUseClass == TRUE ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardRaceChk.SetCheck(m_ModFilter.bUseRace == TRUE ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardCostChk.SetCheck(m_ModFilter.bUseCost == TRUE ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardAttackChk.SetCheck(m_ModFilter.bUseAttack == TRUE ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardHealthChk.SetCheck(m_ModFilter.bUseHealth == TRUE ? BST_CHECKED : BST_UNCHECKED);
	m_ctrlCardTextChk.SetCheck(m_ModFilter.bUseText == TRUE ? BST_CHECKED : BST_UNCHECKED);

	OnClickCardTypeChk();
	OnClickCardSetChk();
	OnClickCardRarityChk();
	OnClickCardClassChk();
	OnClickCardRaceChk();
	OnClickCardCostChk();
	OnClickCardAttackChk();
	OnClickCardHealthChk();
	OnClickCardTextChk();
}


void CFilterDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	Dlg2Data();
	//CDialogEx::OnOK();
}


void CFilterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (nID == SC_CLOSE)
		return OnCancel();

	CDialogEx::OnSysCommand(nID, lParam);
}


void CFilterDlg::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	DestroyWindow();
	//CDialogEx::OnCancel();
}


void CFilterDlg::OnClickCardTypeChk()
{
	BOOL bEnable = m_ctrlCardTypeChk.GetCheck() == BST_CHECKED;

	for (int i = 0; i < 4; i++)
		m_ctrlCardType[i].EnableWindow(bEnable);
}

void CFilterDlg::OnClickCardSetChk()
{
	BOOL bEnable = m_ctrlCardSetChk.GetCheck() == BST_CHECKED;

	for (int i = 0; i < 5; i++)
		m_ctrlCardSet_Regular[i].EnableWindow(bEnable);
	for (int i = 0; i < 12; i++)
		m_ctrlCardSet_Wild[i].EnableWindow(bEnable);
}

void CFilterDlg::OnClickCardRarityChk()
{
	BOOL bEnable = m_ctrlCardRarityChk.GetCheck() == BST_CHECKED;

	for (int i = 0; i < 5; i++)
		m_ctrlCardRarity[i].EnableWindow(bEnable);
}

void CFilterDlg::OnClickCardClassChk()
{
	BOOL bEnable = m_ctrlCardClassChk.GetCheck() == BST_CHECKED;

	for (int i = 0; i < 10; i++)
		m_ctrlCardClass[i].EnableWindow(bEnable);
}

void CFilterDlg::OnClickCardRaceChk()
{
	BOOL bEnable = m_ctrlCardRaceChk.GetCheck() == BST_CHECKED;

	for (int i = 0; i < 8; i++)
		m_ctrlCardRace[i].EnableWindow(bEnable);
}

void CFilterDlg::OnClickCardCostChk()
{
	BOOL bEnable = m_ctrlCardCostChk.GetCheck() == BST_CHECKED;

	for (int i = 0; i < 2; i++)
		m_ctrlCardCost[i].EnableWindow(bEnable);
}

void CFilterDlg::OnClickCardAttackChk()
{
	BOOL bEnable = m_ctrlCardAttackChk.GetCheck() == BST_CHECKED;

	for (int i = 0; i < 2; i++)
		m_ctrlCardAttack[i].EnableWindow(bEnable);
}

void CFilterDlg::OnClickCardHealthChk()
{
	BOOL bEnable = m_ctrlCardHealthChk.GetCheck() == BST_CHECKED;

	for (int i = 0; i < 2; i++)
		m_ctrlCardHealth[i].EnableWindow(bEnable);
}

void CFilterDlg::OnClickCardTextChk()
{
	BOOL bEnable = m_ctrlCardTextChk.GetCheck() == BST_CHECKED;

	m_ctrlText.EnableWindow(bEnable);
}

BOOL CFilterDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
