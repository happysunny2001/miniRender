﻿/*
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

#include "cwCommon.h"
#include <algorithm>

NS_MINIR_BEGIN

CWBYTE cwCommon::parseHexStringByte(const CWSTRING& strHex)
{
	static CWBYTE baseData[23] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15};

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

NS_MINIR_END
