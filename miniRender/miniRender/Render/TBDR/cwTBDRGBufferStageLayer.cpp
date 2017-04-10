/*
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

#include "cwTBDRGBufferStageLayer.h"
#include "cwTBDRConstants.h"
#include "Texture/cwTexture.h"
#include "Texture/cwMultiRTTexture.h"
#include "Shader/cwShader.h"

NS_MINIR_BEGIN

CWVOID cwTBDRGBufferStageLayer::bindingResultParameter(cwShader* pShader)
{
	if (pShader && m_pRenderTarget && m_pDepthStencil) {
		cwMultiRTTexture* pGBuffer = reinterpret_cast<cwMultiRTTexture*>(m_pRenderTarget);

		pShader->setVariableTexture(CW_TBDR_SHADER_PARAM_NORMAL_TEXTURE, pGBuffer->getTexture(0));
		pShader->setVariableTexture(CW_TBDR_SHADER_PARAM_DIFFUSE_TEXTURE, pGBuffer->getTexture(1));
		pShader->setVariableTexture(CW_TBDR_SHADER_PARAM_SPECULAR_TEXTURE, pGBuffer->getTexture(2));
		pShader->setVariableTexture(CW_TBDR_SHADER_PARAM_DEPTH_TEXTURE, m_pDepthStencil);
	}
}

NS_MINIR_END
