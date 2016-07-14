/*
Copyright © 2015-2016 Ziwei Wang

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

#ifndef __CW_PARSER_MANAGER_H__
#define __CW_PARSER_MANAGER_H__

#include "Base/cwMacros.h"
#include "Base/cwUtils.h"
#include "Ref/cwRef.h"
#include "Math/cwMatrix4X4.h"

#include <unordered_map>

NS_MINIR_BEGIN

class cwParserManager : public cwRef
{
public:
	static cwParserManager* create();

	cwParserManager();
	virtual~ cwParserManager();

	virtual CWBOOL init();
	inline cwRef* getParser(eParserType eType) { return m_nArrParser[eType]; }

	CWBOOL getBool(const CWSTRING& strBool) const;
	eColorWriteEnable getColorEnable(const CWSTRING& strColor) const;
	eFormat getFormatType(const CWSTRING& strFormat) const;
	eClassification getClassificationType(const CWSTRING& strClass) const;

	CWBOOL getBool(const char* strBool) const;
	eColorWriteEnable getColorEnable(const char* strColor) const;
	eFormat getFormatType(const char* strFormat) const;
	eClassification getClassificationType(const char* strClass) const;
	eShaderParamType getShaderParamType(const char* strType) const;
	eShaderParamSource getShaderParamSource(const char* strSource) const;
	eShaderParamFreq getShaderParamFreq(const char* strFreq) const;

	cwMatrix4X4 parseMatrixFromString(const char* pcData);

protected:
	CWVOID initFormat();

protected:
	cwRef* m_nArrParser[eParserTypeMax];

	std::unordered_map<CWSTRING, CWBOOL> m_nMapBool;
	std::unordered_map<CWSTRING, eColorWriteEnable> m_nMapColorEnable;
	std::unordered_map<CWSTRING, eFormat> m_nMapFormat;
	std::unordered_map<CWSTRING, eClassification> m_nMapClassification;
	std::unordered_map<CWSTRING, eShaderParamType> m_nMapShaderParamType;
	std::unordered_map<CWSTRING, eShaderParamSource> m_nMapShaderParamSource;
	std::unordered_map<CWSTRING, eShaderParamFreq> m_nMapShaderParamFreq;

};

NS_MINIR_END

#endif
