#pragma once

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