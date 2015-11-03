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

#include "cwReflectionEntity.h"
#include "Material/cwMaterial.h"
#include "Material/MaterialUnit/cwMaterialUnitReflect.h"
#include "Shader/cwShaderConstant.h"
#include "Shader/cwShaderManager.h"
#include "Repertory/cwRepertory.h"
#include "effect/cwEffect.h"

NS_MINIR_BEGIN

cwReflectionEntity* cwReflectionEntity::create()
{
	cwReflectionEntity* pEntity = new cwReflectionEntity();
	if (pEntity && pEntity->init()) {
		pEntity->autorelease();
		return pEntity;
	}

	CW_SAFE_DELETE(pEntity);
	return nullptr;
}

cwReflectionEntity::cwReflectionEntity():
m_pMatUnitDynReflection(nullptr)
{
	m_eRenderType = eRenderTypeReflection;
}

cwReflectionEntity::~cwReflectionEntity()
{

}

CWBOOL cwReflectionEntity::init()
{
	if (!cwEntity::init()) return CWFALSE;

	buildEffect();

	return CWTRUE;
}

CWVOID cwReflectionEntity::setDynamicRelfectionTexture(cwTexture* pTexture)
{
	if (m_pMatUnitDynReflection) {
		m_pMatUnitDynReflection->setReflectionTexture(pTexture);
	}
}

CWVOID cwReflectionEntity::setDynamicRelfectionFactor(CWFLOAT f)
{
	if (m_pMatUnitDynReflection) {
		m_pMatUnitDynReflection->setReflectionFactor(f);
	}
}

CWVOID cwReflectionEntity::buildMaterial()
{
	cwEntity::buildMaterial();

	m_pMatUnitDynReflection = cwMaterialUnitReflect::create();
	if (m_pMatUnitDynReflection) {
		m_pMatUnitDynReflection->setTextureParamName(CW_SHADER_REFLECT_CUBE_MAP);
		m_pMatUnitDynReflection->setFactorParamName(CW_SHADER_REFLECT_FACTOR);
		m_pMatUnitDynReflection->setReflectionFactor(0.5f);

		if (m_pMaterial) {
			m_pMaterial->addMaterialUnit(m_pMatUnitDynReflection);
		}
	}
}

CWVOID cwReflectionEntity::buildEffect()
{
	cwShader* pShader = cwRepertory::getInstance().getShaderManager()->getDefShader(eDefShaderLighting);
	cwEffect* pEffect = cwEffect::create();
	pEffect->setShader(pShader);
	pEffect->setTech("LightTechReflect");

	this->setEffect(pEffect);
}

NS_MINIR_END
