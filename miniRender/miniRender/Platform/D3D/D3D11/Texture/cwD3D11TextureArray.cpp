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

#include "cwD3D11TextureArray.h"
#include "Base/cwStringConvert.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Resource/cwResourceLoader.h"

NS_MINIR_BEGIN

cwD3D11TextureArray* cwD3D11TextureArray::create(const std::vector<CWSTRING>& vecFiles)
{
	cwD3D11TextureArray* pTexArray = new cwD3D11TextureArray();
	if (pTexArray && pTexArray->init(vecFiles)) {
		pTexArray->autorelease();
		return pTexArray;
	}

	CW_SAFE_DELETE(pTexArray);
	return nullptr;
}

cwD3D11TextureArray* cwD3D11TextureArray::createThreadSafe(const std::vector<CWSTRING>& vecFiles)
{
	cwD3D11TextureArray* pTexArray = new cwD3D11TextureArray();
	if (pTexArray && pTexArray->init(vecFiles)) {
		return pTexArray;
	}

	CW_SAFE_DELETE(pTexArray);
	return nullptr;
}

cwD3D11TextureArray::cwD3D11TextureArray():
m_pShaderResource(NULL)
{

}

cwD3D11TextureArray::~cwD3D11TextureArray()
{
	CW_RELEASE_COM(m_pShaderResource);
}

CWBOOL cwD3D11TextureArray::init(const std::vector<CWSTRING>& vecFiles)
{
	if (!cwTextureArray::init(vecFiles)) return CWFALSE;

	CWUINT iSize = (CWUINT)m_nVecFiles.size();
	if (iSize == 0) return false;

	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	cwResourceLoader* pResLoader = cwRepertory::getInstance().getResourceLoader();

	std::vector<ID3D11Texture2D*> vecD3DTex(iSize);
	for (CWUINT i = 0; i < iSize; ++i) {
		D3DX11_IMAGE_LOAD_INFO imgInfo;

		imgInfo.Width = D3DX11_FROM_FILE;
		imgInfo.Height = D3DX11_FROM_FILE;
		imgInfo.Depth = D3DX11_FROM_FILE;
		imgInfo.FirstMipLevel = 0;
		imgInfo.MipLevels = D3DX11_FROM_FILE;
		imgInfo.Usage = D3D11_USAGE_STAGING;
		imgInfo.BindFlags = 0;
		imgInfo.CpuAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		imgInfo.MiscFlags = 0;
		imgInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		imgInfo.Filter = D3DX11_FILTER_NONE;
		imgInfo.MipFilter = D3DX11_FILTER_LINEAR;
		imgInfo.pSrcInfo = 0;

		cwData* pTextureData = pResLoader->getTextureData(m_nVecFiles[i]);
		if (pTextureData) {
			if (pTextureData->m_pData && pTextureData->m_uSize > 0) {
				CW_HR(D3DX11CreateTextureFromMemory(pD3D11Device->getD3D11Device(), 
					pTextureData->m_pData, 
					pTextureData->m_uSize, 
					&imgInfo, 
					NULL, 
					(ID3D11Resource**)&(vecD3DTex[i]), 
					NULL));
			}

			delete pTextureData;
		}
	}

	D3D11_TEXTURE2D_DESC texDesc;
	vecD3DTex[0]->GetDesc(&texDesc);

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width              = texDesc.Width;
	texArrayDesc.Height             = texDesc.Height;
	texArrayDesc.MipLevels          = texDesc.MipLevels;
	texArrayDesc.ArraySize          = iSize;
	texArrayDesc.Format             = texDesc.Format;
	texArrayDesc.SampleDesc.Count   = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage              = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags     = 0;
	texArrayDesc.MiscFlags          = 0;

	ID3D11Texture2D* pTexArray = NULL;
	CW_HR(pD3D11Device->getD3D11Device()->CreateTexture2D(&texArrayDesc, NULL, &pTexArray));

	for (CWUINT iTex = 0; iTex < iSize; ++iTex) {
		for (CWUINT iMip = 0; iMip < texDesc.MipLevels; ++iMip) {
			D3D11_MAPPED_SUBRESOURCE mapRes;
			CW_HR(pD3D11Device->getD3D11DeviceContext()->Map(vecD3DTex[iTex], iMip, D3D11_MAP_READ, 0, &mapRes));

			pD3D11Device->getD3D11DeviceContext()->UpdateSubresource(
				pTexArray,
				D3D11CalcSubresource(iMip, iTex, texDesc.MipLevels),
				NULL,
				mapRes.pData, mapRes.RowPitch, mapRes.DepthPitch
				);

			pD3D11Device->getD3D11DeviceContext()->Unmap(vecD3DTex[iTex], iMip);
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC resViewDesc;
	resViewDesc.Format = texArrayDesc.Format;
	resViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	resViewDesc.Texture2DArray.MostDetailedMip = 0;
	resViewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	resViewDesc.Texture2DArray.FirstArraySlice = 0;
	resViewDesc.Texture2DArray.ArraySize = iSize;

	CW_HR(pD3D11Device->getD3D11Device()->CreateShaderResourceView(pTexArray, &resViewDesc, &m_pShaderResource));

	CW_RELEASE_COM(pTexArray);
	for (CWUINT iTex = 0; iTex < iSize; ++iTex) {
		CW_RELEASE_COM(vecD3DTex[iTex]);
	}

	return CWTRUE;
}

CWHANDLE cwD3D11TextureArray::getHandle() const
{
	return (CWHANDLE)m_pShaderResource;
}

NS_MINIR_END

#endif
