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

#include "cwPUBatchShaderParamTexture.h"
#include "Texture/cwTexture.h"
#include "Texture/cwTextureManager.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Render/cwRenderer.h"
#include "Render/Stage/cwStage.h"
#include "Shader/cwShader.h"

NS_MINIR_BEGIN

cwPUBatchShaderParamTexture* cwPUBatchShaderParamTexture::create()
{
	cwPUBatchShaderParamTexture* pPUBatch = new cwPUBatchShaderParamTexture();
	if (pPUBatch) {
		pPUBatch->autorelease();
	}

	return pPUBatch;
}

cwPUBatchShaderParamTexture::cwPUBatchShaderParamTexture() :
m_pTexture(nullptr)
{

}

cwPUBatchShaderParamTexture::~cwPUBatchShaderParamTexture()
{
	CW_SAFE_RELEASE_NULL(m_pTexture);
}

CWVOID cwPUBatchShaderParamTexture::setTextureName(const CWSTRING& strFileName)
{
	CW_SAFE_RELEASE_NULL(m_pTexture);
	m_pTexture = cwRepertory::getInstance().getTextureManager()->createTexture(strFileName);
	CW_SAFE_RETAIN(m_pTexture);
}

const cwTexture* cwPUBatchShaderParamTexture::getTexture() const
{
	if (m_eSource == eShaderParamSourceData)
		return m_pTexture;

	if (m_nEngineParamType == "StageTexture")
		return getStageTexture();

	return nullptr;
}

const cwTexture* cwPUBatchShaderParamTexture::getStageTexture() const
{
	cwStage* pStage = cwRepertory::getInstance().getEngine()->getRenderer()->getStage(m_nEngineParamName);
	if (pStage) {
		return pStage->getStageTexture(m_nEngineParamAttr);
	}

	return nullptr;
}

CWVOID cwPUBatchShaderParamTexture::updateShaderParam()
{
	if (m_pShader && !m_nShaderParamName.empty()) {
		const cwTexture* pTexture = getTexture();
		if (pTexture) {
			m_pShader->setVariableTexture(m_nShaderParamName, pTexture);
		}
	}
}

NS_MINIR_END
