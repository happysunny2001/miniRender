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

#include "cwTBDRCoreStageLayer.h"
#include "Repertory/cwRepertory.h"
#include "Engine/cwEngine.h"
#include "Entity/cwScene.h"
#include "Camera/cwCamera.h"
#include "Shader/cwShader.h"
#include "Shader/cwShaderConstant.h"
#include "Device/cwDevice.h"

NS_MINIR_BEGIN

cwTBDRCoreStageLayer::cwTBDRCoreStageLayer() :
m_pViewSpaceDirectionalLight(nullptr),
m_pPointLightBuffer(nullptr)
{

}

cwTBDRCoreStageLayer::~cwTBDRCoreStageLayer()
{
	CW_SAFE_RELEASE_NULL(m_pViewSpaceDirectionalLight);
	CW_SAFE_RELEASE_NULL(m_pPointLightBuffer);
}

CWBOOL cwTBDRCoreStageLayer::init()
{
	if (!cwStageLayer::init()) return CWFALSE;
	if (!buildDirectionalLight()) return CWFALSE;
	if (!buildPointLightBuffer()) return CWFALSE;

	return CWTRUE;
}

CWVOID cwTBDRCoreStageLayer::preFrameConfig(cwShader* pShader)
{
	cwStageLayer::preFrameConfig(pShader);

	if (pShader) {
		pShader->setVariableMatrix("gMatViewInv", m_pCamera->getInvertViewMatrix());
	}
}

CWBOOL cwTBDRCoreStageLayer::buildDirectionalLight()
{
	m_pViewSpaceDirectionalLight = cwDirectionalLight::create(cwVector4D::ZERO, cwVector3D::ZERO);
	CW_SAFE_RETAIN(m_pViewSpaceDirectionalLight);

	return CWTRUE;
}

CWBOOL cwTBDRCoreStageLayer::buildPointLightBuffer()
{
	m_pPointLightBuffer = cwRepertory::getInstance().getDevice()->createShaderStructedBuffer(nullptr, sizeof(cwPointLight::PointLight_Struct), CW_MAX_LIGHTS);
	if (!m_pPointLightBuffer) return CWFALSE;
	CW_SAFE_RETAIN(m_pPointLightBuffer);

	m_nVecViewSpacePointLights.resize(CW_MAX_LIGHTS);

	return CWTRUE;
}

CWVOID cwTBDRCoreStageLayer::configDirectionalLight(cwScene* pScene, cwShader* pShader)
{
	cwScene* pCurrScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	const cwMatrix4X4& matView = m_pCamera->getViewMatrix();

	CWUINT iEnableDirectional = 0;
	cwDirectionalLight* pLight = pCurrScene->getDirectionalLight();
	if (!pLight) {
		pShader->setVariableInt(CW_SHADER_DIRECTIONAL_LIGHT_ENABLE, iEnableDirectional);
		return;
	}

	m_pViewSpaceDirectionalLight->getData().m_nColor = pLight->getData().m_nColor;
	m_pViewSpaceDirectionalLight->getData().m_fAttr = pLight->getData().m_fAttr;
	m_pViewSpaceDirectionalLight->getData().m_nDirection = pLight->getData().m_nDirection*matView;

	iEnableDirectional = 1;
	pShader->setVariableInt(CW_SHADER_DIRECTIONAL_LIGHT_ENABLE, iEnableDirectional);
	pShader->setVariableData(CW_SHADER_DIRECTIONAL_LIGHT, m_pViewSpaceDirectionalLight->data(), 0, m_pViewSpaceDirectionalLight->size());
}

CWVOID cwTBDRCoreStageLayer::configPointLight(cwScene* pScene, cwShader* pShader)
{
	cwScene* pCurrScene = cwRepertory::getInstance().getEngine()->getCurrScene();
	const cwMatrix4X4& matView = m_pCamera->getViewMatrix();
	const cwVector<cwPointLight*>& vecLight = pCurrScene->getPointLights();

	CWUINT index = 0;
	for (auto it = vecLight.begin(); it != vecLight.end(); ++it) {
		m_nVecViewSpacePointLights[index] = (*it)->getData();
		cwVector4D v(m_nVecViewSpacePointLights[index].m_nPosition, 1.0f);
		m_nVecViewSpacePointLights[index].m_nPosition = v*matView;

		index++;
		if (index >= CW_MAX_LIGHTS) break;
	}

	m_pPointLightBuffer->refresh(&(m_nVecViewSpacePointLights[0]));
	pShader->setVariableBuffer(CW_SHADER_POINT_LIGHTS, m_pPointLightBuffer);
	pShader->setVariableInt(CW_SHADER_POINT_LIGHTS_CNT, index);
}

NS_MINIR_END
