#pragma once

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "CardEnum.h"
#include "CardFilter.h"

class CProgressDlg;
class CCardEncoder;
class CCard;
class CMetaDeckData;

class CCardListMgr
{
	CCardListMgr();
	~CCardListMgr();
public:
	static CCardListMgr* GetInstance();
	const std::vector<CCard*>& GetCardList() { return m_vecCardList; }
	const std::vector<CCard*>& GetHeroSkinList() { return m_vecHeroSkinList; }
	std::vector<CCard*> GetFilteredList();
	std::vector<CCard*> GetFilteredList(std::vector<CCard*> vecCardList);
	int GetCardListCnt() { return m_vecCardList.size(); }
	CCard* GetTempCard() { return m_pTempCard; }
	CCardFilter* GetCardFilter() { return &m_Filter;  }

	BOOL CheckCardData(CCard* pCard);
	void AddCard(CCard* pCard);
	void MakeTempCard();
	void TraceAll();
	void DownloadAllImg(BOOL* bBreak);

	void AddMetaDeck(E_CARDCLASS eCardClass, CMetaDeckData* pMetaDeck);
	std::map<E_CARDCLASS, std::vector<CMetaDeckData*>> GetMetaDeckList() {return m_mapMetaDeckList;}

	void DeckCode2CardList(std::string deckCode, E_CARDCLASS& eDeckClass, std::map<CCard*, int>& m_mapCards);
	void CardList2DeckCode(E_CARDCLASS eDeckClass, std::map<CCard*, int> mapDeckList, CString& deckCode);

	std::string GetImgPath() { return m_imgPath; }
private:
	static CCardListMgr* m_pInstance;
	static void destroy() { delete m_pInstance; }
	std::vector<CCard*> m_vecHeroSkinList;
	std::vector<CCard*> m_vecCardList;
	std::map<E_CARDCLASS, std::vector<CMetaDeckData*>> m_mapMetaDeckList;
	CCard* m_pTempCard;
	CCardFilter m_Filter;
	std::string m_imgPath;
};