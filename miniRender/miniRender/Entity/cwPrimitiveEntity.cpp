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

#include "cwPrimitiveEntity.h"
#include "Material/cwMaterial.h"
#include "RenderObject/cwRenderObject.h"
#include "RenderObject/cwDynamicRenderObject.h"
#include "Effect/cwEffect.h"
#include "effect/cwEffectManager.h"
#include "Repertory/cwRepertory.h"
#include "Shader/cwShaderManager.h"
//#include "Render/cwRenderBatch.h"
#include "Device/cwDevice.h"

NS_MINIR_BEGIN

CWUINT cwPrimitiveEntity::uMaxVertexCnt = 102400;

cwPrimitiveEntity* cwPrimitiveEntity::create()
{
	cwPrimitiveEntity* pEntity = new cwPrimitiveEntity();
	if (pEntity && pEntity->init()) {
		pEntity->autorelease();
		return pEntity;
	}

	CW_SAFE_DELETE(pEntity);
	return nullptr;
}

cwPrimitiveEntity::cwPrimitiveEntity():
m_pVertexData(nullptr),
m_uVertexCnt(0)
{
	m_eRenderType = eRenderTypePrimitiveEntity;
	m_bCastShadow = CWFALSE;
}

cwPrimitiveEntity::~cwPrimitiveEntity()
{
	CW_SAFE_DELETE_ARRAY(m_pVertexData);
	m_uVertexCnt = 0;
}

CWBOOL cwPrimitiveEntity::init()
{
	if (!cwEntity::init()) return CWFALSE;

	buildMaterial();
	buildRenderObject();
	buildEffect();

	this->setPosition(cwVector3D::ZERO);

	return CWTRUE;
}

//CWVOID cwPrimitiveEntity::render(cwRenderBatch* pRenderBatch)
//{
//	if (m_uVertexCnt == 0) return;
//	m_pRenderObj->updateVertexData(m_pVertexData, m_uVertexCnt);
//
//	if (pRenderBatch && pRenderBatch->m_pEffect) {
//		cwDevice* pDevice = cwRepertory::getInstance().getDevice();
//		pDevice->draw(pRenderBatch->m_pEffect->getShader(), pRenderBatch->m_pEffect->getTech(), m_pRenderObj);
//	}
//
//	m_uVertexCnt = 0;
//}

CWVOID cwPrimitiveEntity::render(cwEffect* pEffect)
{
	if (m_uVertexCnt == 0) return;
	m_pRenderObj->updateVertexData(m_pVertexData, m_uVertexCnt);

	if (pEffect) {
		cwDevice* pDevice = cwRepertory::getInstance().getDevice();
		//pDevice->draw(pEffect->getShader(), pEffect->getTech(), m_pRenderObj);
		pDevice->draw(pEffect, m_pRenderObj);
	}

	m_uVertexCnt = 0;
}

CWVOID cwPrimitiveEntity::buildMaterial()
{
	this->setMaterial(cwMaterial::create());
}

CWVOID cwPrimitiveEntity::buildEffect()
{
	//cwRepertory& repertory = cwRepertory::getInstance();
	//cwShader* pShader = repertory.getShaderManager()->getDefShader(eDefShaderColor);
	//cwEffect* pEffect = cwEffect::create();
	//pEffect->setShader(pShader);

	cwEffect* pEffect = cwRepertory::getInstance().getEffectManager()->getDefEffect(eEffectIDColor);

	this->setEffect(pEffect);
}

CWVOID cwPrimitiveEntity::buildRenderObject()
{
	m_pVertexData = new cwVertexPosColor[uMaxVertexCnt];
	m_uVertexCnt = 0;

	m_pRenderObj = cwDynamicRenderObject::create(ePrimitiveTypeLineList, m_pVertexData, sizeof(cwVertexPosColor), uMaxVertexCnt, NULL, 0, "PosColor");
	CW_SAFE_RETAIN(m_pRenderObj);
}

CWVOID cwPrimitiveEntity::refreshSpatialNode()
{

}

