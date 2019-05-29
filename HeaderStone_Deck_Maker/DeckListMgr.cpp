#include "stdafx.h"
#include "DeckListMgr.h"

#include <fstream>
#include <iostream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"

#include "Card.h"
#include "CardListMgr.h"

#include <algorithm>

using namespace rapidjson;

CDeckListMgr::CDeckListMgr()
{
}


CDeckListMgr::~CDeckListMgr()
{
}

BOOL CDeckListMgr::Export2Json(int nIdx, CString strDeckName, E_CARDCLASS eDeckClass, std::map<CCard*, int> mapDeckList)
{
	std::wstring strProgPath = CCardListMgr::GetInstance()->GetProgramPath();
	strProgPath += _T("deck_list.json");
	std::locale::global(std::locale("Korean"));
	std::wifstream in(strProgPath, std::ios::in);

	wchar_t temp[2048];
	CString strDeckList;

	if (in.is_open())
	{
		while (!in.eof())
		{
			in.getline(temp, 2048);
			strDeckList += CString(temp);
		}

		in.close();
	}

	GenericDocument<UTF16<>> doc;
	auto MakeNewDoc = [&doc]() 
	{
		doc.SetArray();

		GenericValue<UTF16<>> Deck;
		Deck.SetObject();
		Deck.AddMember(_T("Key"), GenericValue<UTF16<>>(-1), doc.GetAllocator());
		Deck.AddMember(_T("Deck Name"), GenericValue<UTF16<>>(kStringType), doc.GetAllocator());
		Deck.AddMember(_T("Deck Class"), GenericValue<UTF16<>>(kNumberType), doc.GetAllocator());
		Deck.AddMember(_T("Deck List"), GenericValue<UTF16<>>(kArrayType), doc.GetAllocator());

		doc.PushBack(Deck, doc.GetAllocator()); 
	};

	if (strDeckList == _T(""))
	{
		MakeNewDoc();
	}
	else
	{
		doc.Parse(strDeckList);

		if (doc.HasParseError() == TRUE)
			MakeNewDoc();
	}

	ASSERT(doc.IsArray());
	int nMaxKey = -1;
	for (auto& DeckData : doc.GetArray())
	{
		GenericValue<UTF16<>> Deck;
		Deck.CopyFrom(static_cast<GenericValue<UTF16<>>>(DeckData.GetObject()), doc.GetAllocator());

		if ( !Deck.HasMember(_T("Key")) )
			continue;

		const GenericValue<UTF16<>>& deckKey = Deck[_T("Key")];

		if ( !deckKey.IsInt() )
			continue;

		int nCurKey = deckKey.GetInt();

		if (nCurKey > nMaxKey)
			nMaxKey = nCurKey;

		if (nCurKey == nIdx)
		{
			doc.Erase(&DeckData);
			break;
		}
		DeckData = Deck;
	}

	GenericValue<UTF16<>> Deck;
	Deck.SetObject();
	Deck.AddMember(_T("Key"), GenericValue<UTF16<>>(kNumberType), doc.GetAllocator());
	Deck.AddMember(_T("Deck Name"), GenericValue<UTF16<>>(kStringType), doc.GetAllocator());
	Deck.AddMember(_T("Deck Class"), GenericValue<UTF16<>>(kNumberType), doc.GetAllocator());
	Deck.AddMember(_T("Deck List"), GenericValue<UTF16<>>(kArrayType), doc.GetAllocator());

	Deck[_T("Key")] = nIdx < 0 ? nMaxKey + 1 : nIdx;
	Deck[_T("Deck Name")] = GenericValue<UTF16<>>(strDeckName, doc.GetAllocator());
	Deck[_T("Deck Class")] = eDeckClass;
	
	for (auto& itr : mapDeckList)
	{
		int nDbfID = itr.first->ndbfID;
		int nCnt = itr.second;
		GenericValue<UTF16<>> Card(kObjectType);

		CString strDbfID;
		strDbfID.Format(_T("%d"), nDbfID);
		Card.AddMember(GenericValue<UTF16<>>(strDbfID, doc.GetAllocator()), GenericValue<UTF16<>>(nCnt), doc.GetAllocator());
		Deck[_T("Deck List")].PushBack(Card, doc.GetAllocator());
	}

	doc.PushBack(Deck, doc.GetAllocator());

	GenericStringBuffer<UTF16<>> Stringbuffer;
	Writer<GenericStringBuffer<UTF16<>>, UTF16<>, UTF16<>> writer(Stringbuffer);
	doc.Accept(writer);

	std::wofstream out(strProgPath, std::ios::out | std::ios::trunc);
	CString strResult = Stringbuffer.GetString();
	out << strResult.GetBuffer();

	return TRUE;
}

