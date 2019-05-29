#pragma once

#include <map>
#include "CardEnum.h"

#include <vector>
#include <map>

class CCard;

struct LocalDeckData
{
	E_CARDCLASS eClass;
	CString strName;
	std::map<CCard*, int> mapCardList;
};

class CDeckListMgr
{
public:
	CDeckListMgr();
	~CDeckListMgr();

public:
	static BOOL Export2Json(int nIdx, CString strDeckName, E_CARDCLASS eDeckClass, std::map<CCard*, int> mapDeckList);
	static BOOL Import2Json(std::map<int, LocalDeckData*>& mapLocalDeck);
};

