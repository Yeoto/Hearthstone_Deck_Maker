#include "stdafx.h"
#include "CardFilter.h"

#include "Card.h"
#include "RegistryUtil.h"

void CCardFilter::init()
{
	bBookmark = FALSE;

	bUseText = FALSE;
	strSearch = _T("");

	bUseCardSet = TRUE;
	nCardSet = E_CARDSET_REGULAR;

	bUseType = FALSE;
	nType = 0;

	bUseRace = FALSE;
	nRace = 0;

	bUseRarity = FALSE;
	nRarity = E_CARDRARITY_NONE;

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
}

BOOL CCardFilter::IsAgree(CCard* pCard)
{
	BOOL bResult = TRUE;

	if (bBookmark)
		bResult &= CRegistryUtil::IsBookMark(pCard->ndbfID);

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
		std::wstring strSearchTemp = strSearch;
		strSearchTemp.erase(std::remove_if(strSearchTemp.begin(), strSearchTemp.end(), [](wchar_t chr) { return chr == ' ';  }), strSearchTemp.end());

		BOOL bTemp = FALSE;
		std::wstring  strTemp = pCard->strName;
		strTemp.erase(std::remove_if(strTemp.begin(), strTemp.end(), [](wchar_t chr) { return chr == ' ';  }), strTemp.end());
		std::size_t posName = strTemp.find(strSearch);
		if (posName != std::string::npos)
			bTemp = TRUE;

		strTemp = pCard->strText;
		strTemp.erase(std::remove_if(strTemp.begin(), strTemp.end(), [](wchar_t chr) { return chr == ' ';  }), strTemp.end());
		std::size_t posText = strTemp.find(strSearch);
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

	bBookmark = rhs.bBookmark;

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
