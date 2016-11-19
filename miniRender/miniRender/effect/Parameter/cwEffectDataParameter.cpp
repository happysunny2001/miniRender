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

#include "cwEffectDataParameter.h"

NS_MINIR_BEGIN

cwEffectDataParameter* cwEffectDataParameter::create()
{
	cwEffectDataParameter* pParameter = new cwEffectDataParameter();
	if (pParameter) {
		pParameter->autorelease();
		return pParameter;
	}

	return pParameter;
}

cwEffectDataParameter::cwEffectDataParameter() :
m_pData(nullptr),
m_uSize(0)
{

}

cwEffectDataParameter::~cwEffectDataParameter()
{
	CW_SAFE_FREE(m_pData);
}

CWVOID cwEffectDataParameter::binding(cwShader* pShader)
{
	if (pShader && m_pData) {
		pShader->setVariableData(m_nStrParamName, m_pData, 0, m_uSize);
	}
}

CWVOID cwEffectDataParameter::setData(CWVOID* pData, CWUINT uSize)
{
	if (pData && uSize > 0) {
		CW_SAFE_FREE(m_pData);
		m_pData = (CWVOID*)malloc(uSize);
		memcpy(m_pData, pData, uSize);
		m_uSize = uSize;
	}
}

NS_MINIR_END
