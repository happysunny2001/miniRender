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

#include "cwGPEffect.h"
#include "Entity/cwEntity.h"
#include "Material/cwMaterial.h"
#include "Device/cwDevice.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Render/cwRenderer.h"
#include "Render/cwRenderBatch.h"

NS_MINIR_BEGIN

cwGPEffect* cwGPEffect::create()
{
	cwGPEffect* pEffect = new cwGPEffect();
	if (pEffect) {
		pEffect->autorelease();
		return pEffect;
	}

	CW_SAFE_DELETE(pEffect);
	return nullptr;
}

cwGPEffect::cwGPEffect()
{

}

cwGPEffect::~cwGPEffect()
{

}

CWVOID cwGPEffect::render(cwRenderBatch* pBatch)
{
	if (!pBatch) return;

	//cwRepertory::getInstance().getEngine()->getRenderer()->setCurrShader(m_pShader);
	this->config();

	//cwMaterial* pMaterial = pBatch->m_pEntity->getMaterial();
	//if (pMaterial)
	//	pMaterial->configShader(m_pShader);

	cwDevice* pDevice = cwRepertory::getInstance().getDevice();
	pDevice->drawGP(m_pShader, m_strTech, &m_nGPInfo);
}

NS_MINIR_END
