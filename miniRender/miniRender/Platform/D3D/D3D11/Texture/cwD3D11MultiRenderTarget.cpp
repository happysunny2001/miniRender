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

#include "cwD3D11MultiRenderTarget.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Repertory/cwRepertory.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"
#include "cwD3D11Texture.h"
#include "Base/cwColor.h"

NS_MINIR_BEGIN

cwD3D11MultiRenderTarget* cwD3D11MultiRenderTarget::create(CWFLOAT fWidth, CWFLOAT fHeight, CWBOOL bThreading)
{
	cwD3D11MultiRenderTarget* pTexture = new cwD3D11MultiRenderTarget();
	if (pTexture && pTexture->init(fWidth, fHeight)) {
		if (!bThreading)
			pTexture->autorelease();
		return pTexture;
	}

	CW_SAFE_DELETE(pTexture);
	return nullptr;
}

cwD3D11MultiRenderTarget::cwD3D11MultiRenderTarget() :
m_pDepthStencilBuffer(NULL),
m_pDepthStencilView(NULL),
m_pDepthShaderResource(NULL),
m_iGBufCount(0),
m_bDepthShaderResource(CWFALSE),
m_pDepthTexture(nullptr)
{
	m_eType = eRenderTextureMultiTarget;

	for (CWUINT i = 0; i < 4; ++i) {
		m_ppRenderTargetTextures[i] = NULL;
		m_ppRenderTargets[i] = NULL;
		m_ppShaderResources[i] = NULL;
	}
}

cwD3D11MultiRenderTarget::~cwD3D11MultiRenderTarget()
{
	releaseAll();
}

CWVOID cwD3D11MultiRenderTarget::releaseAll()
{
	CW_RELEASE_COM(m_pDepthStencilBuffer);
	CW_RELEASE_COM(m_pDepthStencilView);
	CW_RELEASE_COM(m_pDepthShaderResource);
	CW_SAFE_RELEASE_NULL(m_pDepthTexture);

	if (m_ppRenderTargetTextures) {
		for (CWUINT i = 0; i < m_iGBufCount; ++i) {
			CW_RELEASE_COM(m_ppRenderTargetTextures[i]);
		}
	}

	if (m_ppRenderTargets) {
		for (CWUINT i = 0; i < m_iGBufCount; ++i) {
			CW_RELEASE_COM(m_ppRenderTargets[i]);
		}
	}

	if (m_ppShaderResources) {
		for (CWUINT i = 0; i < m_iGBufCount; ++i) {
			CW_RELEASE_COM(m_ppShaderResources[i]);
		}
	}

	m_iGBufCount = 0;
	m_nVecTextures.clear();
}

CWBOOL cwD3D11MultiRenderTarget::init(CWFLOAT fWidth, CWFLOAT fHeight)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_nClearColor = cwColor::black;

	return CWTRUE;
}

CWVOID cwD3D11MultiRenderTarget::beginResize()
{
	//releaseAll();
}

CWBOOL cwD3D11MultiRenderTarget::onResize(CWBOOL bForce)
{
	if (!bForce && m_fWidth > 1.0f) return CWTRUE;

	//if (m_iGBufCount > 0) {
	//	m_ppRenderTargets = new ID3D11RenderTargetView*[m_iGBufCount];
	//	m_ppShaderResources = new ID3D11ShaderResourceView*[m_iGBufCount];
	//	m_ppRenderTargetTextures = new ID3D11Texture2D*[m_iGBufCount];

	//	memset(m_ppRenderTargets, 0, sizeof(ID3D11RenderTargetView*)*m_iGBufCount);
	//	memset(m_ppShaderResources, 0, sizeof(ID3D11ShaderResourceView*)*m_iGBufCount);
	//	memset(m_ppRenderTargetTextures, 0, sizeof(ID3D11Texture2D*)*m_iGBufCount);
	//}

	//this->buildDepthStencilBuffer();

	return CWTRUE;
}

CWVOID cwD3D11MultiRenderTarget::enableDepthBufferShaderResource()
{
	m_bDepthShaderResource = CWTRUE;
}

CWVOID cwD3D11MultiRenderTarget::setRenderTargetCount(CWUINT iCount)
{
	releaseAll();

	m_iGBufCount = iCount;
	for (CWUINT i = 0; i < m_iGBufCount; ++i) {
		m_nVecTextures.pushBack(nullptr);
	}

	//this->onResize(CWTRUE);
	this->buildDepthStencilBuffer();
}

