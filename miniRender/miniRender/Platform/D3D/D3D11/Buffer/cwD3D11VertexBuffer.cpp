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

#include "cwD3D11VertexBuffer.h"
#include "Device/cwDevice.h"
#include "Repertory/cwRepertory.h"
#include "Platform/Windows/cwWinUtils.h"
#include "Platform/D3D/D3D11/Device/cwD3D11Device.h"
#include "Platform/D3D/D3D11/cwD3D11Utils.h"

NS_MINIR_BEGIN

cwD3D11VertexBuffer* cwD3D11VertexBuffer::create(
	CWUINT uSize,
	CWUINT structureByteStride,
	CWUINT usage,
	CWUINT uCpuFlag,
	CWUINT miscFlag)
{
	cwD3D11VertexBuffer* pBuffer = new cwD3D11VertexBuffer();
	if (pBuffer && pBuffer->init(uSize, usage, cwD3D11Device::getBufferBindFlag(eBufferBindVertex), uCpuFlag, miscFlag, structureByteStride)) {
		pBuffer->autorelease();
		return pBuffer;
	}

	CW_SAFE_DELETE(pBuffer);
	return nullptr;
}

cwD3D11VertexBuffer::cwD3D11VertexBuffer()
{
	m_nStride = 0;
	m_nOffset = 0;
}

cwD3D11VertexBuffer::~cwD3D11VertexBuffer()
{
	ID3D11Buffer* pD3D11Buffer = static_cast<ID3D11Buffer*>(m_pDRenderBuffer);
	CW_RELEASE_COM(pD3D11Buffer);
	m_pDRenderBuffer = nullptr;
}

CWBOOL cwD3D11VertexBuffer::init(
	CWUINT uSize,
	CWUINT usage,
	CWUINT bindFlag,
	CWUINT uCpuFlag,
	CWUINT miscFlag,
	CWUINT structureByteStride)
{
	CWUINT uD3D11CpuFlag = cwD3D11Device::getAccessFlag(static_cast<eAccessFlag>(uCpuFlag));
	CWUINT uD3D11Usage = cwD3D11Device::getBufferUsage(static_cast<eBufferUsage>(usage));
	if (!cwBuffer::init(uSize, uD3D11Usage, bindFlag, uD3D11CpuFlag, miscFlag, structureByteStride)) return false;

	return true;
}

CWVOID cwD3D11VertexBuffer::refresh(CWVOID* pData)
{
	if (!pData) return;

	cwD3D11Device* pD3D11Device = static_cast<cwD3D11Device*>(cwRepertory::getInstance().getDevice());

	D3D11_MAPPED_SUBRESOURCE mappedData;
	ID3D11Buffer* pD3D11Buffer = static_cast<ID3D11Buffer*>(m_pDRenderBuffer);
	CW_HR(pD3D11Device->getD3D11DeviceContext()->Map(pD3D11Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

	memcpy(mappedData.pData, pData, this->getSize());

	pD3D11Device->getD3D11DeviceContext()->Unmap(pD3D11Buffer, 0);
}

NS_MINIR_END

#endif
