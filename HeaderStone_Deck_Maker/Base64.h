#pragma once
class CCardEncoder
{
	CCardEncoder();
	~CCardEncoder();

public:
	int Encoding(char *src, int src_size, char **result);
	void Decoding(const char *src, int *result, int *length);

public:
	static CCardEncoder* GetInstance();

private:
	static void destroy() { delete m_pInstance; }
	static CCardEncoder* m_pInstance;
	static const char MimeBase64[];
	static int DecodeMimeBase64[256];
};
