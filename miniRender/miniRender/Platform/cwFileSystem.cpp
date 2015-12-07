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

#include "cwFileSystem.h"
#include "cwPlatform.h"

#include <stdio.h>

#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_
#include "Windows/cwWinFileSystem.h"

#include <io.h>
#include <stdlib.h>
#endif

NS_MINIR_BEGIN

cwData::cwData():
m_pData(nullptr),
m_uSize(0)
{

}

cwData::~cwData()
{
	CW_SAFE_DELETE_ARRAY(m_pData);
	m_uSize = 0;
}

cwFileSystem* cwFileSystem::create()
{
#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_
	return cwWinFileSystem::create();
#endif
	return nullptr;
}

CWSTRING cwFileSystem::getFullFilePath(const CWSTRING& strFileName) const
{
	return m_strWorkingPath + "/" + strFileName;
}

CWBOOL cwFileSystem::isFileExist(const CWSTRING& strFilePath)
{
#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_
	if (_access(strFilePath.c_str(), 0) == 0) return CWTRUE;
#endif
	return CWFALSE;
}

cwData* cwFileSystem::getFileData(const CWSTRING& strFilePath)
{
	FILE* fp = NULL;

#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_
	fopen_s(&fp, strFilePath.c_str(), "rb");
#endif

	if (!fp) {
		return nullptr;
	}

	cwData* pData = new cwData;
	if (!pData) {
		fclose(fp);
		return nullptr;
	}

	fseek(fp, 0, SEEK_END);
	auto len = ftell(fp);
	pData->m_pData = new CWBYTE[len];
	if (!pData->m_pData) {
		delete pData;
		fclose(fp);
		return nullptr;
	}

	fseek(fp, 0, SEEK_SET);
	fread(pData->m_pData, 1, len, fp);
	pData->m_uSize = len;

	fclose(fp);

	return pData;
}

NS_MINIR_END

