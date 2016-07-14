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

#ifndef __CW_PU_BATCH_SHADER_PARAM_H__
#define __CW_PU_BATCH_SHADER_PARAM_H__

#include "Base/cwMacros.h"
#include "Base/cwUtils.h"
#include "../cwPUBatch.h"

NS_MINIR_BEGIN

class cwShader;

class cwPUBatchShaderParam : public cwPUBatch
{
public:
	cwPUBatchShaderParam();
	virtual ~cwPUBatchShaderParam();

	inline eShaderParamFreq getFreq() const { return m_eFreq; }
	inline eShaderParamSource getSourceType() const { return m_eSource; }
	inline eShaderParamType getType() const { return m_eType; }
	inline const CWSTRING& getShaderParamName() const { return m_nShaderParamName; }

	inline CWVOID setSourceType(eShaderParamSource eSource) { m_eSource = eSource; }
	inline CWVOID setFreq(eShaderParamFreq eFreq) { m_eFreq = eFreq; }
	inline CWVOID setShaderParamName(const CWSTRING& strName) { m_nShaderParamName = strName; }

	inline CWVOID setEngineParamType(const CWSTRING& str) { m_nEngineParamType = str; }
	inline CWVOID setEngineParamName(const CWSTRING& str) { m_nEngineParamName = str; }
	inline CWVOID setEngineParamAttr(const CWSTRING& str) { m_nEngineParamAttr = str; }

	inline const CWSTRING& getEngineParamType() const { return m_nEngineParamType; }
	inline const CWSTRING& getEngineParamName() const { return m_nEngineParamName; }
	inline const CWSTRING& getEngineParamAttr() const { return m_nEngineParamAttr; }

	virtual CWVOID reset() override;
	virtual CWVOID updateBatch(cwRenderBatch* pBatch) override;

protected:
	virtual CWVOID updateShaderParam() = 0;

protected:
	eShaderParamFreq m_eFreq;
	eShaderParamSource m_eSource;
	eShaderParamType m_eType;

	CWSTRING m_nShaderParamName;
	cwShader* m_pShader;

	CWSTRING m_nEngineParamType;
	CWSTRING m_nEngineParamName;
	CWSTRING m_nEngineParamAttr;

};

NS_MINIR_END

#endif