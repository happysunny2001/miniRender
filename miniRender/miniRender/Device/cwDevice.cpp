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

#include "cwDevice.h"
#include "Base/cwColor.h"
#include "Base/cwMacros.h"
#include "Texture/cwRenderTexture.h"
#include "Texture/cwTextureManager.h"
#include "ViewPort/cwViewPort.h"
#include "Repertory/cwRepertory.h"
#include "Stencil/cwStencil.h"

NS_MINIR_BEGIN

cwDevice::cwDevice() :
m_fvClearColor(cwColor::black),
m_eRenderState(eRenderStateSolid),
m_bEnableMsaa4x(false),
m_pBlendState(nullptr),
m_pRenderTargetBkBuffer(nullptr),
m_pCurrRenderTarget(nullptr),
m_bRefreshRenderTarget(false),
m_pDefaultViewPort(nullptr),
m_pCurrViewPort(nullptr),
m_bRefreshViewPort(false),
m_pStencil(nullptr),
m_pDisableZBuffer(nullptr)
{

}

cwDevice::~cwDevice()
{
	m_pBlendState = nullptr;
	m_pStencil = nullptr;
	CW_SAFE_RELEASE_NULL(m_pDefaultViewPort);
	CW_SAFE_RELEASE_NULL(m_pCurrViewPort);

	CW_SAFE_RELEASE_NULL(m_pRenderTargetBkBuffer);
	CW_SAFE_RELEASE_NULL(m_pCurrRenderTarget);
	CW_SAFE_RELEASE_NULL(m_pDisableZBuffer);
}

eRenderState cwDevice::getRenderState()
{
	return m_eRenderState;
}

void cwDevice::setRenderTarget(cwRenderTexture* pRenderTexture)
{
	if (m_pCurrRenderTarget == pRenderTexture) return;
	if (pRenderTexture == nullptr) {
		if(m_pCurrRenderTarget == m_pRenderTargetBkBuffer) return;
		pRenderTexture = m_pRenderTargetBkBuffer;
	}

	CW_SAFE_RETAIN(pRenderTexture);
	CW_SAFE_RELEASE_NULL(m_pCurrRenderTarget);
	m_pCurrRenderTarget = pRenderTexture;

	m_bRefreshRenderTarget = CWTRUE;
}

CWVOID cwDevice::setViewPort(cwViewPort* pViewPort)
{
	if (m_pCurrViewPort == pViewPort) return;
	if (pViewPort == nullptr) {
		if (m_pCurrViewPort == m_pDefaultViewPort) return;
		pViewPort = m_pDefaultViewPort;
	}

	CW_SAFE_RETAIN(pViewPort);
	CW_SAFE_RELEASE_NULL(m_pCurrViewPort);
	m_pCurrViewPort = pViewPort;

	m_bRefreshViewPort = CWTRUE;
}

CWVOID cwDevice::createDefaultViewPort()
{
	if (m_pDefaultViewPort) {
		CW_SAFE_RELEASE_NULL(m_pDefaultViewPort);
	}

	CWUINT winWidth = cwRepertory::getInstance().getUInt(gValueWinWidth);
	CWUINT winHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);

	m_pDefaultViewPort = createViewPort(0, 0, static_cast <CWFLOAT>(winWidth), static_cast <CWFLOAT>(winHeight));
	CW_SAFE_RETAIN(m_pDefaultViewPort);

	setViewPort(m_pDefaultViewPort);
}

CWVOID cwDevice::createDefaultRenderTarget()
{
	if (m_pRenderTargetBkBuffer) {
		CW_SAFE_RELEASE_NULL(m_pRenderTargetBkBuffer);
	}

	m_pRenderTargetBkBuffer = cwRepertory::getInstance().getTextureManager()->createRenderTexture(1.0f, 1.0f, eRenderTextureTarget);
	CW_SAFE_RETAIN(m_pRenderTargetBkBuffer);
	this->setRenderTarget(m_pRenderTargetBkBuffer);
}

CWVOID cwDevice::createDefaultStencil()
{
	cwStencilData stencilData;
	
	stencilData.depthEnable = CWFALSE;
	stencilData.depthWriteMask = eDepthWriteMaskAll;
	stencilData.depthFunc = eComparisonLess;

	stencilData.stencilEnable = CWTRUE;
	stencilData.stencilReadMask = 0xFF;
	stencilData.stencilWriteMask = 0xFF;

	stencilData.frontStencilFailOp = eStencilOpKeep;
	stencilData.frontStencilDepthFailOp = eStencilOpIncr;
	stencilData.frontStencilPassOp = eStencilOpKeep;
	stencilData.frontStencilFunc = eComparisonAlways;

	stencilData.backStencilFailOp = eStencilOpKeep;
	stencilData.backStencilDepthFailOp = eStencilOpDecr;
	stencilData.backStencilPassOp = eStencilOpKeep;
	stencilData.backStencilFunc = eComparisonAlways;

	m_pDisableZBuffer = this->createStencil(stencilData);
	CW_SAFE_RETAIN(m_pDisableZBuffer);
	if (m_pDisableZBuffer) {
		m_pDisableZBuffer->setStencilRef(1);
	}
}

CWVOID cwDevice::setClearColor(const cwVector4D& color)
{
	m_fvClearColor = color;
}

CWBOOL cwDevice::disableZBuffer()
{
	if (m_pDisableZBuffer) {
		this->setStencil(m_pDisableZBuffer);
		return CWTRUE;
	}

	return CWFALSE;
}

CWBOOL cwDevice::enableZBuffer()
{
	this->setStencil(nullptr);
	return CWTRUE;
}

NS_MINIR_END
