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

#ifndef __CW_FILE_SYSTEM_H__
#define __CW_FILE_SYSTEM_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Ref/cwRef.h"
#include "Repertory/cwRepertory.h"

NS_MINIR_BEGIN

class cwData
{
public:
	cwData();
	~cwData();

public:
	CWBYTE* m_pData;
	CWUINT64 m_uSize;

};

class CW_DLL cwFileSystem : public cwRef
{
public:
	virtual ~cwFileSystem() {}

	virtual bool init() = 0;

	const CWSTRING& getWokringPath() const { return m_strWorkingPath; }
	CWSTRING getFullFilePath(const CWSTRING& strFileName) const;
	cwData* getFileData(const CWSTRING& strFilePath);

	CWBOOL isFileExist(const CWSTRING& strFilePath);

protected:
	static cwFileSystem* create();
	cwFileSystem() {}

	friend class cwRepertory;

protected:
	CWSTRING m_strWorkingPath;

};

NS_MINIR_END

#endif

