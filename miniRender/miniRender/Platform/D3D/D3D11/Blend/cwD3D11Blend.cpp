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

#include "cwD3D11Blend.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"

NS_MINI_BEGIN

cwD3D11Blend* cwD3D11Blend::create(
bool bEnable, eBlendFactor srcBlend, eBlendFactor dstBlend, eBlendOp blendOp,
eBlendFactor srcBlendAlpha, eBlendFactor dstBlendAlpha, eBlendOp blendOpAlpha,
eColorWriteEnable renderWriteMask)
{
	cwD3D11Blend* pBlend = new cwD3D11Blend();
	if (pBlend && pBlend->init(bEnable, srcBlend, dstBlend, blendOp, srcBlendAlpha, dstBlendAlpha, blendOpAlpha, renderWriteMask)) {
		pBlend->autorelease();
		return pBlend;
	}

	CW_SAFE_DELETE(pBlend);
	return nullptr;
}

cwD3D11Blend::cwD3D11Blend() :
m_pBlendState(nullptr)
{
	
}

cwD3D11Blend::~cwD3D11Blend()
{
	CW_RELEASE_COM(m_pBlendState);
}

bool cwD3D11Blend::init(
	bool bEnable, eBlendFactor srcBlend, eBlendFactor dstBlend, eBlendOp blendOp,
	eBlendFactor srcBlendAlpha, eBlendFactor dstBlendAlpha, eBlendOp blendOpAlpha,
	eColorWriteEnable renderWriteMask)
{
	memset(&m_nBlendDesc, 0, sizeof(m_nBlendDesc));

	m_nBlendDesc.AlphaToCoverageEnable  = FALSE;
	m_nBlendDesc.IndependentBlendEnable = FALSE;
	m_nBlendDesc.RenderTarget[0].BlendEnable           = bEnable;
	m_nBlendDesc.RenderTarget[0].SrcBlend              = static_cast<D3D11_BLEND>(srcBlend);
	m_nBlendDesc.RenderTarget[0].DestBlend             = static_cast<D3D11_BLEND>(dstBlend);
	m_nBlendDesc.RenderTarget[0].BlendOp               = static_cast<D3D11_BLEND_OP>(blendOp);
	m_nBlendDesc.RenderTarget[0].SrcBlendAlpha         = static_cast<D3D11_BLEND>(srcBlendAlpha);
	m_nBlendDesc.RenderTarget[0].DestBlendAlpha        = static_cast<D3D11_BLEND>(dstBlendAlpha);
	m_nBlendDesc.RenderTarget[0].BlendOpAlpha          = static_cast<D3D11_BLEND_OP>(blendOpAlpha);
	m_nBlendDesc.RenderTarget[0].RenderTargetWriteMask = renderWriteMask;

	ID3D11Device* pD3D11Device = static_cast<ID3D11Device*>(cwRepertory::getInstance().getDevice()->getDevice());
	CW_HR(pD3D11Device->CreateBlendState(&m_nBlendDesc, &m_pBlendState));

	return true;
}

NS_MINI_END