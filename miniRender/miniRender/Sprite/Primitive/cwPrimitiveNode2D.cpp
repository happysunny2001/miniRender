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

#include "cwPrimitiveNode2D.h"
#include "Base/cwColor.h"
#include "effect/cwEffect.h"
#include "effect/cwEffectManager.h"
#include "Repertory/cwRepertory.h"
#include "Shader/cwShaderManager.h"
#include "Device/cwDevice.h"

NS_MINIR_BEGIN

CWUINT cwPrimitiveNode2D::uMaxVertexCnt = 102400;

cwPrimitiveNode2D* cwPrimitiveNode2D::create(CWBOOL bThreading)
{
	cwPrimitiveNode2D* pPrimitiveNode = new cwPrimitiveNode2D();
	if (pPrimitiveNode && pPrimitiveNode->init()) {
		if (!bThreading)
			pPrimitiveNode->autorelease();
		return pPrimitiveNode;
	}

	CW_SAFE_DELETE(pPrimitiveNode);
	return nullptr;
}

cwPrimitiveNode2D::cwPrimitiveNode2D() : 
m_pFrameVertexBuffer(nullptr),
m_pFrameRenderObjects(nullptr),
m_uFrameVertexCnt(0),
m_pSolidVertexBuffer(nullptr),
m_uSolidVertexCnt(0),
m_pSolidRenderObjects(nullptr)
{

}

cwPrimitiveNode2D::~cwPrimitiveNode2D()
{
	CW_SAFE_DELETE_ARRAY(m_pFrameVertexBuffer);
	CW_SAFE_RELEASE_NULL(m_pFrameRenderObjects);

	CW_SAFE_DELETE_ARRAY(m_pSolidVertexBuffer);
	CW_SAFE_RELEASE_NULL(m_pSolidRenderObjects);
}

CWBOOL cwPrimitiveNode2D::init()
{
	if (!cwRenderNode2D::init()) return CWFALSE;

	buildRenderBuffer();
	buildEffect();

	return CWTRUE;
}

CWVOID cwPrimitiveNode2D::buildRenderBuffer()
{
	m_pFrameVertexBuffer = new cwVertexPosColor[uMaxVertexCnt];
	m_uFrameVertexCnt = 0;

	m_pFrameRenderObjects = cwDynamicRenderObject::create(
		ePrimitiveTypeLineList, 
		m_pFrameVertexBuffer, 
		sizeof(cwVertexPosColor), 
		uMaxVertexCnt, 
		nullptr, 
		0, 
		"PosColor");
	CW_SAFE_RETAIN(m_pFrameRenderObjects);

	m_pSolidVertexBuffer = new cwVertexPosColor[uMaxVertexCnt];
	m_uSolidVertexCnt = 0;

	m_pSolidRenderObjects = cwDynamicRenderObject::create(
		ePrimitiveTypeTriangleList,
		m_pSolidVertexBuffer,
		sizeof(cwVertexPosColor),
		uMaxVertexCnt,
		nullptr,
		0,
		"PosColor");
	CW_SAFE_RETAIN(m_pSolidRenderObjects);
}

CWVOID cwPrimitiveNode2D::buildEffect()
{
	//cwEffect* pEffect = cwEffect::create();
	//cwShader* pShader = cwRepertory::getInstance().getShaderManager()->getDefShader(eDefShaderColor);
	//if (pShader && pEffect) {
	//	pEffect->setShader(pShader);
	//	this->setEffect(pEffect);
	//}

	cwEffect* pEffect = cwRepertory::getInstance().getEffectManager()->getDefEffect(eEffectIDColor);
	if (pEffect) {
		this->setEffect(pEffect);
	}
}

CWVOID cwPrimitiveNode2D::drawLine(const cwVector2D& start, const cwVector2D& end, const cwVector4D& color)
{
	if (m_uFrameVertexCnt + 2 > uMaxVertexCnt) return;

	m_pFrameVertexBuffer[m_uFrameVertexCnt + 0].pos = cwVector3D(start.x, start.y, 0);
	m_pFrameVertexBuffer[m_uFrameVertexCnt + 0].color = color;

	m_pFrameVertexBuffer[m_uFrameVertexCnt + 1].pos = cwVector3D(end.x, end.y, 0);
	m_pFrameVertexBuffer[m_uFrameVertexCnt + 1].color = color;

	m_uFrameVertexCnt += 2;
}

