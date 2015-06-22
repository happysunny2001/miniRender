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

#include "cwEntity.h"
#include "cwRenderObject.h"
#include "cwRenderDevice.h"
#include "cwRepertory.h"
#include "cwMaterial.h"
#include "cwLight.h"
#include "cwEffects.h"

NS_MINI_BEGIN

cwEntity* cwEntity::create()
{
	cwEntity* pEntity = new cwEntity();
	if (pEntity && pEntity->init()) {
		pEntity->autorelease();
		return pEntity;
	}

	CW_SAFE_DELETE(pEntity);
	return nullptr;
}

bool cwEntity::init()
{
	return true;
}

cwEntity::cwEntity():
m_pRenderObj(nullptr),
m_nPos(0, 0, 0),
m_nRot(0, 0, 0),
m_nScale(1.0f, 1.0f, 1.0f),
m_pMaterial(nullptr)
{
	m_nDiffTextureTrans = cwVector2D(0.0f, 0.0f);
	m_nDiffTextureScale = cwVector2D(1.0f, 1.0f);
}

cwEntity::~cwEntity()
{
	CW_SAFE_RELEASE_NULL(m_pRenderObj);
	CW_SAFE_RELEASE_NULL(m_pMaterial);
}

void cwEntity::setRenderObject(cwRenderObject* pRenderObj)
{
	CW_SAFE_RELEASE_NULL(m_pRenderObj);
	m_pRenderObj = pRenderObj;
	CW_SAFE_RETAIN(m_pRenderObj);
}

void cwEntity::setPosition(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	m_nPos.x = x;
	m_nPos.y = y;
	m_nPos.z = z;
}

void cwEntity::setPosition(const cwVector3D& v)
{
	setPosition(v.x, v.y, v.z);
}

void cwEntity::move(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	setPosition(m_nPos.x + x, m_nPos.y + y, m_nPos.z + z);
}

void cwEntity::move(const cwVector3D& v)
{
	move(v.x, v.y, v.z);
}

void cwEntity::setRotation(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	m_nRot.x = x;
	m_nRot.y = y;
	m_nRot.z = z;
}

void cwEntity::setRotation(const cwVector3D& v)
{
	setRotation(v.x, v.y, v.z);
}

void cwEntity::rotate(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	setRotation(m_nRot.x + x, m_nRot.y + y, m_nRot.z + z);
}

void cwEntity::rotate(const cwVector3D& v)
{
	rotate(v.x, v.y, v.z);
}

void cwEntity::setScale(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	m_nScale.x = x;
	m_nScale.y = y;
	m_nScale.z = z;
}

void cwEntity::setScale(const cwVector3D& v)
{
	setScale(v.x, v.y, v.z);
}

void cwEntity::scale(CWFLOAT x, CWFLOAT y, CWFLOAT z)
{
	setScale(m_nScale.x + x, m_nScale.y + y, m_nScale.z + z);
}

void cwEntity::scale(const cwVector3D& v)
{
	scale(v.x, v.y, v.z);
}

void cwEntity::setMaterial(cwMaterial* pMaterial)
{
	CW_SAFE_RELEASE_NULL(m_pMaterial);
	m_pMaterial = pMaterial;
	CW_SAFE_RETAIN(m_pMaterial);
}

void cwEntity::setDiffuseTextureTrans(const cwMatrix4X4& trans)
{
	m_nDiffuseTrans = trans;
}

void cwEntity::moveDiffuseTexture(CWFLOAT x, CWFLOAT y)
{
	m_nDiffTextureTrans.x = x;
	m_nDiffTextureTrans.y = y;
	updateDiffuseTexture();
}

void cwEntity::scaleDiffuseTexture(CWFLOAT x, CWFLOAT y)
{
	m_nDiffTextureScale.x = x;
	m_nDiffTextureScale.y = y;
	updateDiffuseTexture();
}

void cwEntity::updateDiffuseTexture()
{
	cwMatrix4X4 matTrans;
	cwMatrix4X4 matScale;

	matTrans.setTranslation(m_nDiffTextureTrans.x, m_nDiffTextureTrans.y, 0.0f);
	matScale.setScale(m_nDiffTextureScale.x, m_nDiffTextureScale.y, 1.0f);;
	m_nDiffuseTrans = matScale * matTrans;
}

void cwEntity::transform()
{
	cwMatrix4X4 matTranslate, matScale, matRot;
	matTranslate.setTranslation(m_nPos);
	matScale.setScale(m_nScale);
	matRot.setRotation(m_nRot);
	m_nTrans = matScale * matRot * matTranslate;
}

void cwEntity::renderSelf()
{
	cwMaterial* pMaterial = this->getMaterial();
	if (pMaterial) {
		pMaterial->configEffect();

		cwEffects *pEffect = pMaterial->getEffect();
		if (pEffect) {
			this->transform();
			cwRepertory::getInstance().getDevice()->setEffectWorldTrans(pEffect, this->getWorldTrans(), pCamera);

			const cwMatrix4X4& matDiffTrans = this->getDiffuseTrans();
			cwRepertory::getInstance().getDevice()->setDiffuseTrans(pEffect, matDiffTrans);

			cwRenderObject* pRenderObj = this->getRenderObj();
			cwRepertory::getInstance().getDevice()->draw(pEffect, pMaterial->getTechName(), pRenderObj);
		}
	}
}

// void cwEntity::setLights(vector<cwLight*>& vecLights)
// {
// 	if (m_pMaterial) {
// 		cwEffects* pEffect = m_pMaterial->getEffect();
// 		if (pEffect) {
// 			pEffect->setLights(vecLights);
// 		}
// 	}
// }

// void cwEntity::render(cwCamera* pCamera)
// {
// 	cwMaterial* pMaterial = this->getMaterial();
// 	if (pMaterial) {
// 		pMaterial->configEffect();
// 
// 		cwEffects *pEffect = pMaterial->getEffect();
// 		if (pEffect) {
// 			this->transform();
// 			cwRepertory::getInstance().getDevice()->setEffectWorldTrans(pEffect, this->getWorldTrans(), pCamera);
// 
// 			const cwMatrix4X4& matDiffTrans = this->getDiffuseTrans();
// 			cwRepertory::getInstance().getDevice()->setDiffuseTrans(pEffect, matDiffTrans);
// 
// 			cwRenderObject* pRenderObj = this->getRenderObj();
// 			cwRepertory::getInstance().getDevice()->draw(pEffect, pMaterial->getTechName(), pRenderObj);
// 		}
// 	}
// }

NS_MINI_END