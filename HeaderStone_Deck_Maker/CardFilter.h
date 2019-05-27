#pragma once

#include <string>
#include <algorithm>

#include "CardEnum.h"

class CCard;
class CRegistryUtil;

class CCardFilter
{
public:
	BOOL bBookmark;

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
		init();
	};

public:
	void init();
	BOOL IsAgree(CCard* pCard);
	CCardFilter & operator=(const CCardFilter &rhs);
};