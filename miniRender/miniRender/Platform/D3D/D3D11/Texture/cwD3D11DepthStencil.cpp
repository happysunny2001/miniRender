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

#include "cwD3D11DepthStencil.h"
#include "Repertory/cwRepertory.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"

NS_MINIR_BEGIN

cwD3D11DepthStencil* cwD3D11DepthStencil::create()
{
	cwD3D11DepthStencil* pDepthStencil = new cwD3D11DepthStencil();
	if (pDepthStencil && pDepthStencil->init(1.0f, 1.0f)) {
		pDepthStencil->autorelease();
		return pDepthStencil;
	}

	CW_SAFE_DELETE(pDepthStencil);
	return nullptr;
}

cwD3D11DepthStencil::cwD3D11DepthStencil():
m_pDepthStencilBuffer(NULL),
m_pDepthStencilView(NULL)
{

}

cwD3D11DepthStencil::~cwD3D11DepthStencil()
{
	CW_RELEASE_COM(m_pDepthStencilBuffer);
	CW_RELEASE_COM(m_pDepthStencilView);
}

bool cwD3D11DepthStencil::init(CWFLOAT fWidth, CWFLOAT fHeight)
{
	return cwRenderTexture::init(fWidth, fHeight);
}

void cwD3D11DepthStencil::beginResize()
{
	CW_RELEASE_COM(m_pDepthStencilBuffer);
	CW_RELEASE_COM(m_pDepthStencilView);
}

bool cwD3D11DepthStencil::onResize(bool bForce)
{
	CW_RELEASE_COM(m_pDepthStencilBuffer);
	CW_RELEASE_COM(m_pDepthStencilView);

	cwD3D11Device* pDevice = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	if (!pDevice) return false;

	CWUINT winWidth = cwRepertory::getInstance().getUInt(gValueWinWidth);
	CWUINT winHeight = cwRepertory::getInstance().getUInt(gValueWinHeight);

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = winWidth;
	texDesc.Height = winHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (pDevice->getEnableMsaa4X()) {
		texDesc.SampleDesc.Count = 4;
		texDesc.SampleDesc.Quality = pDevice->getM4xMassQuality() - 1;
	}
	else {
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
	}
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	CW_HR(pDevice->getD3D11Device()->CreateTexture2D(&texDesc, NULL, &m_pDepthStencilBuffer));
	CW_HR(pDevice->getD3D11Device()->CreateDepthStencilView(m_pDepthStencilBuffer, NULL, &m_pDepthStencilView));

	return true;
}

CWHANDLE cwD3D11DepthStencil::getRenderTargetPtr()
{
	return static_cast<CWHANDLE>(m_pDepthStencilView);
}

CWHANDLE cwD3D11DepthStencil::getTexturePtr()
{
	return NULL;
}

CWHANDLE cwD3D11DepthStencil::getTextureMultiThreadPtr()
{
	return NULL;
}

NS_MINIR_END

#endif