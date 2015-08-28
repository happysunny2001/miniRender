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

#include "cwD3D11RenderTextureWritable.h"
#include "Repertory/cwRepertory.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"

NS_MINIR_BEGIN

cwD3D11RenderTextureWritable* cwD3D11RenderTextureWritable::create(CWFLOAT fWidth, CWFLOAT fHeight)
{
	cwD3D11RenderTextureWritable* pRenderTexture = new cwD3D11RenderTextureWritable();
	if (pRenderTexture && pRenderTexture->init(fWidth, fHeight)) {
		pRenderTexture->autorelease();
		return pRenderTexture;
	}

	CW_SAFE_DELETE(pRenderTexture);
	return nullptr;
}

cwD3D11RenderTextureWritable::cwD3D11RenderTextureWritable() :
m_pUnorderedAccessView(nullptr)
{
	m_eType = eRenderTextureWritable;
}

cwD3D11RenderTextureWritable::~cwD3D11RenderTextureWritable()
{
	CW_RELEASE_COM(m_pUnorderedAccessView);
}

bool cwD3D11RenderTextureWritable::init(CWFLOAT fWidth, CWFLOAT fHeight)
{
	return cwD3D11RenderTexture::init(fWidth, fHeight);
}

bool cwD3D11RenderTextureWritable::onResize(bool bForce)
{
	if (!bForce && m_fWidth > 1.0f) return true;

	CW_RELEASE_COM(m_pUnorderedAccessView);
	CW_RELEASE_COM(m_pShaderResource);
	CW_RELEASE_COM(m_pRenderTarget);

	CWUINT texWidth = 0;
	CWUINT texHeight = 0;

	if (m_fWidth > 1.0f) {
		texWidth = static_cast<CWUINT>(m_fWidth);
		texHeight = static_cast<CWUINT>(m_fHeight);
	}
	else {
		CWUINT winWidth  = cwRepertory::getInstance().getUInt(gValueWinWidth);
		CWUINT winHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);

		texWidth  = static_cast<CWUINT>(m_fWidth*winWidth);
		texHeight = static_cast<CWUINT>(m_fHeight*winHeight);
	}

	D3D11_TEXTURE2D_DESC texDesc;

	texDesc.Width = texWidth;
	texDesc.Height = texHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* pTex = NULL;
	cwD3D11Device* pDevice = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	if (!pDevice) return false;

	CW_HR(pDevice->getD3D11Device()->CreateTexture2D(&texDesc, NULL, &pTex));
	CW_HR(pDevice->getD3D11Device()->CreateShaderResourceView(pTex, NULL, &m_pShaderResource));
	CW_HR(pDevice->getD3D11Device()->CreateRenderTargetView(pTex, NULL, &m_pRenderTarget));
	CW_HR(pDevice->getD3D11Device()->CreateUnorderedAccessView(pTex, NULL, &m_pUnorderedAccessView));

	CW_RELEASE_COM(pTex);

	buildDepthStencilBuffer(texWidth, texHeight);

	return true;
}

CWHANDLE cwD3D11RenderTextureWritable::getWritablehandle() const
{
	return static_cast<CWHANDLE>(m_pUnorderedAccessView);
}

NS_MINIR_END

#endif


