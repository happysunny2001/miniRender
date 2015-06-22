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
#include <windows.h>
#include <sstream>
using namespace std;

NS_MINI_BEGIN

CWWSTRING cwStringConvert::convert(const string& str)
{
	int iSize = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wpcStr = new wchar_t[iSize];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wpcStr, iSize);
	CWWSTRING wstr(wpcStr);
	delete[] wpcStr;
	return wstr;
}

CWWSTRING cwStringConvert::createFPSString(const CWWSTRING& title, CWUINT iFPS)
{
	std::wostringstream outs;
	outs << title << L" FPS:" << iFPS;
	return outs.str();
}

NS_MINI_END