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

using namespace rapidjson;

CDeckListMgr::CDeckListMgr()
{
}


CDeckListMgr::~CDeckListMgr()
{
}

BOOL CDeckListMgr::Export2Json(int nIdx, CString strDeckName, std::map<CCard*, int> mapDeckList)
{
	//FILE* fp;
	//_tfopen_s(&fp, _T("deck_list.json"), _T("ab+"));
	//if (fp == NULL)
	//	return FALSE;
	std::wifstream in(_T("deck_list.json"), std::ios::in);

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
	auto MakeNewDoc = [&doc]() 
	{
		doc.SetArray();

		GenericValue<UTF16<>> Deck;
		Deck.SetObject();
		Deck.AddMember(_T("Key"), GenericValue<UTF16<>>(kNumberType), doc.GetAllocator());
		Deck.AddMember(_T("Deck Name"), GenericValue<UTF16<>>(kStringType), doc.GetAllocator());
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
	BOOL bModify = FALSE;
	for (auto& DeckData : doc.GetArray())
	{
		GenericValue<UTF16<>> Deck;
		Deck.CopyFrom(static_cast<GenericValue<UTF16<>>>(DeckData.GetObject()), doc.GetAllocator());

		if ( !Deck.HasMember(_T("Key")) )
			continue;

		const GenericValue<UTF16<>>& deckKey = Deck[_T("Key")];

		if ( !deckKey.IsInt() )
			continue;

		if (deckKey.GetInt() == nIdx)
		{
			bModify = TRUE;
			doc.Erase(&DeckData);
			break;
		}
		DeckData = Deck;
	}

	GenericValue<UTF16<>> Deck;
	Deck.SetObject();
	Deck.AddMember(_T("Key"), GenericValue<UTF16<>>(kNumberType), doc.GetAllocator());
	Deck.AddMember(_T("Deck Name"), GenericValue<UTF16<>>(kStringType), doc.GetAllocator());
	Deck.AddMember(_T("Deck List"), GenericValue<UTF16<>>(kArrayType), doc.GetAllocator());

	Deck[_T("Key")] = nIdx;
	Deck[_T("Deck Name")] = GenericValue<UTF16<>>(strDeckName, doc.GetAllocator());

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

	std::wofstream out(_T("deck_list.json"), std::ios::out | std::ios::trunc);
	CString strResult = Stringbuffer.GetString();
	out << strResult.GetBuffer();

	return TRUE;
}
