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

#ifdef _CW_D3D11_

#include "cwD3D11RenderTexture.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"

NS_MINIR_BEGIN

cwD3D11RenderTexture* cwD3D11RenderTexture::create(CWUINT iWidth, CWUINT iHeight)
{
	cwD3D11RenderTexture* pRenderTex = new cwD3D11RenderTexture();
	if (pRenderTex && pRenderTex->init(iWidth, iHeight)) {
		pRenderTex->autorelease();
		return pRenderTex;
	}

	CW_SAFE_DELETE(pRenderTex);
	return nullptr;
}

cwD3D11RenderTexture::cwD3D11RenderTexture():
m_pShaderResource(NULL),
m_pRenderTarget(NULL)
{

}

cwD3D11RenderTexture::~cwD3D11RenderTexture()
{
	CW_RELEASE_COM(m_pShaderResource);
	CW_RELEASE_COM(m_pRenderTarget);
}

bool cwD3D11RenderTexture::init(CWUINT iWidth, CWUINT iHeight)
{
	return onResize(iWidth, iHeight);
}

bool cwD3D11RenderTexture::onResize(CWUINT iWidth, CWUINT iHeight)
{
	CW_RELEASE_COM(m_pShaderResource);
	CW_RELEASE_COM(m_pRenderTarget);

	D3D11_TEXTURE2D_DESC texDesc;

	texDesc.Width = iWidth;
	texDesc.Height = iHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* pTex = NULL;
	ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(cwRepertory::getInstance().getDevice()->getDevice());
	if (!pD3D11Device) return false;

	CW_HR(pD3D11Device->CreateTexture2D(&texDesc, NULL, &pTex));
	CW_HR(pD3D11Device->CreateShaderResourceView(pTex, NULL, &m_pShaderResource));
	CW_HR(pD3D11Device->CreateRenderTargetView(pTex, NULL, &m_pRenderTarget));

	CW_RELEASE_COM(pTex);

	m_iWidth  = iWidth;
	m_iHeight = iHeight;

	return true;
}

NS_MINIR_END

#endif
