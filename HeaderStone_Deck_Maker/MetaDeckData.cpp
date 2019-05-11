#include "stdafx.h"
#include "MetaDeckData.h"

#include "Card.h"

CMetaDeckData::CMetaDeckData()
{

}

void CMetaDeckData::Init()
{
	m_nArcheType = -2;
	m_strArcheType = _T("");
	m_nAvgGameLength = 0;
	m_dAvgPlayerTurn = 0.0;
	m_mapDeckList.clear();
	m_nTotalGames = 0;
	m_dWinRate = 0.0;
}
