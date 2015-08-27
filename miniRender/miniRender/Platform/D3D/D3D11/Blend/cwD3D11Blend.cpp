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

#include "cwD3D11Blend.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"

NS_MINIR_BEGIN

cwD3D11Blend* cwD3D11Blend::create(const cwBlendData& blendData)
{
	cwD3D11Blend* pBlend = new cwD3D11Blend();
	if (pBlend && pBlend->init(blendData)) {
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

bool cwD3D11Blend::init(const cwBlendData& blendData)
{
	memset(&m_nBlendDesc, 0, sizeof(m_nBlendDesc));

	m_nBlendDesc.AlphaToCoverageEnable = blendData.bAlphaToCoverage;
	m_nBlendDesc.IndependentBlendEnable = blendData.bIndependentBlend;
	m_nBlendDesc.RenderTarget[0].BlendEnable = blendData.bEnable;
	m_nBlendDesc.RenderTarget[0].SrcBlend = cwD3D11Device::getBlendFactor(blendData.srcBlend);
	m_nBlendDesc.RenderTarget[0].DestBlend = cwD3D11Device::getBlendFactor(blendData.dstBlend); 
	m_nBlendDesc.RenderTarget[0].BlendOp = cwD3D11Device::getBlendOp(blendData.blendOp);
	m_nBlendDesc.RenderTarget[0].SrcBlendAlpha = cwD3D11Device::getBlendFactor(blendData.srcBlendAlpha);
	m_nBlendDesc.RenderTarget[0].DestBlendAlpha = cwD3D11Device::getBlendFactor(blendData.dstBlendAlpha);
	m_nBlendDesc.RenderTarget[0].BlendOpAlpha = cwD3D11Device::getBlendOp(blendData.blendOpAlpha); 
	m_nBlendDesc.RenderTarget[0].RenderTargetWriteMask = blendData.uColorEnable;

	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	CW_HR(pD3D11Device->getD3D11Device()->CreateBlendState(&m_nBlendDesc, &m_pBlendState));

	return true;
}

NS_MINIR_END

#endif