CWVOID cwD3D11MultiRenderTarget::setRenderTarget(eFormat format, CWUINT index)
{
	if (index >= m_iGBufCount) return;

	cwD3D11Device* pDevice = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	if (!pDevice) return;

	CW_RELEASE_COM(m_ppRenderTargetTextures[index]);
	CW_RELEASE_COM(m_ppRenderTargets[index]);
	CW_RELEASE_COM(m_ppShaderResources[index]);

	CWUINT texWidth = 0;
	CWUINT texHeight = 0;

	if (m_fWidth > 1.0f) {
		texWidth = static_cast<CWUINT>(m_fWidth);
		texHeight = static_cast<CWUINT>(m_fHeight);
	}
	else {
		CWUINT winWidth = cwRepertory::getInstance().getUInt(gValueWinWidth);
		CWUINT winHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);

		texWidth = static_cast<CWUINT>(m_fWidth*winWidth);
		texHeight = static_cast<CWUINT>(m_fHeight*winHeight);
	}

	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = texWidth;
	texDesc.Height = texHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = pDevice->getFormatType(format);
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	CW_HR(pDevice->getD3D11Device()->CreateTexture2D(&texDesc, NULL, &(m_ppRenderTargetTextures[index])));

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	CW_HR(pDevice->getD3D11Device()->CreateRenderTargetView(m_ppRenderTargetTextures[index], NULL, &(m_ppRenderTargets[index])));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	CW_HR(pDevice->getD3D11Device()->CreateShaderResourceView(m_ppRenderTargetTextures[index], &srvDesc, &(m_ppShaderResources[index])));

	cwD3D11Texture* pTexture = cwD3D11Texture::create(m_ppShaderResources[index]);
	m_nVecTextures.replace(pTexture, index);
}

CWHANDLE cwD3D11MultiRenderTarget::getRenderTargetHandle(CWUINT index) const
{
	if (index >= m_iGBufCount) return NULL;
	return static_cast<CWHANDLE>(m_ppRenderTargets[index]);
}

CWHANDLE cwD3D11MultiRenderTarget::getShaderResourceHandle(CWUINT index) const
{
	if (index >= m_iGBufCount) return NULL;
	return static_cast<CWHANDLE>(m_ppShaderResources[index]);
}

cwTexture* cwD3D11MultiRenderTarget::getRenderTexture(CWINT index) const
{
	if (index >= m_iGBufCount || index < -1) return nullptr;
	if (index == -1) return m_pDepthTexture;
	return m_nVecTextures.at(index);
}

CWVOID cwD3D11MultiRenderTarget::buildDepthStencilBuffer()
{
	CW_RELEASE_COM(m_pDepthStencilBuffer);
	CW_RELEASE_COM(m_pDepthStencilView);

	cwD3D11Device* pDevice = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	if (!pDevice) return;

	CWUINT texWidth = 0;
	CWUINT texHeight = 0;

	if (m_fWidth > 1.0f) {
		texWidth = static_cast<CWUINT>(m_fWidth);
		texHeight = static_cast<CWUINT>(m_fHeight);
	}
	else {
		CWUINT winWidth = cwRepertory::getInstance().getUInt(gValueWinWidth);
		CWUINT winHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);

		texWidth = static_cast<CWUINT>(m_fWidth*winWidth);
		texHeight = static_cast<CWUINT>(m_fHeight*winHeight);
	}

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = texWidth;
	texDesc.Height = texHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	if (!m_bDepthShaderResource) {
		texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	}
	else {
		texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	}

	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	CW_HR(pDevice->getD3D11Device()->CreateTexture2D(&texDesc, NULL, &m_pDepthStencilBuffer));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	memset(&dsvDesc, 0, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	CW_HR(pDevice->getD3D11Device()->CreateDepthStencilView(m_pDepthStencilBuffer, &dsvDesc, &m_pDepthStencilView));

	if (m_bDepthShaderResource) {
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		CW_HR(pDevice->getD3D11Device()->CreateShaderResourceView(m_pDepthStencilBuffer, &srvDesc, &m_pDepthShaderResource));

		m_pDepthTexture = cwD3D11Texture::create(m_pDepthShaderResource);
		CW_SAFE_RETAIN(m_pDepthTexture);
	}
}

CWVOID cwD3D11MultiRenderTarget::binding()
{
	cwD3D11Device* pDevice = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	pDevice->getD3D11DeviceContext()->OMSetRenderTargets(m_iGBufCount, m_ppRenderTargets, m_pDepthStencilView);
}

CWVOID cwD3D11MultiRenderTarget::beginDraw(CWBOOL bClearColor, CWBOOL bClearDepth, CWBOOL bClearStencil)
{
	cwD3D11Device* pDevice = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	if (bClearColor && m_iGBufCount > 0) {
		for (CWUINT i = 0; i < m_iGBufCount; ++i) {
			ID3D11RenderTargetView* p = m_ppRenderTargets[i];
			pDevice->getD3D11DeviceContext()->ClearRenderTargetView(m_ppRenderTargets[i], (const CWFLOAT*)&m_nClearColor);
		}
	}

	CWUINT nBit = 0;
	if (bClearDepth)
		nBit |= D3D11_CLEAR_DEPTH;
	if (bClearStencil)
		nBit |= D3D11_CLEAR_STENCIL;

	pDevice->getD3D11DeviceContext()->ClearDepthStencilView(m_pDepthStencilView, nBit, 1.0f, 0);
}

CWVOID cwD3D11MultiRenderTarget::endDraw()
{

}

NS_MINIR_END

#endif
