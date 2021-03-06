﻿/*
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

#include "cwD3D11Stencil.h"
#include "Repertory/cwRepertory.h"
#include "Device/cwDevice.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"

NS_MINIR_BEGIN

cwD3D11Stencil* cwD3D11Stencil::create(const cwStencilData& stencilData)
{
	cwD3D11Stencil* pStencil = new cwD3D11Stencil();
	if (pStencil && pStencil->init(stencilData)) {
		pStencil->autorelease();
		return pStencil;
	}

	CW_SAFE_DELETE(pStencil);
	return nullptr;
}

cwD3D11Stencil::cwD3D11Stencil() :
m_pDepthStencilState(nullptr)
{

}

cwD3D11Stencil::~cwD3D11Stencil()
{
	CW_RELEASE_COM(m_pDepthStencilState);
}

bool cwD3D11Stencil::init(const cwStencilData& stencilData)
{
	m_nDepthStencilDesc.DepthEnable = stencilData.depthEnable;
	m_nDepthStencilDesc.DepthWriteMask = cwD3D11Device::getDepthWriteMask(stencilData.depthWriteMask);
	m_nDepthStencilDesc.DepthFunc = cwD3D11Device::getComparison(stencilData.depthFunc);

	m_nDepthStencilDesc.StencilEnable = stencilData.stencilEnable;
	m_nDepthStencilDesc.StencilReadMask = stencilData.stencilReadMask;
	m_nDepthStencilDesc.StencilWriteMask = stencilData.stencilWriteMask;

	m_nDepthStencilDesc.FrontFace.StencilFailOp = cwD3D11Device::getStencilOp(stencilData.frontStencilFailOp);
	m_nDepthStencilDesc.FrontFace.StencilDepthFailOp = cwD3D11Device::getStencilOp(stencilData.frontStencilDepthFailOp); 
	m_nDepthStencilDesc.FrontFace.StencilPassOp = cwD3D11Device::getStencilOp(stencilData.frontStencilPassOp);
	m_nDepthStencilDesc.FrontFace.StencilFunc = cwD3D11Device::getComparison(stencilData.frontStencilFunc); 

	m_nDepthStencilDesc.BackFace.StencilFailOp = cwD3D11Device::getStencilOp(stencilData.backStencilFailOp); 
	m_nDepthStencilDesc.BackFace.StencilDepthFailOp = cwD3D11Device::getStencilOp(stencilData.backStencilDepthFailOp); 
	m_nDepthStencilDesc.BackFace.StencilPassOp = cwD3D11Device::getStencilOp(stencilData.backStencilPassOp); 
	m_nDepthStencilDesc.BackFace.StencilFunc = cwD3D11Device::getComparison(stencilData.backStencilFunc); 

	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	CW_HR(pD3D11Device->getD3D11Device()->CreateDepthStencilState(&m_nDepthStencilDesc, &m_pDepthStencilState));

	return true;
}

NS_MINIR_END

#endif
