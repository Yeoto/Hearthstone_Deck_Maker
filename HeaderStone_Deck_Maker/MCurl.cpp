#include "stdafx.h"
#include "MCurl.h"
#include <stdlib.h>
#include <direct.h>
#include <sys/stat.h>

#include <curl.h>

#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "ws2_32.lib")

MCurl* MCurl::m_pInstance;
std::string MCurl::buffer;
MCurl::MCurl() :
	m_strAPIHost("omgvamp-hearthstone-v1.p.rapidapi.com"), m_strAPIKey("FtLw2MDySLmshyBjDI8UXrGOU9qDp1xnrRXjsn5pvdVuwgPqYV")
{
}

MCurl::~MCurl()
{
}

CString MCurl::get(MURL UrlData)
{
	CURL *curl;
	CURLcode res;

	//curl --get --include "https://omgvamp-hearthstone-v1.p.rapidapi.com/cards/%EB%A7%88%EB%82%98%20%EC%A7%80%EB%A3%A1?locale=koKR" 
	//-H "X-RapidAPI-Host: omgvamp-hearthstone-v1.p.rapidapi.com" 
	//-H "X-RapidAPI-Key: FtLw2MDySLmshyBjDI8UXrGOU9qDp1xnrRXjsn5pvdVuwgPqYV"
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, UrlData.strCmd.c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		
		std::string response_string;
		std::string header_string;
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData2strBuf);
		
		curl_slist* responseHeaders = NULL;
		for ( MURLHeader header : UrlData.Header )
		{
			std::string strHeader("");
			strHeader.append(header.strHead);
			strHeader.append(": ");
			strHeader.append(header.strData);
			responseHeaders = curl_slist_append(responseHeaders, strHeader.c_str());
		}
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, responseHeaders);
		
		char* url;
		long response_code;
		double elapsed;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
		curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &elapsed);
		curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);

		res = curl_easy_perform(curl);

		curl_slist_free_all(responseHeaders);
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();

	return CString(buffer.c_str());
}

std::string MCurl::DownloadImg(std::string filePath, std::string urlImg)
{
	CURL* curl;
	FILE* fp;
	CURLcode res;

	std::string filename =urlImg.substr(urlImg.rfind("/") +1, urlImg.length());
	filePath += filename;

	struct stat buffer;
	// 같은 이름 파일 있으면 스킵함
	if ( stat(filePath.c_str(), &buffer) == 0 )
		return filePath;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (curl)
	{
		fopen_s(&fp, filePath.c_str(), "wb+");
		curl_easy_setopt(curl, CURLOPT_URL, urlImg.c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData2File);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		res = curl_easy_perform(curl);
		/* always cleanup */
		curl_easy_cleanup(curl);
		fclose(fp);
	}
	curl_global_cleanup();
	return filePath;
}

void MCurl::put(MURL UrlData)
{
}

std::string MCurl::GetAPIHost()
{
	return m_strAPIHost;
}

std::string MCurl::GetAPIKey()
{
	return m_strAPIKey;
}

MCurl * MCurl::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new MCurl();
		atexit(destroy);
	}

	return m_pInstance;
}

size_t MCurl::WriteData2strBuf(void * ptr, size_t size, size_t nmemb, void * userp)
{
	buffer.append((char*)ptr, size*nmemb);
	return size * nmemb;
}

size_t MCurl::WriteData2File(void * contents, size_t size, size_t nmemb, void * userp)
{
	size_t written;
	written = fwrite(contents, size, nmemb, (FILE*)userp);
	return written;
}

