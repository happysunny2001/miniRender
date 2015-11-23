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

#include "cwD3D11CubeTexture.h"
#include "Base/cwStringConvert.h"
#include "Base/cwColor.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"

NS_MINIR_BEGIN

cwD3D11CubeTexture* cwD3D11CubeTexture::create(const CWSTRING& strTexture)
{
	cwD3D11CubeTexture* pTexture = new cwD3D11CubeTexture();
	if (pTexture && pTexture->init(strTexture)) {
		pTexture->autorelease();
		return pTexture;
	}
	
	CW_SAFE_RELEASE_NULL(pTexture);
	return nullptr;
}

cwD3D11CubeTexture* cwD3D11CubeTexture::createThreadSafe(const CWSTRING& strTexture)
{
	cwD3D11CubeTexture* pTexture = new cwD3D11CubeTexture();
	if (pTexture && pTexture->init(strTexture)) {
		return pTexture;
	}

	CW_SAFE_RELEASE_NULL(pTexture);
	return nullptr;
}

cwD3D11CubeTexture* cwD3D11CubeTexture::create(CWUINT iSize)
{
	cwD3D11CubeTexture* pTexture = new cwD3D11CubeTexture();
	if (pTexture && pTexture->init(iSize)) {
		pTexture->autorelease();
		return pTexture;
	}

	CW_SAFE_RELEASE_NULL(pTexture);
	return nullptr;
}

cwD3D11CubeTexture::cwD3D11CubeTexture():
m_pShaderResource(NULL),
m_pDepthStencilView(NULL)
{
	memset(m_pRenderTargetView, 0, sizeof(ID3D11RenderTargetView*)* 6);
}

cwD3D11CubeTexture::~cwD3D11CubeTexture()
{
	CW_RELEASE_COM(m_pShaderResource);
	CW_RELEASE_COM(m_pDepthStencilView);

	for (CWUINT i = 0; i < 6; ++i) {
		if (m_pRenderTargetView[i]) {
			CW_RELEASE_COM(m_pRenderTargetView[i]);
		}
	}
}

CWBOOL cwD3D11CubeTexture::init(const CWSTRING& strTexture)
{
	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	CWWSTRING wstrName = cwStringConvert::convertToWideChar(strTexture);

	ID3D11Texture2D* pTexture2D = NULL;
	CW_HR(D3DX11CreateTextureFromFile(pD3D11Device->getD3D11Device(), wstrName.c_str(), NULL, NULL, (ID3D11Resource**)&pTexture2D, NULL));

	D3D11_TEXTURE2D_DESC textureDesc;
	pTexture2D->GetDesc(&textureDesc);

	m_fWidth = static_cast<CWFLOAT>(textureDesc.Width);
	m_fHeight = static_cast<CWFLOAT>(textureDesc.Height);

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc;
	resourceDesc.Format = textureDesc.Format;
	resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	resourceDesc.TextureCube.MostDetailedMip = 0;
	resourceDesc.TextureCube.MipLevels = -1;

	pD3D11Device->getD3D11Device()->CreateShaderResourceView(pTexture2D, &resourceDesc, &m_pShaderResource);
	CW_RELEASE_COM(pTexture2D);

	return CWTRUE;
}

CWBOOL cwD3D11CubeTexture::init(CWUINT iSize)
{
	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());

	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = iSize;
	textureDesc.Height = iSize;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 6;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* pCubeTex = NULL;
	CW_HR(pD3D11Device->getD3D11Device()->CreateTexture2D(&textureDesc, 0, &pCubeTex));

	D3D11_RENDER_TARGET_VIEW_DESC renderDesc;
	renderDesc.Format = textureDesc.Format;
	renderDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	renderDesc.Texture2DArray.MipSlice = 0;
	renderDesc.Texture2DArray.ArraySize = 1;
	for (CWUINT i = 0; i < 6; ++i) {
		renderDesc.Texture2DArray.FirstArraySlice = i;
		CW_HR(pD3D11Device->getD3D11Device()->CreateRenderTargetView(pCubeTex, &renderDesc, &m_pRenderTargetView[i]));
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc;
	resourceDesc.Format = textureDesc.Format;
	resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	resourceDesc.TextureCube.MostDetailedMip = 0;
	resourceDesc.TextureCube.MipLevels = -1;

	pD3D11Device->getD3D11Device()->CreateShaderResourceView(pCubeTex, &resourceDesc, &m_pShaderResource);
	CW_RELEASE_COM(pCubeTex);

	buildDepthStencilView(iSize);

	m_fWidth = m_fHeight = (CWFLOAT)iSize;

	return CWTRUE;
}

CWVOID cwD3D11CubeTexture::buildDepthStencilView(CWUINT iSize)
{
	cwD3D11Device* pDevice = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	if (!pDevice) return;

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = iSize;
	texDesc.Height = iSize;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* pDepthTex = NULL;
	CW_HR(pDevice->getD3D11Device()->CreateTexture2D(&texDesc, NULL, &pDepthTex));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = texDesc.Format;
	dsvDesc.Flags = 0;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	CW_HR(pDevice->getD3D11Device()->CreateDepthStencilView(pDepthTex, &dsvDesc, &m_pDepthStencilView));
	CW_RELEASE_COM(pDepthTex);
}

CWHANDLE cwD3D11CubeTexture::getHandle() const
{
	return (CWHANDLE)m_pShaderResource;
}

CWHANDLE cwD3D11CubeTexture::getCubeFaceHandle(eCubeTextureFace eFace) const
{
	return (CWHANDLE)(m_pRenderTargetView[eFace]);
}

CWVOID cwD3D11CubeTexture::binding()
{
	ID3D11RenderTargetView* arrTargetView[1] = { m_pRenderTargetView[m_eCurrActiveFace] };

	cwD3D11Device* pDevice = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	pDevice->getD3D11DeviceContext()->OMSetRenderTargets(1, arrTargetView, m_pDepthStencilView);
}

CWVOID cwD3D11CubeTexture::beginDraw(CWBOOL bClearColor, CWBOOL bClearDepth, CWBOOL bClearStencil)
{
	cwD3D11Device* pDevice = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	if (bClearColor)
		pDevice->getD3D11DeviceContext()->ClearRenderTargetView(m_pRenderTargetView[m_eCurrActiveFace], (const CWFLOAT*)&cwColor::black);

	CWUINT nBit = 0;
	if (bClearDepth)
		nBit |= D3D11_CLEAR_DEPTH;
	if (bClearStencil)
		nBit |= D3D11_CLEAR_STENCIL;

	pDevice->getD3D11DeviceContext()->ClearDepthStencilView(m_pDepthStencilView, nBit, 1.0f, 0);
}

CWVOID cwD3D11CubeTexture::endDraw()
{

}

CWVOID cwD3D11CubeTexture::generateMips()
{
	if (m_pShaderResource) {
		cwD3D11Device* pDevice = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
		pDevice->getD3D11DeviceContext()->GenerateMips(m_pShaderResource);
	}
}

NS_MINIR_END

#endif