CWVOID cwPrimitiveEntity::addPrimitive(const cwAABB& aabb)
{
	addPrimitive(aabb, m_pMaterial->getDiffuse());
}

CWVOID cwPrimitiveEntity::addPrimitive(const cwAABB& aabb, const cwVector4D& color)
{
	static CWUINT uIndex[24] = { 0, 1, 1, 3, 3, 2, 2, 0, 4, 5, 5, 7, 7, 6, 6, 4, 3, 7, 1, 5, 0, 4, 2, 6 };

	if (m_uVertexCnt + 24 > uMaxVertexCnt) return;

	cwVector3D points[8];

	for (CWUINT i = 0; i < 8; ++i)
		points[i] = aabb.corner(i);

	for (CWUINT i = 0; i < 24; ++i) {
		m_pVertexData[m_uVertexCnt + i].pos = points[uIndex[i]];
		m_pVertexData[m_uVertexCnt + i].color = color;
	}

	m_uVertexCnt += 24;
}

CWVOID cwPrimitiveEntity::addPrimitive(const cwRay& ray)
{
	addPrimitive(ray, 100.0f, m_pMaterial->getDiffuse());
}

CWVOID cwPrimitiveEntity::addPrimitive(const cwRay& ray, CWFLOAT fLen, const cwVector4D& color)
{
	m_pVertexData[m_uVertexCnt + 0].pos = ray.m_nOrigin;
	m_pVertexData[m_uVertexCnt + 0].color = color;

	m_pVertexData[m_uVertexCnt + 1].pos = ray.m_nOrigin + ray.m_nDir*fLen;
	m_pVertexData[m_uVertexCnt + 1].color = color;

	m_uVertexCnt += 2;
}

CWVOID cwPrimitiveEntity::addPrimitive(cwCamera* pCamera)
{
	addPrimitive(pCamera, m_pMaterial->getDiffuse());
}

CWVOID cwPrimitiveEntity::addPrimitive(cwCamera* pCamera, const cwVector4D& color)
{
	static CWUINT uIndex[24] = { 1, 2, 2, 3, 3, 4, 4, 1, 5, 6, 6, 7, 7, 8, 8, 5, 0, 5, 0, 6, 0, 7, 0, 8 };

	if (m_uVertexCnt + 24 > uMaxVertexCnt) return;

	CWFLOAT fFovY = pCamera->getFovY();
	CWFLOAT fFovX = pCamera->getAspect()*fFovY;

	CWFLOAT fTanY = tanf(fFovY*0.5f);
	CWFLOAT fTanX = tanf(fFovX*0.5f);

	CWFLOAT fNearZ = pCamera->getNearZ();
	CWFLOAT fFarZ = pCamera->getFarZ();

	CWFLOAT fNearH = fNearZ*fTanY;
	CWFLOAT fNearW = fNearZ*fTanX;

	CWFLOAT fFarH = fFarZ*fTanY;
	CWFLOAT fFarW = fFarZ*fTanX;

	cwVector4D points[9];
	points[1].set(-fNearW,  fNearH, fNearZ, 1.0f);
	points[2].set(fNearW, fNearH, fNearZ, 1.0f);
	points[3].set(fNearW, -fNearH, fNearZ, 1.0f);
	points[4].set(-fNearW, -fNearH, fNearZ, 1.0f);

	points[5].set(-fFarW, fFarH, fFarZ, 1.0f);
	points[6].set(fFarW, fFarH, fFarZ, 1.0f);
	points[7].set(fFarW, -fFarH, fFarZ, 1.0f);
	points[8].set(-fFarW, -fFarH, fFarZ, 1.0f);

	points[0] = pCamera->getPos();

	cwMatrix4X4 invMat = pCamera->getViewMatrix().inverse();
	for (int i = 1; i < 9; ++i) {
		points[i] *= invMat;
	}

	for (CWUINT i = 0; i < 24; ++i) {
		m_pVertexData[m_uVertexCnt + i].pos = points[uIndex[i]];
		m_pVertexData[m_uVertexCnt + i].color = color;
	}

	m_uVertexCnt += 24;
}

NS_MINIR_END
