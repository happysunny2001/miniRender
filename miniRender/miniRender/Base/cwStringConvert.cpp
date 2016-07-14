/*
Copyright © 2015-2016 Ziwei Wang (happy.sunny.2001@163.com)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the “Software”), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "cwStringConvert.h"

#include <sstream>
#include <algorithm>

NS_MINIR_BEGIN

CWVOID cwStringConvert::ltrim(CWSTRING& str)
{
	ltrim(str, ' ');
}

CWVOID cwStringConvert::ltrim(CWSTRING& str, CWCHAR c)
{
	str.erase(0, str.find_first_not_of(c));
}

CWVOID cwStringConvert::rtrim(CWSTRING& str)
{
	rtrim(str, ' ');
}

CWVOID cwStringConvert::rtrim(CWSTRING& str, CWCHAR c)
{
	str.erase(str.find_last_not_of(c) + 1);
}

CWVOID cwStringConvert::trim(CWSTRING& str)
{
	trim(str, ' ');
}

CWVOID cwStringConvert::trim(CWSTRING& str, CWCHAR c)
{
	str.erase(0, str.find_first_not_of(c));
	str.erase(str.find_last_not_of(c) + 1);
}

CWVOID cwStringConvert::split(CWSTRING& str, const CWSTRING& strSep, std::vector<CWSTRING>& vecRet)
{
	if (str.empty()) return;

	std::string::size_type pos = str.find_first_not_of(strSep);
	while (pos != std::string::npos) {
		string tmp;
		std::string::size_type sepPos = str.find(strSep, pos);
		if (sepPos != std::string::npos) {
			tmp = str.substr(pos, sepPos - pos);
			pos = sepPos + strSep.length();
		}
		else {
			tmp = str.substr(pos);
			pos = sepPos;
		}

		if (!tmp.empty()) {
			vecRet.push_back(tmp);
		}
	}
}

CWSTRING cwStringConvert::replace(CWSTRING& strOrigin, const CWSTRING& strFind, const CWSTRING& strReplace)
{
	if (strOrigin.empty()) return strOrigin;
	if (strFind.empty()) return strOrigin;

	std::stringstream ss;

	std::string::size_type startPos = 0;
	std::string::size_type pos = strOrigin.find(strFind, startPos);
	while (pos != std::string::npos) {
		ss << strOrigin.substr(startPos, pos - startPos);
		ss << strReplace;

		startPos = pos + strFind.length();
		if (startPos >= strOrigin.length()) break;

		pos = strOrigin.find(strFind, startPos);
		if (pos == std::string::npos) {
			ss << strOrigin.substr(startPos);
			break;
		}
	}

	return ss.str();
}

CWBYTE cwStringConvert::parseHexStringByte(const CWSTRING& strHex)
{
	static CWBYTE baseData[23] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15 };

	CWBYTE ret = 0;

	CWSTRING strLower = strHex;
	std::transform(strLower.begin(), strLower.end(), strLower.begin(), ::toupper);

	auto nPos = strLower.find('X');
	if (nPos == std::string::npos) return ret;
	if (nPos + 2 >= strLower.size()) return ret;

	CWUINT indexUpper = strLower[nPos + 1] - '0';
	if (indexUpper < 23)
		ret |= baseData[indexUpper] << 4;

	CWUINT indexLower = strLower[nPos + 2] - '0';
	if (indexLower < 23)
		ret |= baseData[indexLower];

	return ret;
}

CWINT cwStringConvert::convertToInt(const CWSTRING& str, CWINT iFailed)
{
	stringstream ss;
	CWINT i;
	ss << str;
	ss >> i;
	if (ss.fail()) return iFailed;
	return i;
}

CWSTRING cwStringConvert::convertToString(CWUINT i)
{
	stringstream ss;
	ss << i;
	return ss.str();
}

NS_MINIR_END
