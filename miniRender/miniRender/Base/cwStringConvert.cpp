/*
Copyright © 2015 Ziwei Wang

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

NS_MINIR_BEGIN

CWVOID cwStringConvert::ltrim(CWSTRING& str)
{
	str.erase(0, str.find_first_not_of(' '));
}

CWVOID cwStringConvert::rtrim(CWSTRING& str)
{
	str.erase(str.find_last_not_of(' ')+1);
}

CWVOID cwStringConvert::trim(CWSTRING& str)
{
	str.erase(0, str.find_first_not_of(' '));
	str.erase(str.find_last_not_of(' ') + 1);
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

NS_MINIR_END
