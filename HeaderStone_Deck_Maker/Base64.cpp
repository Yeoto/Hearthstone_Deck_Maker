#include "stdafx.h"
#include "Base64.h"

#define LITTLE_ENDIAN

const char CCardEncoder::MimeBase64[] = {
'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
'w', 'x', 'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9', '+', '/'
};

int CCardEncoder::DecodeMimeBase64[256] = {
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,
-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};

CCardEncoder* CCardEncoder::m_pInstance;
CCardEncoder::CCardEncoder()
{
}

CCardEncoder::~CCardEncoder()
{
}

int CCardEncoder::Encoding(int *src, int src_size, std::string& result)
{
	int i, j = 0;

	int size = (4 * (src_size / 3)) + (src_size % 3 ? 4 : 0);

	std::vector<char> vecChar;
	for (i = 0; i < src_size; i = i + 3, j = j + 4)
	{
		INT64 temp = 0;
		if (vecChar.size() != j)
			ASSERT(0);
		temp |= (src[i] << 16) & 0xFF0000;
		if ((i + 1) > src_size) temp &= ~0xFF00;
		else temp |= (src[i + 1] << 8) & 0xFF00;
		if ((i + 2) > src_size) temp &= ~0xFF;
		else temp |= (src[i + 2]) & 0xFF; 

		for ( int Idx = 0 ; Idx < 4 && i + Idx <= src_size ; Idx++ )
			vecChar.push_back(MimeBase64[(temp & (0xFC0000 >> (6 * Idx))) >> (6 * (3-Idx))]);

		if ((i + 2) > src_size) vecChar.push_back('=');
		if ((i + 3) > src_size) vecChar.push_back('=');
	}
	result = std::string(vecChar.begin(), vecChar.end());
	return size;
}

void CCardEncoder::Decoding(const char* src, int *result, int *length)
{
	int i, j = 0, src_length, blank = 0;

	src_length = strlen(src);

	for (i = 0; i < src_length; i = i + 4, j = j + 3) {
		//8bit * 4
		//32bit
		INT32 encode = 0x0000;
		encode |= DecodeMimeBase64[src[i]] << 18;
		encode |= DecodeMimeBase64[src[i + 1]] << 12;
		if (src[i + 2] == '=') {
			encode &= ~0x00000FC0;
			blank++;
		}
		else encode |= DecodeMimeBase64[src[i + 2]] << 6;
		if (src[i + 3] == '=') {
			encode &= ~0x0000003F;
			blank++;
		}
		else encode |= DecodeMimeBase64[src[i + 3]];

		result[j] =		(encode & 0x00FF0000) >> 16;
		result[j + 1] = (encode & 0x0000FF00) >> 8;
		result[j + 2] = (encode & 0x000000FF);
	}
	*length = j - blank;
}

CCardEncoder* CCardEncoder::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CCardEncoder();
		atexit(destroy);
	}

	return m_pInstance;
}
