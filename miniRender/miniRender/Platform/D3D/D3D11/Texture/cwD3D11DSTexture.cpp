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

#include "cwD3D11DSTexture.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"

NS_MINIR_BEGIN

cwD3D11DSTexture* cwD3D11DSTexture::create(CWBOOL bThreadSafe)
{
	cwD3D11DSTexture* pTexture = new cwD3D11DSTexture();
	if (pTexture && pTexture->init()) {
		if (!bThreadSafe)
			pTexture->autorelease();
		return pTexture;
	}

	CW_SAFE_DELETE(pTexture);
	return nullptr;
}

cwD3D11DSTexture* cwD3D11DSTexture::create(CWFLOAT fWidth, CWFLOAT fHeight, CWBOOL bShaderUsage, CWBOOL bThreadSafe)
{
	cwD3D11DSTexture* pTexture = new cwD3D11DSTexture();
	if (pTexture && pTexture->init(fWidth, fHeight, bShaderUsage)) {
		if (!bThreadSafe)
			pTexture->autorelease();
		return pTexture;
	}

	CW_SAFE_DELETE(pTexture);
	return nullptr;
}

cwD3D11DSTexture::cwD3D11DSTexture() :
m_pDepthStencilView(NULL)
{

}

cwD3D11DSTexture::~cwD3D11DSTexture()
{
	CW_RELEASE_COM(m_pDepthStencilView);
}

CWBOOL cwD3D11DSTexture::init(CWFLOAT fWidth, CWFLOAT fHeight, CWBOOL bShaderUsage)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_bShaderUsage = bShaderUsage;

	return buildDepthStencil();
}

CWBOOL cwD3D11DSTexture::init()
{
	cwD3D11Device* pDevice = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	if (!pDevice) return CWFALSE;

	ID3D11Texture2D* backBuffer = NULL;
	CW_HR(pDevice->getSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));

	D3D11_TEXTURE2D_DESC texDesc;
	backBuffer->GetDesc(&texDesc);
	CW_RELEASE_COM(backBuffer);

	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* pTex = NULL;
	CW_HR(pDevice->getD3D11Device()->CreateTexture2D(&texDesc, NULL, &pTex));
	CW_HR(pDevice->getD3D11Device()->CreateDepthStencilView(pTex, NULL, &m_pDepthStencilView));
	CW_RELEASE_COM(pTex);

	return CWTRUE;
}

CWHANDLE cwD3D11DSTexture::getRenderHandle() const
{
	return static_cast<CWHANDLE>(m_pDepthStencilView);
}

CWBOOL cwD3D11DSTexture::buildDepthStencil()
{
	cwD3D11Device* pDevice = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	if (!pDevice) return CWFALSE;

	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));

	texDesc.Width = static_cast<UINT>(m_fWidth);
	texDesc.Height = static_cast<UINT>(m_fHeight);
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	if (m_bShaderUsage) {
		texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	}
	else {
		texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	}

	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* pTex = NULL;
	CW_HR(pDevice->getD3D11Device()->CreateTexture2D(&texDesc, NULL, &pTex));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	memset(&dsvDesc, 0, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	CW_HR(pDevice->getD3D11Device()->CreateDepthStencilView(pTex, &dsvDesc, &m_pDepthStencilView));

	if (m_bShaderUsage) {
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		memset(&srvDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		CW_HR(pDevice->getD3D11Device()->CreateShaderResourceView(pTex, &srvDesc, &m_pShaderResource));
	}

	return CWTRUE;
}

NS_MINIR_END

#endif
