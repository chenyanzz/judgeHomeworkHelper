#if defined(_MSC_VER) && (_MSC_VER >= 1600)  
# pragma execution_character_set("utf-8")  
#endif

#include <QString>

#ifndef COMMON_H
#define COMMON_H

#include <clocale>
#include <string>

/**
* WTH IS THIS DOING???
* @see https://www.cnblogs.com/dyg540/articles/7874214.html
*/
inline std::string WString2String(const std::wstring& ws) {
	std::string strLocale = setlocale(LC_ALL, "");
	const wchar_t* wchSrc = ws.c_str();
	size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
	char* chDest = new char[nDestSize];
	memset(chDest, 0, nDestSize);
	wcstombs(chDest, wchSrc, nDestSize);
	std::string strResult = chDest;
	delete[]chDest;
	setlocale(LC_ALL, strLocale.c_str());
	return strResult;
}

inline std::string QString2Ascii(const QString& qs) {
	std::wstring& ws = qs.toStdWString();
	std::string& s = WString2String(ws);
	return s;
}

using cstr = const char*;

#endif //COMMON_H