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

#ifdef _CW_D3D11_

#include "cwD3D11ShadowMapTexture.h"
#include "Texture/cwRenderTexture.h"
#include "ViewPort/cwViewPort.h"
#include "Repertory/cwRepertory.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"

NS_MINIR_BEGIN

cwD3D11ShadowMapTexture* cwD3D11ShadowMapTexture::create(CWFLOAT fWidth, CWFLOAT fHeight, CWBOOL bThreading)
{
	cwD3D11ShadowMapTexture* pTexture = new cwD3D11ShadowMapTexture();
	if (pTexture && pTexture->init(fWidth, fHeight)) {
		if (!bThreading) {
			pTexture->autorelease();
		}
		return pTexture;
	}

	CW_SAFE_DELETE(pTexture);
	return nullptr;
}

cwD3D11ShadowMapTexture::cwD3D11ShadowMapTexture()
{
	m_eType = eRenderTextureShadowMap;
}

cwD3D11ShadowMapTexture::~cwD3D11ShadowMapTexture()
{
	
}

CWBOOL cwD3D11ShadowMapTexture::init(CWFLOAT fWidth, CWFLOAT fHeight)
{
	if (!cwRenderTexture::init(fWidth, fHeight)) return CWFALSE;

	m_bResizeable = CWFALSE;

	return CWTRUE;
}

CWVOID cwD3D11ShadowMapTexture::beginResize()
{
}

CWBOOL cwD3D11ShadowMapTexture::onResize(CWBOOL bForce)
{
	CW_RELEASE_COM(m_pShaderResource);
	CW_RELEASE_COM(m_pRenderTarget);

	this->buildDepthStencilBuffer(static_cast<CWUINT>(m_fWidth), static_cast<CWUINT>(m_fHeight));

	return CWTRUE;
}

CWVOID cwD3D11ShadowMapTexture::buildDepthStencilBuffer(CWUINT iWidth, CWUINT iHeight)
{
	CW_RELEASE_COM(m_pDepthStencilBuffer);
	CW_RELEASE_COM(m_pDepthStencilView);

	cwD3D11Device* pDevice = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	if (!pDevice) return;

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = static_cast<CWUINT>(m_fWidth);
	texDesc.Height = static_cast<CWUINT>(m_fHeight);
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* pD3D11Tex = nullptr;
	CW_HR(pDevice->getD3D11Device()->CreateTexture2D(&texDesc, NULL, &pD3D11Tex));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	CW_HR(pDevice->getD3D11Device()->CreateDepthStencilView(pD3D11Tex, &dsvDesc, &m_pDepthStencilView));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	CW_HR(pDevice->getD3D11Device()->CreateShaderResourceView(pD3D11Tex, &srvDesc, &m_pShaderResource));

	CW_RELEASE_COM(pD3D11Tex);
}

CWHANDLE cwD3D11ShadowMapTexture::getHandle() const
{
	return static_cast<CWHANDLE>(m_pShaderResource);
}

CWVOID cwD3D11ShadowMapTexture::binding()
{
	ID3D11RenderTargetView* arrTargetView[1] = { 0 };

	cwD3D11Device* pDevice = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	pDevice->getD3D11DeviceContext()->OMSetRenderTargets(1, arrTargetView, m_pDepthStencilView);
}

CWVOID cwD3D11ShadowMapTexture::beginDraw(CWBOOL bClearColor, CWBOOL bClearDepth, CWBOOL bClearStencil)
{
	cwD3D11Device* pDevice = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	pDevice->getD3D11DeviceContext()->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

NS_MINIR_END

#endif
