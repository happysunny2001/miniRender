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

#include "cwEffectTextureParameter.h"
//#include "Texture/cwRenderTexture.h"

NS_MINIR_BEGIN

cwEffectTextureParameter* cwEffectTextureParameter::create()
{
	cwEffectTextureParameter* pEffectParameter = new cwEffectTextureParameter();
	if (pEffectParameter) {
		pEffectParameter->autorelease();
		return pEffectParameter;
	}

	return nullptr;
}

cwEffectTextureParameter::cwEffectTextureParameter():
m_pTexture(nullptr),
m_bWritable(CWFALSE)
{

}

cwEffectTextureParameter::~cwEffectTextureParameter()
{
	CW_SAFE_RELEASE_NULL(m_pTexture);
}

CWVOID cwEffectTextureParameter::binding(cwShader* pShader)
{
	if (pShader) {
		if (!m_bWritable)
			pShader->setVariableTexture(m_nStrParamName, m_pTexture);
		else
			pShader->setVariableTextureWritable(m_nStrParamName, m_pTexture);
	}
}

CWVOID cwEffectTextureParameter::setTexture(cwTexture* pTexture)
{
	CW_SAFE_RETAIN(pTexture);
	CW_SAFE_RELEASE_NULL(m_pTexture);
	m_pTexture = pTexture;
}

NS_MINIR_END