CWVOID cwPrimitiveNode2D::drawQuad(
	const cwVector2D& p0,
	const cwVector2D& p1,
	const cwVector2D& p2,
	const cwVector2D& p3,
	const cwVector4D& color, CWBOOL bFill)
{
	if (!bFill) {
		if (m_uFrameVertexCnt + 8 > uMaxVertexCnt) return;

		m_pFrameVertexBuffer[m_uFrameVertexCnt + 0].pos = cwVector3D(p0.x, p0.y, 0);
		m_pFrameVertexBuffer[m_uFrameVertexCnt + 0].color = color;
		m_pFrameVertexBuffer[m_uFrameVertexCnt + 1].pos = cwVector3D(p1.x, p1.y, 0);
		m_pFrameVertexBuffer[m_uFrameVertexCnt + 1].color = color;

		m_pFrameVertexBuffer[m_uFrameVertexCnt + 2].pos = cwVector3D(p1.x, p1.y, 0);
		m_pFrameVertexBuffer[m_uFrameVertexCnt + 2].color = color;
		m_pFrameVertexBuffer[m_uFrameVertexCnt + 3].pos = cwVector3D(p2.x, p2.y, 0);
		m_pFrameVertexBuffer[m_uFrameVertexCnt + 3].color = color;

		m_pFrameVertexBuffer[m_uFrameVertexCnt + 4].pos = cwVector3D(p2.x, p2.y, 0);
		m_pFrameVertexBuffer[m_uFrameVertexCnt + 4].color = color;
		m_pFrameVertexBuffer[m_uFrameVertexCnt + 5].pos = cwVector3D(p3.x, p3.y, 0);
		m_pFrameVertexBuffer[m_uFrameVertexCnt + 5].color = color;

		m_pFrameVertexBuffer[m_uFrameVertexCnt + 6].pos = cwVector3D(p3.x, p3.y, 0);
		m_pFrameVertexBuffer[m_uFrameVertexCnt + 6].color = color;
		m_pFrameVertexBuffer[m_uFrameVertexCnt + 7].pos = cwVector3D(p0.x, p0.y, 0);
		m_pFrameVertexBuffer[m_uFrameVertexCnt + 7].color = color;

		m_uFrameVertexCnt += 8;
	}
	else {
		if (m_uSolidVertexCnt + 6 > uMaxVertexCnt) return;

		m_pSolidVertexBuffer[m_uSolidVertexCnt + 0].pos = cwVector3D(p0.x, p0.y, 0);
		m_pSolidVertexBuffer[m_uSolidVertexCnt + 0].color = color;
		m_pSolidVertexBuffer[m_uSolidVertexCnt + 1].pos = cwVector3D(p1.x, p1.y, 0);
		m_pSolidVertexBuffer[m_uSolidVertexCnt + 1].color = color;
		m_pSolidVertexBuffer[m_uSolidVertexCnt + 2].pos = cwVector3D(p2.x, p2.y, 0);
		m_pSolidVertexBuffer[m_uSolidVertexCnt + 2].color = color;

		m_pSolidVertexBuffer[m_uSolidVertexCnt + 3].pos = cwVector3D(p0.x, p0.y, 0);
		m_pSolidVertexBuffer[m_uSolidVertexCnt + 3].color = color;
		m_pSolidVertexBuffer[m_uSolidVertexCnt + 4].pos = cwVector3D(p2.x, p2.y, 0);
		m_pSolidVertexBuffer[m_uSolidVertexCnt + 4].color = color;
		m_pSolidVertexBuffer[m_uSolidVertexCnt + 5].pos = cwVector3D(p3.x, p3.y, 0);
		m_pSolidVertexBuffer[m_uSolidVertexCnt + 5].color = color;

		m_uSolidVertexCnt += 6;
	}
}

CWVOID cwPrimitiveNode2D::render(cwCamera* pCamera)
{
	if (!m_pEffect) return;
	cwRepertory& repertory = cwRepertory::getInstance();

	if (this->getStencil())
		repertory.getDevice()->setStencil(this->getStencil());

	cwShader* pShader = m_pEffect->getShader();
	if (pShader) {
		repertory.getDevice()->setShaderWorldTrans(pShader, this->getTransformMatrix(), pCamera);

		if (m_uFrameVertexCnt > 0) {
			m_pFrameRenderObjects->setValidVertexCnt(m_uFrameVertexCnt);
			m_pFrameRenderObjects->updateVertexData(m_pFrameVertexBuffer, m_uFrameVertexCnt);
			//repertory.getDevice()->draw(pShader, m_pEffect->getTech(), m_pFrameRenderObjects);
			repertory.getDevice()->draw(m_pEffect, m_pFrameRenderObjects);
		}

		if (m_uSolidVertexCnt > 0) {
			m_pSolidRenderObjects->setValidVertexCnt(m_uSolidVertexCnt);
			m_pSolidRenderObjects->updateVertexData(m_pSolidVertexBuffer, m_uSolidVertexCnt);
			//repertory.getDevice()->draw(pShader, m_pEffect->getTech(), m_pSolidRenderObjects);
			repertory.getDevice()->draw(m_pEffect, m_pSolidRenderObjects);
		}
	}

	m_uFrameVertexCnt = 0;
	m_uSolidVertexCnt = 0;
}

NS_MINIR_END

