#pragma once

#include <string>

class CRegistryUtil
{
public:
	CRegistryUtil();
	~CRegistryUtil();

	static BOOL WriteStringValueInRegistry(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, LPCTSTR lpValue);
	static BOOL WriteDWORDValueInRegistry(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, DWORD dwValue);
	static CString ReadStringValueInRegistry(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey);
	static DWORD ReadDWORDValueInRegistry(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, DWORD dwDefault = -1);

	static BOOL SetDeckCode(CString strDeckName, std::string strDeckCode);
	static BOOL SetBookMark(int dbfId);
	static std::string GetDeckCode(CString deckName);
	static BOOL IsBookMark(int dbfId);

	static BOOL AddSubBaseKey();
	static BOOL AddSubKey(HKEY hKey, LPCTSTR lpSubKey, HKEY* pSubKey);
private:
	static CString m_strSubKey_Bookmark;
	static CString m_strSubKey_Decks;
};
