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

#include "cwEffectBufferParameter.h"
#include "Buffer/cwBuffer.h"

NS_MINIR_BEGIN

cwEffectBufferParameter* cwEffectBufferParameter::create()
{
	cwEffectBufferParameter* pParameter = new cwEffectBufferParameter();
	if (pParameter) {
		pParameter->autorelease();
		return pParameter;
	}

	return nullptr;
}

cwEffectBufferParameter::cwEffectBufferParameter() :
m_pBuffer(nullptr),
m_bWritable(CWFALSE)
{

}

cwEffectBufferParameter::~cwEffectBufferParameter()
{
	CW_SAFE_RELEASE_NULL(m_pBuffer);
}

CWVOID cwEffectBufferParameter::setBuffer(cwBuffer* pBuffer)
{
	CW_SAFE_RETAIN(pBuffer);
	CW_SAFE_RELEASE_NULL(m_pBuffer);
	m_pBuffer = pBuffer;
}

CWVOID cwEffectBufferParameter::binding(cwShader* pShader)
{
	if (pShader) {
		if (!m_bWritable) {
			pShader->setVariableBuffer(m_nStrParamName, m_pBuffer);
		}
		else {
			pShader->setVariableBufferWritable(m_nStrParamName, m_pBuffer);
		}
	}
}

NS_MINIR_END
