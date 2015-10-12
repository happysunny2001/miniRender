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
m_pVertexBuffer(nullptr)
{

}

cwSprite::~cwSprite()
{
	CW_SAFE_RELEASE_NULL(m_pTexture);
	CW_SAFE_DELETE(m_pVertexBuffer);
}

CWBOOL cwSprite::init()
{
	if (!cwRenderNode::init()) return CWFALSE;

	m_eType = eSceneObjectSprite;
	m_nColor.set(1.0f, 1.0f, 1.0f, 1.0f);

	if (!buildVertexBuffer()) return CWFALSE;

	return CWTRUE;
}

CWBOOL cwSprite::init(const std::string& strFile)
{
	if (!cwRenderNode::init()) return CWFALSE;

	m_eType = eSceneObjectSprite;
	m_nColor.set(1.0f, 1.0f, 1.0f, 1.0f);

	if (!loadTexture(strFile)) return CWFALSE;
	if (!buildVertexBuffer()) return CWFALSE;

	return CWTRUE;
}

CWBOOL cwSprite::loadTexture(const std::string& strFile)
{
	CW_SAFE_RELEASE_NULL(m_pTexture);

	m_pTexture = cwRepertory::getInstance().getTextureManager()->getTexture(strFile);
	if (!m_pTexture) return CWFALSE;
	CW_SAFE_RETAIN(m_pTexture);

	return CWTRUE;
}

CWBOOL cwSprite::buildVertexBuffer()
{
	CW_SAFE_DELETE(m_pVertexBuffer);

	m_pVertexBuffer = new cwVertexPosTexColor[6];
	if (!m_pVertexBuffer) return CWFALSE;

	initVertexBuffer();

	return CWTRUE;
}

CWVOID cwSprite::initVertexBuffer()
{
	CWFLOAT fHalfTexWidth  = 0.5f;
	CWFLOAT fHalfTexHeight = 0.5f;

	if (m_pTexture) {
		fHalfTexWidth  = m_pTexture->getWidth()*0.5f;
		fHalfTexHeight = m_pTexture->getWidth()*0.5f;
	}

	m_pVertexBuffer[0].pos.set(-fHalfTexWidth, -fHalfTexHeight, 0);
	m_pVertexBuffer[0].tex.set(0, 1.0f);
	m_pVertexBuffer[0].color = m_nColor;

	m_pVertexBuffer[1].pos.set(-fHalfTexWidth, fHalfTexHeight, 0);
	m_pVertexBuffer[1].tex.set(0, 0);
	m_pVertexBuffer[1].color = m_nColor;

	m_pVertexBuffer[2].pos.set(fHalfTexWidth, -fHalfTexHeight, 0);
	m_pVertexBuffer[2].tex.set(0, 1.0f);
	m_pVertexBuffer[2].color = m_nColor;

	m_pVertexBuffer[3].pos.set(-fHalfTexWidth, fHalfTexHeight, 0);
	m_pVertexBuffer[3].tex.set(0, 0);
	m_pVertexBuffer[3].color = m_nColor;

	m_pVertexBuffer[4].pos.set(fHalfTexWidth, fHalfTexHeight, 0);
	m_pVertexBuffer[4].tex.set(0, 0);
	m_pVertexBuffer[4].color = m_nColor;

	m_pVertexBuffer[5].pos.set(fHalfTexWidth, -fHalfTexHeight, 0);
	m_pVertexBuffer[5].tex.set(0, 1.0f);
	m_pVertexBuffer[5].color = m_nColor;
}

CWVOID cwSprite::insertSpatialNode(cwRenderNode* pNode)
{

}

CWVOID cwSprite::removeSpatialNode(cwRenderNode* pNode)
{

}

CWVOID cwSprite::refreshSpatialNode()
{

}

NS_MINIR_END
