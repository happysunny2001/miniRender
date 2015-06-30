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

#include <windows.h>
#include "cwWinFileSystem.h"
#include "Base/cwStringConvert.h"

NS_MINIR_BEGIN

cwWinFileSystem* cwWinFileSystem::create()
{
	cwWinFileSystem* pFileSystem = new cwWinFileSystem();
	if (pFileSystem && pFileSystem->init()) {
		pFileSystem->autorelease();
		return pFileSystem;
	}

	CW_SAFE_DELETE(pFileSystem);
	return nullptr;
}

cwWinFileSystem::cwWinFileSystem()
{

}

cwWinFileSystem::~cwWinFileSystem()
{

}

bool cwWinFileSystem::init()
{
	buildModulePath();
	return true;
}

void cwWinFileSystem::buildModulePath()
{
	TCHAR szBuffer[MAX_PATH];
	GetModuleFileName(NULL, szBuffer, MAX_PATH);
	TCHAR* pLastSlash = wcsrchr(szBuffer, L'\\');
	if (pLastSlash) {
		*pLastSlash = L'\0';
	}

	replaceWindowsStyleSlash(szBuffer);
	m_strWorkingPath = cwStringConvert::convertToMultiByte(szBuffer);
}

void cwWinFileSystem::replaceWindowsStyleSlash(TCHAR* pszChar)
{
	int index = 0;
	while (pszChar[index] != L'\0') {
		if (pszChar[index] == L'\\')
			pszChar[index] = L'/';
		index++;
	}
}

NS_MINIR_END

