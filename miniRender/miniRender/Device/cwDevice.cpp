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

NS_MINIR_BEGIN

cwDevice::cwDevice() :
m_fvClearColor(cwColor::black),
m_eRenderState(eRenderStateSolid),
m_bEnableMsaa4x(false),
m_pBlendState(nullptr),
m_pRenderTargetBkBuffer(nullptr),
m_pCurrRenderTarget(nullptr),
m_pDepthStencil(nullptr),
m_bRefreshRenderTarget(true)
{

}

cwDevice::~cwDevice()
{
	m_pBlendState = nullptr;
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

	m_bRefreshRenderTarget = true;
}

void cwDevice::setDepthStentil(cwRenderTexture* pDepthStencil)
{
	if (m_pDepthStencil == pDepthStencil) return;

	CW_SAFE_RETAIN(pDepthStencil);
	CW_SAFE_RELEASE_NULL(m_pDepthStencil);
	m_pDepthStencil = pDepthStencil;

	m_bRefreshRenderTarget = true;
}

void cwDevice::setClearColor(const cwVector4D& color)
{
	m_fvClearColor = color;
}

NS_MINIR_END
