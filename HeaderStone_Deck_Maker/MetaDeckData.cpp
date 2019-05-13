#include "stdafx.h"
#include "MetaDeckData.h"

#include <regex>
#include <algorithm>

#include "Card.h"
#include "CardListMgr.h"

CMetaDeckData::CMetaDeckData()
{

}

CMetaDeckData& CMetaDeckData::operator=(const CMetaDeckData &rhs)
{
	if (this == &rhs)      // µø¿œ ∞¥√º?
		return *this;

	m_nArcheType		= rhs.m_nArcheType;
	m_strArcheType		= rhs.m_strArcheType;
	m_dAvgGameLength	= rhs.m_dAvgGameLength;
	m_dAvgPlayerTurn	= rhs.m_dAvgPlayerTurn;
	m_strDeckList		= rhs.m_strDeckList;
	m_mapDeckList		= rhs.m_mapDeckList;
	m_nTotalGames		= rhs.m_nTotalGames;
	m_dWinRate			= rhs.m_dWinRate;
	return *this;
}

void CMetaDeckData::Init()
{
	m_nArcheType = -2;
	m_strArcheType = _T("");
	m_dAvgGameLength = 0;
	m_dAvgPlayerTurn = 0.0;
	m_strDeckList = _T("");
	m_mapDeckList.clear();
	m_nTotalGames = 0;
	m_dWinRate = 0.0;
}

BOOL CMetaDeckData::ImportMetaDeckData(std::wstring strName, void* pValue)
{
	std::transform(strName.begin(), strName.end(), strName.begin(), ::tolower);

	//"deck_id": "gvmIowQruHbLgsZ9hTfQE",
	//"deck_list" : "[[51779,2],[381,2],[52809,1],[48607,2],[52810,2],[47063,2],[836,2],[503,2],[493,1],[52819,2],[51781,2],[64,2],[51380,2],[51790,2],[742,2],[50042,2]]",
	//"archetype_id" : 7,
	//"digest" : "10421dba5af249b2d0dc1812efdc845c",
	//"total_games" : 482,
	//"win_rate" : 59.54,
	//"avg_game_length_seconds" : 414,
	//"avg_num_player_turns" : 8.5
	if (strName.compare(_T("archetype_id")) == 0)
		m_nArcheType = *(int*)pValue;
	else if (strName.compare(_T("total_games")) == 0)
		m_nTotalGames = *(int*)pValue;
	else if (strName.compare(_T("win_rate")) == 0)
		m_dWinRate = *(double*)pValue;
	else if (strName.compare(_T("deck_list")) == 0)
		m_strDeckList = *(std::wstring*)pValue;
	else if (strName.compare(_T("avg_game_length_seconds")) == 0)
		m_dAvgGameLength = *(double*)pValue;
	else if (strName.compare(_T("avg_num_player_turns")) == 0)
		m_dAvgPlayerTurn = *(double*)pValue;

	return TRUE;
}

BOOL CMetaDeckData::DeckListStr2DeckListMap()
{
	if (m_strDeckList.compare(_T("")) == 0)
	{
		ASSERT(0);
		return FALSE;
	}

	std::wregex reg(L"\\[([0-9]+),([0-9]+)\\]");
	std::wsregex_iterator it(m_strDeckList.begin(), m_strDeckList.end(), reg);
	std::wsregex_iterator end;

	m_mapDeckList.clear();
	std::vector<CCard*> vecCardList = CCardListMgr::GetInstance()->GetCardList();
	while (it != end)
	{
		std::wsmatch m = *it;

		std::wstring m1 = m.str(1);
		std::wstring m2 = m.str(2);
		int nDbfID = std::stoi(m1);
		int nCnt = std::stoi(m2);
		auto itrCard = std::find_if(vecCardList.begin(), vecCardList.end(), [&nDbfID](CCard* pCard) { return pCard->ndbfID == nDbfID;  });

		m_mapDeckList[*itrCard] = nCnt;
		++it;
	}
	return TRUE;
}