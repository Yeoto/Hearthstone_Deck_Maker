#pragma once

#include <map>
#include "CardEnum.h"

class CCard;
class CCardListMgr;

class CMetaDeckData
{
public:
	CMetaDeckData();

public:
	CMetaDeckData & operator=(const CMetaDeckData &rhs);
	void Init();
	BOOL ImportMetaDeckData(std::wstring strName, void* pValue);
	BOOL DeckListStr2DeckListMap();
public:
	E_CARDCLASS m_eDeckClass;
	int m_nArcheType;
	std::wstring m_strArcheType;
	double m_dAvgGameLength;
	double m_dAvgPlayerTurn;
	std::wstring m_strDeckList;
	std::map<CCard*, int> m_mapDeckList;
	int m_nTotalGames;
	double m_dWinRate;

	//archetype_id		- 2
	//avg_game_length_seconds		523
	//avg_num_player_turns		9.5
	//deck_id		"Qd1fssr8NXP0o36AmpBPwb"
	//deck_list		"[[47887,2],[49990,2],[48612,1],[254,2],[47423,2],[52809,1],[51769,2],[48625,1],[1659,2],[48837,1],[836,2],[1014,2],[47287,1],[481,2],[1050,2],[932,2],[467,2],[52434,1]]"
	//digest		"2cdeb4ce3fd31ae74137f53fc2f3a1b0"
	//total_games		1774
	//win_rate		31.45
};