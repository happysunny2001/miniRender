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

#ifndef __CW_SHADER_MANAGER_H__
#define __CW_SHADER_MANAGER_H__

#include "Base/cwMacros.h"
#include "Base/cwUtils.h"
#include "Base/cwMap.h"
#include "Ref/cwRef.h"
#include "Repertory/cwRepertory.h"
#include "cwShader.h"

#include <mutex>

NS_MINIR_BEGIN

class CW_DLL cwShaderManager : public cwRef
{
public:
	virtual ~cwShaderManager();

	virtual CWBOOL init();

	virtual CWVOID loadDefaultShader();

	cwShader* createShader(const CWSTRING& strFile);
	cwShader* createShaderThreadSafe(const CWSTRING& strFile);

	cwShader* getShader(const CWSTRING& strFile);
	cwShader* getDefShader(eDefShaderID eShaderID);

	cwShader* isExist(const CWSTRING& strFile);
	CWVOID appendShader(cwShader* pShader);

	CWVOID removeShader(cwShader* pShader);
	CWVOID removeShader(const CWSTRING& strName);

	const CWSTRING& getShaderParamString(eShaderParamIndex eParam) const;

protected:
	cwShaderManager();

	CWVOID buildShaderParam();

protected:
	cwMap<CWSTRING, cwShader*> m_nMapShader;
	cwMap<eDefShaderID, cwShader*> m_nMapDefShader;

	CWSTRING m_strShaderParam[eShaderParamMax];

	std::mutex m_nMutex;

};

NS_MINIR_END

#endif
