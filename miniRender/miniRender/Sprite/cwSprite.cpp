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

#include "cwSprite.h"
#include "Base/cwUtils.h"
#include "Repertory/cwRepertory.h"
#include "Texture/cwTexture.h"
#include "Texture/cwTextureManager.h"
#include "Math/cwMath.h"
#include "RenderObject/cwRenderObject.h"
#include "Generator/cwGeometryGenerator.h"
#include "effect/cwEffectManager.h"
#include "effect/cwEffect.h"
#include "Device/cwDevice.h"

NS_MINIR_BEGIN

cwSprite* cwSprite::create()
{
	cwSprite* pSprite = new cwSprite();
	if (pSprite && pSprite->init()) {
		pSprite->autorelease();
		return pSprite;
	}

	CW_SAFE_DELETE(pSprite);
	return nullptr;
}

cwSprite* cwSprite::create(const std::string& strFile)
{
	cwSprite* pSprite = new cwSprite();
	if (pSprite && pSprite->init(strFile)) {
		pSprite->autorelease();
		return pSprite;
	}

	CW_SAFE_DELETE(pSprite);
	return nullptr;
}

cwSprite::cwSprite():
m_pTexture(nullptr),
//m_pVertexBuffer(nullptr),
m_pRenderObject(nullptr)
{
	m_eRenderType = eRenderTypeSprite;
	m_nColor.set(1.0f, 1.0f, 1.0f, 1.0f);
	m_nAnchorPoint.zero();
}

cwSprite::~cwSprite()
{
	CW_SAFE_RELEASE_NULL(m_pTexture);
	//CW_SAFE_DELETE(m_pVertexBuffer);
	CW_SAFE_RELEASE_NULL(m_pRenderObject);
}

CWBOOL cwSprite::init()
{
	if (!cwRenderNode2D::init()) return CWFALSE;
	if (!buildVertexBuffer()) return CWFALSE;

	return CWTRUE;
}

CWBOOL cwSprite::init(const std::string& strFile)
{
	if (!cwRenderNode2D::init()) return CWFALSE;
	if (!loadTexture(strFile)) return CWFALSE;
	if (!buildVertexBuffer()) return CWFALSE;

	return CWTRUE;
}

CWBOOL cwSprite::loadTexture(const std::string& strFile)
{
	CW_SAFE_RELEASE_NULL(m_pTexture);

	cwRepertory& repetory = cwRepertory::getInstance();
	m_pTexture = repetory.getTextureManager()->getTexture(strFile);
	if (!m_pTexture) return CWFALSE;
	CW_SAFE_RETAIN(m_pTexture);

	this->setEffect(repetory.getEffectManager()->defaultSpriteEffect());

	CWFLOAT fHalfTexWidth = 0.5f;
	CWFLOAT fHalfTexHeight = 0.5f;

	if (m_pTexture) {
		fHalfTexWidth = m_pTexture->getWidth()*0.5f;
		fHalfTexHeight = m_pTexture->getWidth()*0.5f;
	}

	m_nMatSizeScale.setScale(fHalfTexWidth, fHalfTexHeight, 1.0f);

	return CWTRUE;
}

CWVOID cwSprite::setTexture(cwTexture* pTexture)
{
	CW_SAFE_RETAIN(pTexture);
	CW_SAFE_RELEASE(m_pTexture);
	m_pTexture = pTexture;
	if (!m_pTexture) return;

	cwRepertory& repetory = cwRepertory::getInstance();
	this->setEffect(repetory.getEffectManager()->defaultSpriteEffect());

	CWFLOAT fHalfTexWidth = 0.5f;
	CWFLOAT fHalfTexHeight = 0.5f;

	if (m_pTexture) {
		fHalfTexWidth = m_pTexture->getWidth()*0.5f;
		fHalfTexHeight = m_pTexture->getWidth()*0.5f;
	}

	m_nMatSizeScale.setScale(fHalfTexWidth, fHalfTexHeight, 1.0f);
}

CWBOOL cwSprite::buildVertexBuffer()
{
	//CW_SAFE_DELETE(m_pVertexBuffer);

	//m_pVertexBuffer = new cwVertexPosTexColor[6];
	//if (!m_pVertexBuffer) return CWFALSE;

	//initVertexBuffer(m_pVertexBuffer);

	cwRepertory& repetory = cwRepertory::getInstance();
	m_pRenderObject = repetory.getGeoGenerator()->defaultSpriteRenderObject();
	CW_SAFE_RETAIN(m_pRenderObject);

	return CWTRUE;
}

CWVOID cwSprite::setAnchorPoint(const cwVector2D& anchorPoint)
{
	m_nAnchorPoint.x = anchorPoint.x;
	m_nAnchorPoint.y = anchorPoint.y;
	m_bTransDirty = CWTRUE;
}

CWVOID cwSprite::setAnchorPoint(const cwVector3D& anchorPoint)
{
	m_nAnchorPoint = anchorPoint;
	m_bTransDirty = CWTRUE;
}

