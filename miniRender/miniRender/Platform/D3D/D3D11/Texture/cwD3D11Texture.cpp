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

#include "cwD3D11Texture.h"
#include "Base/cwStringConvert.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"

NS_MINIR_BEGIN

cwD3D11Texture* cwD3D11Texture::create(ID3D11ShaderResourceView* pShaderResource)
{
	cwD3D11Texture* pTexture = new cwD3D11Texture();
	if (pTexture && pTexture->init(pShaderResource)) {
		pTexture->autorelease();
		return pTexture;
	}

	CW_SAFE_DELETE(pTexture);
	return nullptr;
}

cwD3D11Texture* cwD3D11Texture::create(const string& strFileName)
{
	cwD3D11Texture* pTexture = new cwD3D11Texture();
	if (pTexture && pTexture->init(strFileName)) {
		pTexture->autorelease();
		return pTexture;
	}

	CW_SAFE_DELETE(pTexture);
	return nullptr;
}

cwD3D11Texture* cwD3D11Texture::createThreadSafe(const CWSTRING& strFileName)
{
	cwD3D11Texture* pTexture = new cwD3D11Texture();
	if (pTexture && pTexture->init(strFileName)) {
		return pTexture;
	}

	CW_SAFE_DELETE(pTexture);
	return nullptr;
}

cwD3D11Texture* cwD3D11Texture::create(CWVOID* pData, CWUINT64 uSize)
{
	cwD3D11Texture* pTexture = new cwD3D11Texture();
	if (pTexture && pTexture->init(pData, uSize)) {
		pTexture->autorelease();
		return pTexture;
	}

	CW_SAFE_DELETE(pTexture);
	return nullptr;
}

cwD3D11Texture* cwD3D11Texture::createThreadSafe(CWVOID* pData, CWUINT64 uSize)
{
	cwD3D11Texture* pTexture = new cwD3D11Texture();
	if (pTexture && pTexture->init(pData, uSize)) {
		return pTexture;
	}

	CW_SAFE_DELETE(pTexture);
	return nullptr;
}

cwD3D11Texture* cwD3D11Texture::create(CWVOID* pData, CWUINT iWidth, CWUINT iHeight, CWUINT iElementSize, eFormat format)
{
	cwD3D11Texture* pTexture = new cwD3D11Texture();
	if (pTexture && pTexture->init(pData, iWidth, iHeight, iElementSize, format)) {
		pTexture->autorelease();
		return pTexture;
	}

	CW_SAFE_DELETE(pTexture);
	return nullptr;
}

cwD3D11Texture* cwD3D11Texture::createThreadSafe(CWVOID* pData, CWUINT iWidth, CWUINT iHeight, CWUINT iElementSize, eFormat format)
{
	cwD3D11Texture* pTexture = new cwD3D11Texture();
	if (pTexture && pTexture->init(pData, iWidth, iHeight, iElementSize, format)) {
		return pTexture;
	}

	CW_SAFE_DELETE(pTexture);
	return nullptr;
}

cwD3D11Texture* cwD3D11Texture::create(CWVOID* pData, const CW_TEXTURE_DESC& texDesc, const CW_SHADER_RESOURCE_VIEW_DESC& srvDesc, CWBOOL bThreading)
{
	cwD3D11Texture* pTexture = new cwD3D11Texture();
	if (pTexture && pTexture->init(pData, texDesc, srvDesc)) {
		if (!bThreading)
			pTexture->autorelease();
		return pTexture;
	}

	CW_SAFE_DELETE(pTexture);
	return nullptr;
}

cwD3D11Texture::cwD3D11Texture() :
m_pShaderResource(nullptr),
m_bShaderUsage(CWTRUE),
m_iMSAASamples(1)
{

}

cwD3D11Texture::~cwD3D11Texture()
{
	CW_RELEASE_COM(m_pShaderResource);
}

CWBOOL cwD3D11Texture::init(ID3D11ShaderResourceView* pShaderResource)
{
	m_pShaderResource = pShaderResource;
	if (m_pShaderResource) {
		m_pShaderResource->AddRef();
	}
	return CWTRUE;
}

CWBOOL cwD3D11Texture::init(const CWSTRING& strFileName)
{
	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	CWWSTRING wstrName = cwStringConvert::convertToWideChar(strFileName);

	ID3D11Texture2D* pTexture2D = NULL;
	CW_HR(D3DX11CreateTextureFromFile(pD3D11Device->getD3D11Device(), wstrName.c_str(), NULL, NULL, (ID3D11Resource**)&pTexture2D, NULL));

	D3D11_TEXTURE2D_DESC textureDesc;
	pTexture2D->GetDesc(&textureDesc);

	m_fWidth = static_cast<CWFLOAT>(textureDesc.Width);
	m_fHeight = static_cast<CWFLOAT>(textureDesc.Height);

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc;
	resourceDesc.Format = textureDesc.Format;
	resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Texture2D.MostDetailedMip = 0;
	resourceDesc.Texture2D.MipLevels = -1;

	pD3D11Device->getD3D11Device()->CreateShaderResourceView(pTexture2D, &resourceDesc, &m_pShaderResource);
	CW_RELEASE_COM(pTexture2D);

	return CWTRUE;
}

