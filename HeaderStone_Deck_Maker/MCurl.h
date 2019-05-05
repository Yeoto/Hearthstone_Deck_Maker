#if !defined(__MCURL_H__)
#define __MCURL_H__

#include <vector>
#include <string>

struct MURLHeader
{
	std::string strHead;
	std::string strData;

	MURLHeader(std::string strHead, std::string strData)
	{
		this->strHead = strHead;
		this->strData = strData;
	}
};
struct MURL
{
	std::string strCmd;
	std::vector<MURLHeader> Header;

	MURL operator=(MURL src)
	{
		this->strCmd = src.strCmd;
		this->Header = src.Header;
	};
};

class MCurl
{
	MCurl();
	~MCurl();

public:
	CString get(MURL UrlData);
	std::wstring DownloadImg(std::string filePath, std::string urlImg);
	void put(MURL UrlData);

	std::string GetAPIHost();
	std::string GetAPIKey();

private:
	const std::string m_strAPIHost;
	const std::string m_strAPIKey;

public:
	static MCurl* GetInstance() ;
	static size_t WriteData2strBuf(void *contents, size_t size, size_t nmemb, void *userp);
	static size_t WriteData2File(void *contents, size_t size, size_t nmemb, void *userp);
private:
	static void destroy() { delete m_pInstance; }
	static MCurl* m_pInstance;
	static std::string buffer;
};

#endif