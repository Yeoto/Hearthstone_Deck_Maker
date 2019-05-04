// FilterDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "HeaderStone_Deck_Maker.h"
#include "FilterDlg.h"
#include "afxdialogex.h"


// CFilterDlg 대화 상자

IMPLEMENT_DYNAMIC(CFilterDlg, CDialogEx)

CFilterDlg::CFilterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILTER_DLG, pParent)
{

}

CFilterDlg::CFilterDlg(CCardFilter* pFilter, CWnd* pParent /*= nullptr*/) :
	m_pFilter(pFilter), CDialogEx(IDD_FILTER_DLG, pParent)
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
}


BEGIN_MESSAGE_MAP(CFilterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RESET_BTN, OnClickReset)
END_MESSAGE_MAP()


// CFilterDlg 메시지 처리기


void CFilterDlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//CDialogEx::PostNcDestroy();
	delete this;
}

void CFilterDlg::OnClickReset()
{
	m_Modified = *m_pFilter;
	Data2Dlg();
}

void CFilterDlg::Dlg2Data()
{

}

void CFilterDlg::Data2Dlg()
{
}
