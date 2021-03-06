﻿/*
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

#ifndef __CW_STRINGCONVERT_H__
#define __CW_STRINGCONVERT_H__

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include <string>
using namespace std;

NS_MINIR_BEGIN

class CW_DLL cwStringConvert
{
public:
	static CWWSTRING convertToWideChar(const CWSTRING& str);
	static CWWSTRING createFPSString(const CWWSTRING& title, CWUINT iFPS);
	static CWSTRING convertToMultiByte(const CWWSTRING& str);
	static CWINT convertToInt(const CWSTRING& str, CWINT iFailed=-1);
	static CWSTRING convertToString(CWUINT i);

	static CWVOID ltrim(CWSTRING& str);
	static CWVOID rtrim(CWSTRING& str);
	static CWVOID trim(CWSTRING& str);

	static CWVOID ltrim(CWSTRING& str, CWCHAR c);
	static CWVOID rtrim(CWSTRING& str, CWCHAR c);
	static CWVOID trim(CWSTRING& str, CWCHAR c);

	static CWVOID split(CWSTRING& str, const CWSTRING& strSep, std::vector<CWSTRING>& vecRet);
	static CWSTRING replace(CWSTRING& strOrigin, const CWSTRING& strFind, const CWSTRING& strReplace);

	static CWBYTE parseHexStringByte(const CWSTRING& strHex);

protected:
};

NS_MINIR_END

#endif