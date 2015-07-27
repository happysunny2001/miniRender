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

#include "cwD3D11RenderTarget.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"

NS_MINIR_BEGIN

cwD3D11RenderTarget* cwD3D11RenderTarget::create(CWUINT iWidth, CWUINT iHeight)
{
	cwD3D11RenderTarget* pRenderTarget = new cwD3D11RenderTarget();
	if (pRenderTarget && pRenderTarget->init(iWidth, iHeight)) {
		pRenderTarget->autorelease();
		return pRenderTarget;
	}

	CW_SAFE_DELETE(pRenderTarget);
	return nullptr;
}

cwD3D11RenderTarget::cwD3D11RenderTarget():
m_pRenderTarget(nullptr)
{

}

cwD3D11RenderTarget::~cwD3D11RenderTarget()
{
	CW_RELEASE_COM(m_pRenderTarget);
}

bool cwD3D11RenderTarget::init(CWUINT iWidth, CWUINT iHeight)
{
	return onResize(iWidth, iHeight);
}

bool cwD3D11RenderTarget::onResize(CWUINT iWidth, CWUINT iHeight)
{

	ID3D11Texture2D* backBuffer = NULL;
	CW_HR(m_pDxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer));
	CW_HR(m_pD3D11Device->CreateRenderTargetView(backBuffer, NULL, &m_pRenderTarget));
	CW_RELEASE_COM(backBuffer);
}

NS_MINIR_END
