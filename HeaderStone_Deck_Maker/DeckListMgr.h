#pragma once

#include <map>

class CCard;
class CDeckListMgr
{
public:
	CDeckListMgr();
	~CDeckListMgr();

public:
	static BOOL Export2Json(int nIdx, CString strDeckName, std::map<CCard*, int> mapDeckList);
};

