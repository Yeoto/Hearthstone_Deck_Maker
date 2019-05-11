#include "stdafx.h"
#include "CardFilter.h"

#include "Card.h"

BOOL CCardFilter::IsAgree(CCard* pCard)
{
	BOOL bResult = TRUE;
	if (bUseCost)
		bResult &= nFromCost <= pCard->nCost && pCard->nCost <= nToCost;
	if (bUseAttack)
		bResult &= nFromAttack <= pCard->nAttack && pCard->nAttack <= nToAttack;
	if (bUseHealth)
		bResult &= nFromHealth <= pCard->nHealth && pCard->nHealth <= nToHealth;

	if (bUseCardSet)
		bResult &= (pCard->eCardSet & nCardSet) > 0;
	if (bUseClass)
		bResult &= (pCard->eClass & nClass) > 0;
	if (bUseRarity)
		bResult &= (pCard->eRarity & nRarity) > 0;
	if (bUseRace)
		bResult &= (pCard->eRace & nRace) > 0;
	if (bUseType)
		bResult &= (pCard->eType & nType) > 0;

	if (bUseText)
	{
		BOOL bTemp = FALSE;
		std::size_t posName = pCard->strName.find(strSearch);
		if (posName != std::string::npos)
			bTemp = TRUE;

		std::size_t posText = pCard->strText.find(strSearch);
		if (posText != std::string::npos)
			bTemp = TRUE;

		bResult &= bTemp;
	}

	return bResult;
}

CCardFilter & CCardFilter::operator=(const CCardFilter &rhs)
{
	if (this == &rhs)      // µø¿œ ∞¥√º?
		return *this;

	bUseText = rhs.bUseText;
	bUseCardSet = rhs.bUseCardSet;
	bUseType = rhs.bUseType;
	bUseRace = rhs.bUseRace;
	bUseRarity = rhs.bUseRarity;
	bUseClass = rhs.bUseClass;
	bUseCost = rhs.bUseCost;
	bUseAttack = rhs.bUseAttack;
	bUseHealth = rhs.bUseHealth;

	strSearch = rhs.strSearch;
	nCardSet = rhs.nCardSet;
	nType = rhs.nType;
	nRace = rhs.nRace;
	nRarity = rhs.nRarity;
	nClass = rhs.nClass;

	bUseCost = rhs.bUseCost;
	nFromCost = rhs.nFromCost;
	nToCost = rhs.nToCost;

	bUseAttack = rhs.bUseAttack;
	nFromAttack = rhs.nFromAttack;
	nToAttack = rhs.nToAttack;

	bUseHealth = rhs.bUseHealth;
	nFromHealth = rhs.nFromHealth;
	nToHealth = rhs.nToHealth;
	return *this;
}
