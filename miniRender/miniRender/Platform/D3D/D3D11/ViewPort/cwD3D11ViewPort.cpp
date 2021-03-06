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

#include "cwD3D11ViewPort.h"
#include "Repertory/cwRepertory.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"

NS_MINIR_BEGIN

cwD3D11ViewPort* cwD3D11ViewPort::create(CWFLOAT fTopLeftX, CWFLOAT fTopLeftY,
										 CWFLOAT fWidth, CWFLOAT fHeight,
										 CWFLOAT fMinDepth, CWFLOAT fMaxDepth,
										 CWBOOL bThreading)
{
	cwD3D11ViewPort* pViewPort = new cwD3D11ViewPort();
	if (pViewPort && pViewPort->init(fTopLeftX, fTopLeftY, fWidth, fHeight, fMinDepth, fMaxDepth)) {
		if (!bThreading)
			pViewPort->autorelease();
		return pViewPort;
	}

	CW_SAFE_DELETE(pViewPort);
	return nullptr;
}

CWBOOL cwD3D11ViewPort::init(
	CWFLOAT fTopLeftX, CWFLOAT fTopLeftY,
	CWFLOAT fWidth, CWFLOAT fHeight,
	CWFLOAT fMinDepth, CWFLOAT fMaxDepth)
{
	if (!cwViewPort::init(fTopLeftX, fTopLeftY, fWidth, fHeight, fMinDepth, fMaxDepth)) return false;

	m_nD3D11ViewPort.TopLeftX = fTopLeftX;
	m_nD3D11ViewPort.TopLeftY = fTopLeftY;
	m_nD3D11ViewPort.Width  = fWidth;
	m_nD3D11ViewPort.Height = fHeight;
	m_nD3D11ViewPort.MinDepth = fMinDepth;
	m_nD3D11ViewPort.MaxDepth = fMaxDepth;

	return true;
}

CWVOID cwD3D11ViewPort::refresh(
	CWFLOAT fTopLeftX, CWFLOAT fTopLeftY,
	CWFLOAT fWidth, CWFLOAT fHeight,
	CWFLOAT fMinDepth, CWFLOAT fMaxDepth)
{
	cwViewPort::refresh(fTopLeftX, fTopLeftY, fWidth, fHeight, fMinDepth, fMaxDepth);

	m_nD3D11ViewPort.TopLeftX = fTopLeftX;
	m_nD3D11ViewPort.TopLeftY = fTopLeftY;
	m_nD3D11ViewPort.Width = fWidth;
	m_nD3D11ViewPort.Height = fHeight;
	m_nD3D11ViewPort.MinDepth = fMinDepth;
	m_nD3D11ViewPort.MaxDepth = fMaxDepth;
}

CWVOID cwD3D11ViewPort::binding()
{
	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());
	pD3D11Device->getD3D11DeviceContext()->RSSetViewports(1, &m_nD3D11ViewPort);
}

NS_MINIR_END

#endif