CWBOOL cwD3D11Texture::init(CWVOID* pData, CWUINT64 uSize)
{
	if (!pData) return CWFALSE;

	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());

	ID3D11Texture2D* pTexture2D = NULL;
	CW_HR(D3DX11CreateTextureFromMemory(pD3D11Device->getD3D11Device(), pData, uSize, NULL, NULL, (ID3D11Resource**)&pTexture2D, NULL));

	D3D11_TEXTURE2D_DESC textureDesc;
	pTexture2D->GetDesc(&textureDesc);

	m_fWidth = static_cast<CWFLOAT>(textureDesc.Width);
	m_fHeight = static_cast<CWFLOAT>(textureDesc.Height);

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc;
	resourceDesc.Format = textureDesc.Format;
	resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Texture2D.MostDetailedMip = 0;
	resourceDesc.Texture2D.MipLevels = -1;

	pD3D11Device->getD3D11Device()->CreateShaderResourceView(pTexture2D, &resourceDesc, &m_pShaderResource);
	CW_RELEASE_COM(pTexture2D);

	return CWTRUE;
}

CWBOOL cwD3D11Texture::init(CWVOID* pData, CWUINT iWidth, CWUINT iHeight, CWUINT iElementSize, eFormat format)
{
	if (!pData) return CWFALSE;

	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = iWidth;
	texDesc.Height = iHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = pD3D11Device->getFormatType(format);
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = pData;
	subData.SysMemPitch = iWidth*iElementSize;
	subData.SysMemSlicePitch = 0;

	ID3D11Texture2D* pTexture2D = nullptr;
	CW_HR(pD3D11Device->getD3D11Device()->CreateTexture2D(&texDesc, &subData, &pTexture2D));

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc;
	resourceDesc.Format = texDesc.Format;
	resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Texture2D.MostDetailedMip = 0;
	resourceDesc.Texture2D.MipLevels = -1;

	pD3D11Device->getD3D11Device()->CreateShaderResourceView(pTexture2D, &resourceDesc, &m_pShaderResource);
	CW_RELEASE_COM(pTexture2D);

	m_fWidth = static_cast<CWFLOAT>(iWidth);
	m_fHeight = static_cast<CWFLOAT>(iHeight);

	return CWTRUE;
}

CWBOOL cwD3D11Texture::init(CWVOID* pData, const CW_TEXTURE_DESC& texDesc, const CW_SHADER_RESOURCE_VIEW_DESC& srvDesc)
{
	if (!pData) return CWFALSE;

	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());

	D3D11_TEXTURE2D_DESC d3d11TexDesc;
	memset(&d3d11TexDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));

	d3d11TexDesc.Width = texDesc.iWidth;
	d3d11TexDesc.Height = texDesc.iHeight;
	d3d11TexDesc.MipLevels = texDesc.iMipLevels;
	d3d11TexDesc.ArraySize = texDesc.iArraySize;
	d3d11TexDesc.Format = pD3D11Device->getFormatType(texDesc.nFormat);
	d3d11TexDesc.SampleDesc.Count = texDesc.iMultiSamplingCount;
	d3d11TexDesc.SampleDesc.Quality = texDesc.iMultiSamplingQuality;
	d3d11TexDesc.Usage = pD3D11Device->getBufferUsage(texDesc.nUsage);//D3D11_USAGE_DEFAULT;
	d3d11TexDesc.BindFlags = pD3D11Device->getBufferBindFlags(texDesc.iBindFlags);//D3D11_BIND_SHADER_RESOURCE;
	d3d11TexDesc.CPUAccessFlags = texDesc.iCpuAccessFlags;
	d3d11TexDesc.MiscFlags = texDesc.iMiscFlags;

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = pData;
	subData.SysMemPitch = texDesc.iWidth*pD3D11Device->getFormatTypeSize(texDesc.nFormat);
	subData.SysMemSlicePitch = 0;

	ID3D11Texture2D* pTexture2D = nullptr;
	CW_HR(pD3D11Device->getD3D11Device()->CreateTexture2D(&d3d11TexDesc, &subData, &pTexture2D));

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc;
	resourceDesc.Format = pD3D11Device->getFormatType(srvDesc.nFormat);
	resourceDesc.ViewDimension = pD3D11Device->getShaderResourceViewDimension(srvDesc.nSRVDimension);
	resourceDesc.Texture2D.MostDetailedMip = srvDesc.iMostDetailedMip;
	resourceDesc.Texture2D.MipLevels = srvDesc.iMipLevels;

	pD3D11Device->getD3D11Device()->CreateShaderResourceView(pTexture2D, &resourceDesc, &m_pShaderResource);
	CW_RELEASE_COM(pTexture2D);

	m_fWidth = static_cast<CWFLOAT>(texDesc.iWidth);
	m_fHeight = static_cast<CWFLOAT>(texDesc.iHeight);

	return CWTRUE;
}

CWHANDLE cwD3D11Texture::getHandle() const
{
	return (CWHANDLE)m_pShaderResource;
}

NS_MINIR_END

#endif
