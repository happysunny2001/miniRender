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

#include "cwD3D11RWTexture.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Repertory/cwRepertory.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"

NS_MINIR_BEGIN

cwD3D11RWTexture* cwD3D11RWTexture::create(CWFLOAT fWidth, CWFLOAT fHeight, eFormat format, CWBOOL bThreadSafe)
{
	cwD3D11RWTexture* pTexture = new cwD3D11RWTexture();
	if (pTexture && pTexture->init(fWidth, fHeight, format)) {
		if (!bThreadSafe)
			pTexture->autorelease();
		return pTexture;
	}

	CW_SAFE_DELETE(pTexture);
	return pTexture;
}

cwD3D11RWTexture* cwD3D11RWTexture::create(CWFLOAT fWidth, CWFLOAT fHeight, eFormat format, CWUINT iMSAASamples, CWBOOL bThreadSafe)
{
	cwD3D11RWTexture* pTexture = new cwD3D11RWTexture();
	if (pTexture && pTexture->init(fWidth, fHeight, format, iMSAASamples)) {
		if (!bThreadSafe)
			pTexture->autorelease();
		return pTexture;
	}

	CW_SAFE_DELETE(pTexture);
	return pTexture;
}

cwD3D11RWTexture::cwD3D11RWTexture() :
m_pUnorderedAccessView(NULL)
{

}

cwD3D11RWTexture::~cwD3D11RWTexture()
{
	CW_RELEASE_COM(m_pUnorderedAccessView);
	m_eTextureFormat = eFormatUnknown;
}

CWBOOL cwD3D11RWTexture::init(CWFLOAT fWidth, CWFLOAT fHeight, eFormat format)
{
	m_eTextureFormat = format;
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	return buildTexture();
}

CWBOOL cwD3D11RWTexture::init(CWFLOAT fWidth, CWFLOAT fHeight, eFormat format, CWUINT iMSAASamples)
{
	m_eTextureFormat = format;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_iMSAASamples = iMSAASamples == 4 || iMSAASamples == 8 ? iMSAASamples : 1;

	return buildTexture();
}

CWBOOL cwD3D11RWTexture::buildTexture()
{
	cwD3D11Device* pDevice = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());

	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));

	DXGI_FORMAT d3d11Format = pDevice->getFormatType(m_eTextureFormat);

	texDesc.Width = static_cast<UINT>(m_fWidth);
	texDesc.Height = static_cast<UINT>(m_fHeight);
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = d3d11Format;
	texDesc.SampleDesc.Count = m_iMSAASamples;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* pTex = NULL;

	CW_HR(pDevice->getD3D11Device()->CreateTexture2D(&texDesc, NULL, &pTex));

	if (m_iMSAASamples == 1) {
		CW_HR(pDevice->getD3D11Device()->CreateShaderResourceView(pTex, NULL, &m_pShaderResource));
	}
	else {
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		memset(&srvDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srvDesc.Format = d3d11Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		CW_HR(pDevice->getD3D11Device()->CreateShaderResourceView(pTex, &srvDesc, &m_pShaderResource));
	}

	if (m_iMSAASamples == 1) {
		CW_HR(pDevice->getD3D11Device()->CreateUnorderedAccessView(pTex, NULL, &m_pUnorderedAccessView));
	}
	else {
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		memset(&uavDesc, 0, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

		uavDesc.Format = d3d11Format;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_UNKNOWN;
		uavDesc.Texture2D.MipSlice = 0;

		CW_HR(pDevice->getD3D11Device()->CreateUnorderedAccessView(pTex, &uavDesc, &m_pUnorderedAccessView));
	}

	CW_RELEASE_COM(pTex);

	return CWTRUE;
}

CWHANDLE cwD3D11RWTexture::getWritablehandle() const
{
	return static_cast<CWHANDLE>(m_pUnorderedAccessView);
}

NS_MINIR_END

#endif
