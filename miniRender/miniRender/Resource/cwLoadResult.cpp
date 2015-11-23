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

#include "cwLoadResult.h"
#include "cwLoadBatch.h"
#include "Repertory/cwRepertory.h"
#include "Texture/cwTextureManager.h"
#include "Shader/cwShaderManager.h"

NS_MINIR_BEGIN

cwLoadResult* cwLoadResult::create()
{
	cwLoadResult* pResult = new cwLoadResult();
	if (pResult) {
		return pResult;
	}

	return nullptr;
}

cwLoadResult::cwLoadResult():
m_pLoadBatch(nullptr)
{

}

cwLoadResult::~cwLoadResult()
{
	CW_SAFE_RELEASE_NULL(m_pLoadBatch);
}

CWVOID cwLoadResult::setLoadBatch(cwLoadBatch* pBatch)
{
	if (m_pLoadBatch == pBatch) return;
	CW_SAFE_RETAIN(pBatch);
	CW_SAFE_RELEASE_NULL(m_pLoadBatch);
	m_pLoadBatch = pBatch;
}

CWVOID cwLoadResult::distribute()
{
	cwRepertory& repertory = cwRepertory::getInstance();
	cwTextureManager* pTexManager = repertory.getTextureManager();

	for (auto pTex : m_nVecTexture) {
		pTexManager->appendTexture(pTex);
		CW_SAFE_RELEASE(pTex);
	}

	cwShaderManager* pShaderManager = repertory.getShaderManager();
	for (auto pShader : m_nVecShader) {
		pShaderManager->appendShader(pShader);
		CW_SAFE_RELEASE(pShader);
	}
}

CWVOID cwLoadResult::load()
{
	if (!m_pLoadBatch) return;

	for (auto it = m_pLoadBatch->begin(); it != m_pLoadBatch->end(); ++it) {
		switch (it->m_eType)
		{
		case eResourceTypeTexture2D:
			loadTexture2D(*it);
			break;
		case eResourceTypeTextureCube:
			loadTextureCubeMap(*it);
			break;
		case eResourceTypeShader:
			loadTextureShader(*it);
			break;
		default:
			break;
		}
	}
}

CWVOID cwLoadResult::loadTexture2D(const cwResourceInfo& resInfo)
{
	cwTexture* pTex = cwRepertory::getInstance().getTextureManager()->createTextureThreadSafe(resInfo.m_nStrName);
	if (pTex) {
		m_nVecTexture.push_back(pTex);
	}
}

CWVOID cwLoadResult::loadTextureCubeMap(const cwResourceInfo& resInfo)
{
	cwTexture* pTex = cwRepertory::getInstance().getTextureManager()->createCubeTextureThreadSafe(resInfo.m_nStrName);
	if (pTex) {
		m_nVecTexture.push_back(pTex);
	}
}

CWVOID cwLoadResult::loadTextureShader(const cwResourceInfo& resInfo)
{
	cwShader* pShader = cwRepertory::getInstance().getShaderManager()->createShaderThreadSafe(resInfo.m_nStrName);
	if (pShader) {
		m_nVecShader.push_back(pShader);
	}
}

NS_MINIR_END
