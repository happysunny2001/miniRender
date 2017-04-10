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

#include "cwEffectFloatVectorArrayParameter.h"
#include "Shader/cwShader.h"

NS_MINIR_BEGIN

cwEffectFloatVectorArrayParameter* cwEffectFloatVectorArrayParameter::create()
{
	cwEffectFloatVectorArrayParameter* pParameter = new cwEffectFloatVectorArrayParameter();
	if (pParameter) {
		pParameter->autorelease();
		return pParameter;
	}

	return nullptr;
}

cwEffectFloatVectorArrayParameter::cwEffectFloatVectorArrayParameter() :
m_pVector(nullptr),
m_iCnt(0)
{

}

cwEffectFloatVectorArrayParameter::~cwEffectFloatVectorArrayParameter()
{
	CW_SAFE_DELETE_ARRAY(m_pVector);
	m_iCnt = 0;
}

CWVOID cwEffectFloatVectorArrayParameter::setVectorArray(cwVector4D* pVec, CWUINT iCnt)
{
	CW_SAFE_DELETE_ARRAY(m_pVector);
	m_iCnt = 0;

	if (pVec && iCnt > 0) {
		m_pVector = new cwVector4D[iCnt];
		if (m_pVector) {
			memcpy_s(m_pVector, sizeof(cwVector4D)*iCnt, pVec, sizeof(cwVector4D)*iCnt);
			m_iCnt = iCnt;
		}
	}
}

CWVOID cwEffectFloatVectorArrayParameter::binding(cwShader* pShader)
{
	if (pShader && m_pVector && m_iCnt > 0) {
		pShader->setVariableFloatVectorArray(m_nStrParamName, reinterpret_cast<CWFLOAT*>(m_pVector), m_iCnt);
	}
}

NS_MINIR_END
