#include "stdafx.h"

#include <windows.h>
#include "RegistryUtil.h"

CString CRegistryUtil::m_strSubKey_Bookmark = _T("SOFTWARE\\YeotoSoft\\DeckMaker\\Bookmark");
CString CRegistryUtil::m_strSubKey_Decks = _T("SOFTWARE\\YeotoSoft\\DeckMaker\\DeckList");

CRegistryUtil::CRegistryUtil()
{
}


CRegistryUtil::~CRegistryUtil()
{
}

BOOL CRegistryUtil::WriteStringValueInRegistry( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, LPCTSTR lpValue )
{
	CString strValue = _T("");
	HKEY hSubKey = NULL;
	// open the key 
	if ( ::RegOpenKeyEx( hKey, lpSubKey, 0, KEY_ALL_ACCESS, &hSubKey ) == ERROR_SUCCESS ) 
	{
		DWORD cbSize = (DWORD)_tcslen(lpValue) + 1;
		BYTE *pBuf = new BYTE [ cbSize ];
		::ZeroMemory( pBuf, cbSize );
		::CopyMemory( pBuf, lpValue, cbSize - 1 );
		::RegSetValueEx( hSubKey, lpValueKey, NULL, REG_SZ, pBuf, cbSize );
		// 키 닫기
		::RegCloseKey( hSubKey );
		delete [] pBuf;
		return TRUE;
	} 
	return FALSE;
}

BOOL CRegistryUtil::WriteDWORDValueInRegistry( HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, DWORD dwValue )
{ 
	CString strValue = _T("");
	HKEY hSubKey = NULL;
	// open the key 
	if (::RegOpenKeyEx(hKey, lpSubKey, 0, KEY_ALL_ACCESS, &hSubKey) != ERROR_SUCCESS)
	{
		if (!AddSubKey(hKey, lpSubKey, &hSubKey))
		{
			ASSERT(0);
			return FALSE;
		}
	}

	::RegSetValueEx( hSubKey, lpValueKey, NULL, REG_DWORD, (LPBYTE)&dwValue, sizeof(DWORD) );
	// 키 닫기 
	::RegCloseKey( hSubKey );
	return TRUE;
}

CString CRegistryUtil::ReadStringValueInRegistry(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey)
{
	CString strValue = _T("");
	HKEY hSubKey = NULL;

	// open the key
	if (::RegOpenKeyEx(hKey, lpSubKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
	{
		DWORD buf_size = 0;

		// 문자열의 크기를 먼저 읽어온다.
		if (::RegQueryValueEx(hSubKey, lpValueKey, NULL, NULL, NULL, &buf_size) == ERROR_SUCCESS)
		{
			// 메모리 할당하고...,
			TCHAR *pBuf = new TCHAR[buf_size + 1];

			// 실제 값을 읽어온다.
			if (::RegQueryValueEx(hSubKey, lpValueKey, NULL, NULL, (LPBYTE)pBuf, &buf_size) == ERROR_SUCCESS)
			{
				pBuf[buf_size] = _T('\0');
				strValue = CString(pBuf);
			}

			// to avoid leakage
			delete[] pBuf;
		}

		// 키 닫기
		::RegCloseKey(hSubKey);
	}

	return strValue;
}

DWORD CRegistryUtil::ReadDWORDValueInRegistry(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, DWORD dwDefault /*= -1*/)
{
	DWORD dwValue = (DWORD)dwDefault;
	HKEY hSubKey = NULL;

	// 키 열기
	if (::RegOpenKeyEx(hKey, lpSubKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
	{
		DWORD buf_size = sizeof(DWORD);

		// DWORD 값 읽어오기
		if (::RegQueryValueEx(hSubKey, lpValueKey, NULL, NULL, (LPBYTE)&dwValue, &buf_size) == ERROR_SUCCESS)
		{
			ASSERT(buf_size == sizeof(DWORD));
		}

		// 키 닫기
		::RegCloseKey(hSubKey);
	}

	return dwValue;
}

BOOL CRegistryUtil::SetDeckCode(CString strDeckName, std::string strDeckCode)
{
	std::wstring strTemp;
	strTemp.assign(strDeckCode.begin(), strDeckCode.end());

	return WriteStringValueInRegistry(HKEY_LOCAL_MACHINE, m_strSubKey_Decks, strDeckName, strTemp.c_str());
}

BOOL CRegistryUtil::SetBookMark(int dbfId)
{
	CString strTemp;
	strTemp.Format(_T("%d"), dbfId);

	int nTemp = 1;
	if (ReadDWORDValueInRegistry(HKEY_LOCAL_MACHINE, m_strSubKey_Bookmark, strTemp) == 1)
		nTemp = 0;

	return WriteDWORDValueInRegistry(HKEY_LOCAL_MACHINE, m_strSubKey_Bookmark, strTemp, nTemp);
}

std::string CRegistryUtil::GetDeckCode(CString deckName)
{
	std::wstring wstrData = ReadStringValueInRegistry(HKEY_LOCAL_MACHINE, m_strSubKey_Decks, deckName);
	std::string temp;
	temp.assign(wstrData.begin(), wstrData.end());

	return temp;
}

BOOL CRegistryUtil::IsBookMark(int dbfId)
{
	CString strID;
	strID.Format(_T("%d"), dbfId);
	return ReadDWORDValueInRegistry(HKEY_LOCAL_MACHINE, m_strSubKey_Bookmark, strID, 0) == 1;
}

BOOL CRegistryUtil::AddSubBaseKey()
{
	//"SOFTWARE\\YeotoSoft\\DeckMaker\\Bookmark"
	HKEY temp1;
	HKEY temp2;

	long ret1 = RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\YeotoSoft"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_CREATE_SUB_KEY, NULL, &temp1, NULL);
	long ret2 = RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\YeotoSoft\\DeckMaker"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_CREATE_SUB_KEY, NULL, &temp2, NULL);

	if (temp1 != NULL)
		RegCloseKey(temp1);
	if (temp2 != NULL)
		RegCloseKey(temp2);

	return TRUE;
}

BOOL CRegistryUtil::AddSubKey(HKEY hKey, LPCTSTR lpSubKey, HKEY* pSubKey)
{
	//AddSubBaseKey();

	DWORD disposition = 0;
	long ret = RegCreateKeyEx(hKey, lpSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_CREATE_SUB_KEY, NULL, pSubKey, &disposition);
	if (ret != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}
