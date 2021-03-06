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

#ifndef __CW_COMMON_H__
#define __CW_COMMON_H__

#include "cwMacros.h"
#include "cwBasicType.h"
#include "cwUtils.h"

#include <unordered_map>

NS_MINIR_BEGIN

class cwCommon
{
public:
	static CWSTRING getNewTextureID();
	static cwCommon& getInstance();

	CWBOOL getBool(const CWSTRING& strBool) const;
	eColorWriteEnable getColorEnable(const CWSTRING& strColor) const;
	eFormat getFormatType(const CWSTRING& strFormat) const;
	eClassification getClassificationType(const CWSTRING& strClass) const;

	CWBOOL getBool(const char* strBool) const;
	eColorWriteEnable getColorEnable(const char* strColor) const;
	eFormat getFormatType(const char* strFormat) const;
	eClassification getClassificationType(const char* strClass) const;

private:
	cwCommon();
	cwCommon(cwCommon const&) {}
	cwCommon& operator=(cwCommon const&) {}
	virtual ~cwCommon();

	CWVOID initFormat();

private:
	static CWUINT m_iGTextureID;

	std::unordered_map<CWSTRING, CWBOOL> m_nMapBool;
	std::unordered_map<CWSTRING, eColorWriteEnable> m_nMapColorEnable;
	std::unordered_map<CWSTRING, eFormat> m_nMapFormat;
	std::unordered_map<CWSTRING, eClassification> m_nMapClassification;

};

NS_MINIR_END

#endif