BOOL CDeckListMgr::Import2Json(std::map<int, LocalDeckData*>& mapLocalDeck)
{
	std::wstring strProgPath = CCardListMgr::GetInstance()->GetProgramPath();
	strProgPath += _T("deck_list.json");
	std::locale::global(std::locale("Korean"));
	std::wifstream in(strProgPath, std::ios::in);

	wchar_t temp[2048];
	CString strDeckList;

	if (!in.is_open())
		return FALSE;

	while (!in.eof())
	{
		in.getline(temp, 2048);
		strDeckList += CString(temp);
	}

	in.close();

	GenericDocument<UTF16<>> doc;
	if (strDeckList == _T(""))
	{
		return FALSE;
	}
	else
	{
		doc.Parse(strDeckList);

		if (doc.HasParseError() == TRUE)
			return FALSE;
	}

	ASSERT(doc.IsArray());
	BOOL bModify = FALSE;
	for (auto& DeckData : doc.GetArray())
	{
		GenericValue<UTF16<>> Deck;
		Deck.CopyFrom(static_cast<GenericValue<UTF16<>>>(DeckData.GetObject()), doc.GetAllocator());

		if (!Deck.HasMember(_T("Key")))
			continue;

		const GenericValue<UTF16<>>& deckKey = Deck[_T("Key")];
		ASSERT(deckKey.IsNumber());

		const GenericValue<UTF16<>>& deckName = Deck[_T("Deck Name")];
		ASSERT(deckName.IsString());

		const GenericValue<UTF16<>>& deckClass = Deck[_T("Deck Class")];
		ASSERT(deckClass.IsNumber());

		const GenericValue<UTF16<>>& deckList = Deck[_T("Deck List")];
		ASSERT(deckList.IsArray());

		int nKey = deckKey.GetInt();

		LocalDeckData* pLocalDeck = new LocalDeckData();	//대화상자에서 delete 해줄꺼임
		pLocalDeck->nKey = nKey;
		pLocalDeck->eClass = (E_CARDCLASS)deckClass.GetInt();
		pLocalDeck->strName = deckName.GetString();

		const std::vector<CCard*> vecCards = CCardListMgr::GetInstance()->GetCardList();
		for (auto& CardData : deckList.GetArray())
		{
			ASSERT(CardData.IsObject());

			for (GenericValue<UTF16<>>::ConstMemberIterator itrValue = CardData.MemberBegin(); itrValue != CardData.MemberEnd(); ++itrValue)
			{
				ASSERT(itrValue->name.IsString());
				ASSERT(itrValue->value.IsNumber());

				int ndbfID = _ttoi(itrValue->name.GetString());
				int nCnt = itrValue->value.GetInt();
				
				auto itrCard = std::find_if(vecCards.begin(), vecCards.end(), [&ndbfID](CCard* pCard) {return pCard->ndbfID == ndbfID; });

				if (itrCard == vecCards.end())
					continue;

				CCard* pCard = *itrCard;
				pLocalDeck->mapCardList[pCard] = nCnt;
			}
		}

		if (mapLocalDeck.find(nKey) != mapLocalDeck.end())
			ASSERT(0);

		mapLocalDeck[nKey] = pLocalDeck;
	}
	return TRUE;
}
