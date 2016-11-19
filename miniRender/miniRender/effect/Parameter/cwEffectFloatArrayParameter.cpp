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

#include "cwEffectFloatArrayParameter.h"

NS_MINIR_BEGIN

cwEffectFloatArrayParameter* cwEffectFloatArrayParameter::create()
{
	cwEffectFloatArrayParameter* pEffectParameter = new cwEffectFloatArrayParameter();
	if (pEffectParameter) {
		pEffectParameter->autorelease();
		return pEffectParameter;
	}

	return nullptr;
}

cwEffectFloatArrayParameter::cwEffectFloatArrayParameter():
m_pFloatArray(NULL),
m_iCnt(0)
{

}

cwEffectFloatArrayParameter::~cwEffectFloatArrayParameter()
{
	CW_SAFE_DELETE_ARRAY(m_pFloatArray);
	m_iCnt = 0;
}

CWVOID cwEffectFloatArrayParameter::binding(cwShader* pShader)
{
	//pShader->setVariableFloatArray(m_nStrParamName, m_pFloatArray, m_iCnt);
	if (m_pFloatArray && m_iCnt > 0) {
		pShader->setVariableData(m_nStrParamName, m_pFloatArray, 0, sizeof(CWFLOAT)*m_iCnt);
	}
}

CWVOID cwEffectFloatArrayParameter::setFloatArray(CWFLOAT* pFloat, CWUINT iCnt)
{
	if (pFloat && iCnt > 0) {
		m_pFloatArray = new CWFLOAT[iCnt];
		memcpy(m_pFloatArray, pFloat, sizeof(CWFLOAT)*iCnt);
		m_iCnt = iCnt;
	}
}

NS_MINIR_END