//CWVOID cwSprite::initVertexBuffer(cwVertexPosTexColor* pVertexBuffer)
//{
//	CWFLOAT fHalfTexWidth  = 0.5f;
//	CWFLOAT fHalfTexHeight = 0.5f;
//
//	if (m_pTexture) {
//		fHalfTexWidth  = m_pTexture->getWidth()*0.5f;
//		fHalfTexHeight = m_pTexture->getWidth()*0.5f;
//	}
//
//	pVertexBuffer[0].pos.set(-fHalfTexWidth, -fHalfTexHeight, 0);
//	pVertexBuffer[0].tex.set(0, 1.0f);
//	pVertexBuffer[0].color = m_nColor;
//
//	pVertexBuffer[1].pos.set(-fHalfTexWidth, fHalfTexHeight, 0);
//	pVertexBuffer[1].tex.set(0, 0);
//	pVertexBuffer[1].color = m_nColor;
//
//	pVertexBuffer[2].pos.set(fHalfTexWidth, -fHalfTexHeight, 0);
//	pVertexBuffer[2].tex.set(1.0f, 1.0f);
//	pVertexBuffer[2].color = m_nColor;
//
//	pVertexBuffer[3].pos.set(-fHalfTexWidth, fHalfTexHeight, 0);
//	pVertexBuffer[3].tex.set(0, 0);
//	pVertexBuffer[3].color = m_nColor;
//
//	pVertexBuffer[4].pos.set(fHalfTexWidth, fHalfTexHeight, 0);
//	pVertexBuffer[4].tex.set(1.0f, 0);
//	pVertexBuffer[4].color = m_nColor;
//
//	pVertexBuffer[5].pos.set(fHalfTexWidth, -fHalfTexHeight, 0);
//	pVertexBuffer[5].tex.set(1.0f, 1.0f);
//	pVertexBuffer[5].color = m_nColor;
//
//	m_nBoundingBox.m_nMin = pVertexBuffer[0].pos;
//	m_nBoundingBox.m_nMax = pVertexBuffer[4].pos;
//}

//CWVOID cwSprite::initVertexBuffer(cwVector4D* pVertexBuffer)
//{
//	CWFLOAT fHalfTexWidth = 0.5f;
//	CWFLOAT fHalfTexHeight = 0.5f;
//
//	if (m_pTexture) {
//		fHalfTexWidth = m_pTexture->getWidth()*0.5f;
//		fHalfTexHeight = m_pTexture->getWidth()*0.5f;
//	}
//
//	pVertexBuffer[0].set(-fHalfTexWidth, -fHalfTexHeight, 0, 1.0f);
//	pVertexBuffer[1].set(-fHalfTexWidth, fHalfTexHeight, 0, 1.0f);
//	pVertexBuffer[2].set(fHalfTexWidth, -fHalfTexHeight, 0, 1.0f);
//
//	pVertexBuffer[3].set(-fHalfTexWidth, fHalfTexHeight, 0, 1.0f);
//	pVertexBuffer[4].set(fHalfTexWidth, fHalfTexHeight, 0, 1.0f);
//	pVertexBuffer[5].set(fHalfTexWidth, -fHalfTexHeight, 0, 1.0f);
//}

CWVOID cwSprite::transform()
{
	if (m_bTransDirty) {
		cwRenderNode2D::transform();

		cwVector3D vecTexture(m_pTexture->getWidth(), m_pTexture->getHeight(), 0);
		cwVector4D offset = (cwVector3D::ZERO - m_nAnchorPoint)*m_nScale*vecTexture;
		m_nLocalTrans.m41 += offset.x;
		m_nLocalTrans.m42 += offset.y;
		m_nLocalTrans.m43 += offset.z;
	}
}

CWVOID cwSprite::refreshTransform()
{
	cwRenderNode::refreshTransform();
	m_nTrans = m_nMatSizeScale*m_nTrans;

	//transformBuffer();
}

//CWVOID cwSprite::transformBuffer()
//{
//	cwVector4D nVertex[6];
//	initVertexBuffer(nVertex);
//
//	for (CWUINT i = 0; i < 6; ++i) {
//		m_pVertexBuffer[i].pos = nVertex[i] * m_nTrans;
//	}
//
//	m_nBoundingBox.m_nMin = m_pVertexBuffer[0].pos;
//	m_nBoundingBox.m_nMax = m_pVertexBuffer[4].pos;
//}

CWVOID cwSprite::refreshBoundingBox()
{

}

CWVOID cwSprite::render(cwCamera* pCamera)
{
	if (!m_pEffect) return;
	cwRepertory& repertory = cwRepertory::getInstance();

	if (this->getStencil())
		repertory.getDevice()->setStencil(this->getStencil());

	cwShader* pShader = m_pEffect->getShader();
	if (pShader) {
		repertory.getDevice()->setShaderWorldTrans(pShader, this->getTransformMatrix(), pCamera);
		pShader->setVariableTexture(eShaderParamDiffuseTexture, this->getTexture());
		//repertory.getDevice()->draw(pShader, m_pEffect->getTech(), m_pRenderObject);
		repertory.getDevice()->draw(m_pEffect, m_pRenderObject);
	}
}

CWVOID cwSprite::render(cwEffect* pEffect)
{
	if (pEffect) {
		cwShader* pShader = m_pEffect->getShader();
		if (pShader && this->getTexture()) {
			pShader->setVariableTexture("gSpriteTexture", this->getTexture());
		}

		cwDevice* pDevice = cwRepertory::getInstance().getDevice();

		if (m_pStencil)
			pDevice->setStencil(m_pStencil);
		if (m_pBlend)
			pDevice->setBlend(m_pBlend);

		pDevice->draw(pEffect, m_pRenderObject);
	}
}

NS_MINIR_END
