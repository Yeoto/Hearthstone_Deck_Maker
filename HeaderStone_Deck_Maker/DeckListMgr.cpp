#include "stdafx.h"
#include "DeckListMgr.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Card.h"

using namespace rapidjson;

CDeckListMgr::CDeckListMgr()
{
}


CDeckListMgr::~CDeckListMgr()
{
}

BOOL CDeckListMgr::Export2Json(std::map<CCard*, int> mapDeckList)
{

}
