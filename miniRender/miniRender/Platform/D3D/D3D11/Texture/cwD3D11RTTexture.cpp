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

#include "cwD3D11RTTexture.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"

NS_MINIR_BEGIN

cwD3D11RTTexture* cwD3D11RTTexture::create(CWBOOL bThreadSafe)
{
	cwD3D11RTTexture* pTexture = new cwD3D11RTTexture();
	if (pTexture && pTexture->init()) {
		if (!bThreadSafe)
			pTexture->autorelease();
		return pTexture;
	}

	CW_SAFE_DELETE(pTexture);
	return nullptr;
}

cwD3D11RTTexture* cwD3D11RTTexture::create(CWFLOAT fWidth, CWFLOAT fHeight, eFormat format, CWBOOL bShaderUsage, CWBOOL bThreadSafe)
{
	cwD3D11RTTexture* pTexture = new cwD3D11RTTexture();
	if (pTexture && pTexture->init(fWidth, fHeight, format, bShaderUsage)) {
		if (!bThreadSafe)
			pTexture->autorelease();
		return pTexture;
	}
	
	CW_SAFE_DELETE(pTexture);
	return nullptr;
}

cwD3D11RTTexture::cwD3D11RTTexture() :
m_pRenderTargetView(nullptr)
{

}

cwD3D11RTTexture::~cwD3D11RTTexture()
{
	CW_RELEASE_COM(m_pRenderTargetView);
}

CWBOOL cwD3D11RTTexture::init()
{
	m_bShaderUsage = CWFALSE;

	if (!buildRenderTargetBackBuffer()) return CWFALSE;

	return CWTRUE;
}

CWBOOL cwD3D11RTTexture::init(CWFLOAT fWidth, CWFLOAT fHeight, eFormat format, CWBOOL bShaderUsage)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_eTextureFormat = format;
	m_bShaderUsage = bShaderUsage;

	if (!buildRenderTarget()) return CWFALSE;
	return CWTRUE;
}

CWHANDLE cwD3D11RTTexture::getRenderHandle() const
{
	return static_cast<CWHANDLE>(m_pRenderTargetView);
}

std::vector<CWHANDLE>* cwD3D11RTTexture::getRenderHandleArray() const
{
	static std::vector<CWHANDLE> vecHandle(1);

	vecHandle[0] = static_cast<CWHANDLE>(m_pRenderTargetView);
	return &vecHandle;
}

CWBOOL cwD3D11RTTexture::buildRenderTargetBackBuffer()
{
	cwD3D11Device* pDevice = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	if (!pDevice) return CWFALSE;

	ID3D11Texture2D* backBuffer = NULL;
	CW_HR(pDevice->getSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	CW_HR(pDevice->getD3D11Device()->CreateRenderTargetView(backBuffer, NULL, &m_pRenderTargetView));

	D3D11_TEXTURE2D_DESC texDesc;
	backBuffer->GetDesc(&texDesc);

	m_fWidth = static_cast<CWFLOAT>(texDesc.Width);
	m_fHeight = static_cast<CWFLOAT>(texDesc.Height);
	m_eTextureFormat = pDevice->getFormatTypeDXGI(texDesc.Format);

	m_nTextureDesc.iWidth = texDesc.Width;
	m_nTextureDesc.iHeight = texDesc.Height;

	CW_RELEASE_COM(backBuffer);

	return CWTRUE;
}

CWBOOL cwD3D11RTTexture::buildRenderTarget()
{
	cwD3D11Device* pDevice = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	if (!pDevice) return CWFALSE;

	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = static_cast<UINT>(m_fWidth);
	texDesc.Height = static_cast<UINT>(m_fHeight);
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = pDevice->getFormatType(m_eTextureFormat);
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	if (m_bShaderUsage)
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	else
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET;

	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* pTex = NULL;
	CW_HR(pDevice->getD3D11Device()->CreateTexture2D(&texDesc, NULL, &pTex));

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	CW_HR(pDevice->getD3D11Device()->CreateRenderTargetView(pTex, &rtvDesc, &m_pRenderTargetView));

	if (m_bShaderUsage) {
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		CW_HR(pDevice->getD3D11Device()->CreateShaderResourceView(pTex, &srvDesc, &m_pShaderResource));
	}

	CW_RELEASE_COM(pTex);

	return CWTRUE;
}

const CW_TEXTURE_DESC* cwD3D11RTTexture::getTextureDesc() const
{
	return &m_nTextureDesc;
}

NS_MINIR_END

#endif
