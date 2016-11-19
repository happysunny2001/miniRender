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

#include "cwEffectManager.h"
#include "effect/cwEffect.h"
#include "Repertory/cwRepertory.h"
#include "Shader/cwShaderManager.h"

NS_MINIR_BEGIN

cwEffectManager* cwEffectManager::create()
{
	cwEffectManager* pEffectManager = new cwEffectManager();
	if (pEffectManager) {
		pEffectManager->autorelease();
		return pEffectManager;
	}

	return nullptr;
}

cwEffectManager::cwEffectManager():
m_pDefaultSpriteEffect(nullptr)
{

}

cwEffectManager::~cwEffectManager()
{
	CW_SAFE_RELEASE_NULL(m_pDefaultSpriteEffect);
}

cwEffect* cwEffectManager::defaultSpriteEffect()
{
	if (m_pDefaultSpriteEffect == nullptr) {
		cwShader* pSpriteShader = cwRepertory::getInstance().getShaderManager()->createShader("SpriteRenderTechnique.hlsl");
		m_pDefaultSpriteEffect = cwEffect::create();
		m_pDefaultSpriteEffect->setShader(pSpriteShader);
		m_pDefaultSpriteEffect->setTech("SpriteRender");
		CW_SAFE_RETAIN(m_pDefaultSpriteEffect);
	}

	return m_pDefaultSpriteEffect;
}

cwEffect* cwEffectManager::getDefEffect(eDefEffectID effectID)
{
	auto it = m_nMapDefEffect.find(effectID);
	return it != m_nMapDefEffect.end() ? it->second : nullptr;
}

CWVOID cwEffectManager::loadDefEffect()
{
	cwShaderManager* pShaderManager = cwRepertory::getInstance().getShaderManager();

	cwShader* pShader = pShaderManager->getShader("colorTex.fx");
	{
		cwEffect* pEffectPosTexColor = cwEffect::create();
		pEffectPosTexColor->setShader(pShader);
		pEffectPosTexColor->setTech("TechPosTexColor");
		m_nMapDefEffect.insert(eEffectIDPosTexColor, pEffectPosTexColor);
	}

	{
		cwEffect* pEffectColor = cwEffect::create();
		pEffectColor->setShader(pShader);
		pEffectColor->setTech("TechPosColor");
		m_nMapDefEffect.insert(eEffectIDColor, pEffectColor);
	}
}

NS_MINIR_END
